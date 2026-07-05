#include <libgen.h>
#include <stdlib.h>

#include <cstdio>
#include <cstring>
#include <ctime>
#include <memory>

#include "command.h"
#include "list.h"

static inline bool is_space(char c) {
  return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' ||
         c == '\f';
}

static const char *skip_spaces(const char *p) {
  while (p && *p && is_space(*p)) ++p;
  return p;
}

static bool read_config(const char *exe_path, int &k_name, int &k_phone) {
  size_t len = strlen(exe_path);
  std::unique_ptr<char[]> exe_copy(new char[len + 1]);
  strcpy(exe_copy.get(), exe_path);
  char *dir = dirname(exe_copy.get());
  const char *config_name = "config.txt";
  size_t path_len = strlen(dir) + 1 + strlen(config_name) + 1;
  std::unique_ptr<char[]> config_path(new char[path_len]);
  snprintf(config_path.get(), path_len, "%s/%s", dir, config_name);

  FILE *fp = fopen(config_path.get(), "r");
  if (!fp) {
    fprintf(stderr, "Cannot open config file: %s\n", config_path.get());
    return false;
  }

  char line[256];
  int values[2] = {0};
  int idx = 0;
  while (fgets(line, sizeof(line), fp) && idx < 2) {
    const char *p = skip_spaces(line);
    if (*p == '\0' || *p == '#') continue;
    char *endptr;
    long val = strtol(p, &endptr, 10);
    if (endptr != p && (*endptr == '\0' || is_space(*endptr))) {
      values[idx++] = (int)val;
    }
  }
  fclose(fp);

  if (idx < 2 || values[0] <= 0 || values[1] <= 0) {
    fprintf(stderr,
            "Config must contain two positive integers (k_name k_phone)\n");
    return false;
  }
  k_name = values[0];
  k_phone = values[1];
  return true;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s filename\n", argv[0]);
    return 1;
  }

  int k_name = 0, k_phone = 0;
  if (!read_config(argv[0], k_name, k_phone)) {
    return 2;
  }

  list2 lst(k_name, k_phone);
  io_status st = lst.read_from_file(argv[1]);

  switch (st) {
    case io_status::success:
      break;
    case io_status::open:
      fprintf(stderr, "Cannot open file %s\n", argv[1]);
      return 2;
    case io_status::format:
      fprintf(stderr, "File format error in %s\n", argv[1]);
      return 2;
    case io_status::memory:
      fprintf(stderr, "Memory allocation error\n");
      return 2;
    default:
      fprintf(stderr, "Unknown error reading file\n");
      return 2;
  }

  char line[LEN];
  int total_found = 0;
  clock_t start_commands = clock();

  bool done = false;
  while (fgets(line, sizeof(line), stdin) && !done) {
    line[strcspn(line, "\r\n")] = '\0';
    const char *pos = line;

    while (*pos && !done) {
      pos = skip_spaces(pos);
      if (!*pos) break;

      const char *semicolon = strchr(pos, ';');
      char command_str[LEN];

      if (semicolon) {
        size_t len = semicolon - pos;
        if (len >= LEN) len = LEN - 1;
        strncpy(command_str, pos, len);
        command_str[len] = '\0';
        pos = semicolon + 1;
      } else {
        strncpy(command_str, pos, LEN - 1);
        command_str[LEN - 1] = '\0';
        pos = pos + strlen(pos);
      }

      command cmd;
      if (cmd.parse(command_str)) {
        if (cmd.get_type() == command_type::quit) {
          done = true;
          break;
        }
        if (cmd.get_type() == command_type::select) {
          total_found += lst.select_and_print(cmd, stdout);
        } else if (cmd.get_type() == command_type::insert) {
          lst.insert(cmd);
        } else if (cmd.get_type() == command_type::del) {
          lst.remove(cmd);
        }
      }

      if (!semicolon) break;
    }
  }

  double elapsed_commands = double(clock() - start_commands) / CLOCKS_PER_SEC;
  printf("%s : Result = %d Elapsed = %.2f\n", argv[0], total_found,
         elapsed_commands);
  return 0;
}