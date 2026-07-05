#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "io.h"
#include "net_utils.h"

static inline int is_space(char c) {
  return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

static const char *skip_spaces(const char *p) {
  while (*p && is_space(*p)) ++p;
  return p;
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s host port\n", argv[0]);
    return 1;
  }

  int port = atoi(argv[2]);
  if (port <= 0 || port > 65535) {
    fprintf(stderr, "%s: invalid port: %s\n", argv[0], argv[2]);
    return 1;
  }

  struct hostent *hostinfo = gethostbyname(argv[1]);
  if (!hostinfo) {
    fprintf(stderr, "%s: unknown host: %s\n", argv[0], argv[1]);
    return 1;
  }

  int sock = socket(PF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    perror("client: socket");
    return 1;
  }

  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons((unsigned short)port);
  memcpy(&server_addr.sin_addr, hostinfo->h_addr, sizeof(server_addr.sin_addr));

  if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
    perror("client: connect");
    close(sock);
    return 1;
  }

  int flag = 1;
  setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (char *)&flag, sizeof(flag));

  char cmd_buf[LEN * 4];
  int cmd_len = 0;
  cmd_buf[0] = '\0';

  char line[LEN * 4];
  char reply[LEN * 4];
  int done = 0;
  int total_found = 0;
  clock_t start_time = clock();

  while (!done && fgets(line, (int)sizeof(line), stdin)) {
    const char *pos = line;

    while (!done && *pos) {
      const char *semi = strchr(pos, ';');

      if (semi) {
        int frag = (int)(semi - pos);
        if (cmd_len + frag < (int)sizeof(cmd_buf) - 1) {
          memcpy(cmd_buf + cmd_len, pos, (size_t)frag);
          cmd_len += frag;
        }
        cmd_buf[cmd_len] = '\0';
        pos = semi + 1;

        const char *trimmed = skip_spaces(cmd_buf);
        if (*trimmed) {
          if (writeToClient(sock, trimmed) != 0) {
            fprintf(stderr, "client: send failed\n");
            done = 1;
            break;
          }

          while (1) {
            int r = readFromClient(sock, reply, (int)sizeof(reply));
            if (r < 0) {
              fprintf(stderr, "client: recv failed\n");
              done = 1;
              break;
            }

            if (strcmp(reply, "QUIT") == 0) {
              done = 1;
              break;
            }

            if (strcmp(reply, "EMPTY") == 0 || reply[0] == '\0') {
              break;
            }

            if (reply[0] != '\0') {
              total_found++;
            }
            fprintf(stdout, "%s", reply);
          }
        }

        cmd_len = 0;
        cmd_buf[0] = '\0';
      } else {
        int frag = (int)strlen(pos);
        if (cmd_len + frag < (int)sizeof(cmd_buf) - 1) {
          memcpy(cmd_buf + cmd_len, pos, (size_t)frag);
          cmd_len += frag;
          cmd_buf[cmd_len] = '\0';
        }
        break;
      }
    }
  }

  close(sock);

  double elapsed = (double)(clock() - start_time) / CLOCKS_PER_SEC;
  fprintf(stdout, "%s : Result = %d Elapsed = %.2f\n", argv[0], total_found,
          elapsed);

  return 0;
}