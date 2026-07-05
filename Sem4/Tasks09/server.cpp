#include <arpa/inet.h>
#include <errno.h>
#include <libgen.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include <memory>

#include "command.h"
#include "io.h"
#include "list.h"
#include "net_utils.h"

#define BACKLOG 3

static inline bool is_space(char c) {
  return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' ||
         c == '\f';
}

static bool read_config(const char *exe_path, int &k_name, int &k_phone) {
  std::unique_ptr<char[]> exe_copy(new char[strlen(exe_path) + 1]);
  strcpy(exe_copy.get(), exe_path);
  char *dir = dirname(exe_copy.get());
  const char *config_name = "config.txt";
  size_t path_len = strlen(dir) + 1 + strlen(config_name) + 1;
  std::unique_ptr<char[]> config_path(new char[path_len]);
  snprintf(config_path.get(), path_len, "%s/%s", dir, config_name);

  FILE *fp = fopen(config_path.get(), "r");
  if (!fp) {
    fprintf(stderr, "server: cannot open config: %s\n", config_path.get());
    return false;
  }

  char line[256];
  int values[2] = {0};
  int idx = 0;
  while (fgets(line, (int)sizeof(line), fp) && idx < 2) {
    const char *p = line;
    while (*p == ' ' || *p == '\t') ++p;
    if (*p == '#' || *p == '\n' || *p == '\0') continue;
    char *end;
    long v = strtol(p, &end, 10);
    if (end != p) values[idx++] = (int)v;
  }
  fclose(fp);

  if (idx < 2 || values[0] <= 0 || values[1] <= 0) {
    fprintf(stderr, "server: config must contain two positive integers\n");
    return false;
  }
  k_name = values[0];
  k_phone = values[1];
  return true;
}

struct client_state {
  int fd;
  int total_found;
  clock_t start_time;
};

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s filename port\n", argv[0]);
    return 1;
  }

  int port;
  if (sscanf(argv[2], "%d", &port) != 1 || port <= 0 || port > 65535) {
    fprintf(stderr, "%s: invalid port: %s\n", argv[0], argv[2]);
    return 1;
  }

  int k_name = 0, k_phone = 0;
  if (!read_config(argv[0], k_name, k_phone)) return 2;

  FILE *fp = fopen(argv[1], "r");
  if (!fp) {
    fprintf(stderr, "server: cannot open file: %s\n", argv[1]);
    return 2;
  }

  list2 lst(k_name, k_phone);
  io_status st = lst.read_from_file(argv[1]);
  fclose(fp);

  if (st == io_status::open) {
    fprintf(stderr, "server: cannot open file: %s\n", argv[1]);
    return 2;
  }
  if (st == io_status::format) {
    fprintf(stderr, "server: format error in: %s\n", argv[1]);
    return 2;
  }
  if (st == io_status::memory) {
    fprintf(stderr, "server: out of memory\n");
    return 2;
  }

  int opt = 1;
  int sock = socket(PF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    perror("Server: cannot create socket");
    return 1;
  }

  setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt));

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons((unsigned short)port);
  addr.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    perror("Server: cannot bind socket");
    close(sock);
    return 1;
  }

  if (listen(sock, BACKLOG) < 0) {
    perror("Server: listen queue failure");
    close(sock);
    return 1;
  }

  client_state clients[FD_SETSIZE];
  for (int i = 0; i < FD_SETSIZE; ++i) clients[i].fd = -1;

  fd_set active_set, read_set;
  FD_ZERO(&active_set);
  FD_SET(sock, &active_set);

  fprintf(stdout, "Ready to receive connections\n");

  char cmd_buf[LEN * 4];
  int server_stop = 0;

  while (!server_stop) {
    read_set = active_set;
    if (select(FD_SETSIZE, &read_set, NULL, NULL, NULL) < 0) {
      if (errno == EINTR) continue;
      perror("Server: select failure");
      break;
    }

    for (int i = 0; i < FD_SETSIZE && !server_stop; ++i) {
      if (!FD_ISSET(i, &read_set)) continue;

      if (i == sock) {
        struct sockaddr_in client;
        socklen_t size = sizeof(client);
        int new_sock = accept(sock, (struct sockaddr *)&client, &size);
        if (new_sock < 0) {
          perror("accept");
          continue;
        }
        fprintf(stdout, "Server: connect from host %s, port %d.\n",
                inet_ntoa(client.sin_addr), (int)ntohs(client.sin_port));
        clients[new_sock].fd = new_sock;
        clients[new_sock].total_found = 0;
        clients[new_sock].start_time = clock();
        FD_SET(new_sock, &active_set);
      } else {
        int err = readFromClient(i, cmd_buf, (int)sizeof(cmd_buf));
        if (err < 0) {
          close(i);
          FD_CLR(i, &active_set);
          clients[i].fd = -1;
          continue;
        }

        // fprintf(stdout, "Server got message: %s\n", cmd_buf);

        if (strncmp(cmd_buf, "stop", 4) == 0 &&
            (cmd_buf[4] == '\0' || is_space(cmd_buf[4]))) {
          double elapsed =
              (double)(clock() - clients[i].start_time) / CLOCKS_PER_SEC;
          printf("%s : Result = %d Elapsed = %.2f\n", argv[0],
                 clients[i].total_found, elapsed);
          writeToClient(i, "QUIT");
          close(i);
          FD_CLR(i, &active_set);
          clients[i].fd = -1;
          close(sock);
          server_stop = 1;
          continue;
        }

        if (strncmp(cmd_buf, "quit", 4) == 0 &&
            (cmd_buf[4] == '\0' || is_space(cmd_buf[4]))) {
          double elapsed =
              (double)(clock() - clients[i].start_time) / CLOCKS_PER_SEC;
          printf("%s : Result = %d Elapsed = %.2f\n", argv[0],
                 clients[i].total_found, elapsed);
          writeToClient(i, "QUIT");
          close(i);
          FD_CLR(i, &active_set);
          clients[i].fd = -1;
          continue;
        }

        command cmd;
        if (!cmd.parse(cmd_buf)) {
          writeToClient(i, "EMPTY");
          continue;
        }

        command_type ct = cmd.get_type();

        if (ct == command_type::select) {
          int found = lst.select_and_send(cmd, i);
          clients[i].total_found += found;
        } else if (ct == command_type::insert) {
          lst.insert(cmd);
          writeToClient(i, "EMPTY");
        } else if (ct == command_type::del) {
          lst.remove(cmd);
          writeToClient(i, "EMPTY");
        } else {
          writeToClient(i, "EMPTY");
        }
      }
    }
  }

  for (int i = 0; i < FD_SETSIZE; ++i)
    if (clients[i].fd >= 0) close(clients[i].fd);

  fprintf(stdout, "server: stopped\n");
  return 0;
}