#include <cstdio>
#include <ctime>
#include <cstring>
#include "list.h"
#include "command.h"

static inline bool is_space(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' || c == '\f';
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s filename\n", argv[0]);
        return 1;
    }
    const char *filename = argv[1];

    list2 lst;
    io_status st = lst.read_from_file(filename);

    do {
        switch (st) {
            case io_status::success:
                continue;
            case io_status::open:
                fprintf(stderr, "Cannot open file %s\n", filename);
                break;
            case io_status::format:
                fprintf(stderr, "File format error in %s\n", filename);
                break;
            case io_status::memory:
                fprintf(stderr, "Memory allocation error\n");
                break;
            case io_status::eof:
                fprintf(stderr, "Unexpected EOF in %s\n", filename);
                break;
            default:
                fprintf(stderr, "Unknown error reading file\n");
                break;
        }
        return 2;
    } while (0);

    char cmdline[LEN];
    int total_found = 0;
    clock_t start = clock();

    while (fgets(cmdline, sizeof(cmdline), stdin)) {
        cmdline[strcspn(cmdline, "\r\n")] = '\0';

        const char *p = cmdline;
        while (*p && is_space(*p)) ++p;
        if (*p == '\0') continue;

        command cmd;
        if (cmd.parse(cmdline)) {
            int found = lst.apply_and_print(cmd, stdout);
            total_found += found;
        }
    }

    double elapsed = double(clock() - start) / CLOCKS_PER_SEC;
    printf("%s : Result = %d Elapsed = %.2f\n", argv[0], total_found, elapsed);
    return 0;
}