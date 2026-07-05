#include "list.h"
#include <cstdio>
#include <ctime>
#include <cstring>

static inline bool is_space(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' || c == '\f';
}

static const char *skip_spaces(const char *p) {
    while (p && *p && is_space(*p)) ++p;
    return p;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s filename\n", argv[0]);
        return 1;
    }

    list2 lst;
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
    clock_t start = clock();

    while (fgets(line, sizeof(line), stdin)) {
        line[strcspn(line, "\r\n")] = '\0';
        const char *pos = line;

        while (true) {
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
                pos = pos + strlen(pos); // конец
            }

            command cmd;
            if (cmd.parse(command_str)) {
                if (cmd.get_type() == command_type::quit) {
                    goto end;
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

    end:
    double elapsed = double(clock() - start) / CLOCKS_PER_SEC;
    printf("%s : Result = %d Elapsed = %.2f\n", argv[0], total_found, elapsed);
    return 0;
}