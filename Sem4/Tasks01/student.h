#ifndef STUDENT_H
#define STUDENT_H

#include <cstring>
#include <cstdio>
#include <memory>
#include "io.h"

class student {
private:
    std::unique_ptr<char []> name;
    int value = 0;

public:
    student() = default;
    student(const student &x) = delete;
    student(student &&x) = default;
    ~student() = default;
    student &operator=(const student &x) = delete;
    student &operator=(student &&x) = default;

    void print(FILE *fp = stdout) const {
        if (name && name.get()[0] != '\0')
            fprintf(fp, "%s %d\n", name.get(), value);
        else
            fprintf(fp, "(null) %d\n", value);
    }

    io_status read(FILE *fp = stdin) {
        const int LEN = 1234;
        char n[LEN];
        int v;
        int ret = fscanf(fp, "%s%d", n, &v);
        if (ret == EOF) {
            return io_status::eof;
        }
        if (ret != 2) {
            return io_status::format;
        }
        erase();
        return init(n, v);
    }

    int cmp(const student &x) const {
        const char *a = name ? name.get() : nullptr;
        const char *b = x.name ? x.name.get() : nullptr;

        if (a == nullptr) {
            if (b != nullptr) return -1;
            return value - x.value;
        }
        if (b == nullptr) return 1;
        int res = strcmp(a, b);
        if (res) return res;
        return value - x.value;
    }

    int operator<(const student &x) const { return cmp(x) < 0; }
    int operator<=(const student &x) const { return cmp(x) <= 0; }
    int operator>(const student &x) const { return cmp(x) > 0; }
    int operator>=(const student &x) const { return cmp(x) >= 0; }
    int operator==(const student &x) const { return cmp(x) == 0; }
    int operator!=(const student &x) const { return cmp(x) != 0; }

    const char* get_name() const {
        return name ? name.get() : nullptr;
    }

private:
    io_status init(const char *n, int v) {
        value = v;
        if (n != nullptr) {
            size_t len = strlen(n);
            name = std::make_unique<char []>(len + 1);
            if (name) {
                for (size_t i = 0; i <= len; ++i)
                    name[i] = n[i];
            } else {
                return io_status::memory;
            }
        }
        return io_status::success;
    }

    void erase() {
        value = 0;
        name.reset();
    }
};

#endif
