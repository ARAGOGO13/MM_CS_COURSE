#ifndef STUDENT_H
#define STUDENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum class io_status {
    success,
    eof,
    format,
    memory,
};

class student {
private:
    char *name = nullptr;
    int value = 0;

    static int f(int s, int n, int i);

public:
    student() = default;
    student(const student &x) { init(x.name, x.value); }
    student(student &&x) {
        name = x.name;
        x.name = nullptr;
        value = x.value;
        x.value = 0;
    }
    ~student() { erase(); }

    student &operator=(const student &x) {
        if (this == &x)
            return *this;
        erase();
        init(x.name, x.value);
        return *this;
    }

    student &operator=(student &&x) {
        if (this == &x)
            return *this;
        erase();
        name = x.name;
        x.name = nullptr;
        value = x.value;
        x.value = 0;
        return *this;
    }

    void print() const {
        if (name != nullptr)
            printf("%s %d\n", name, value);
        else
            printf("null %d\n", value);
    }

    io_status read(FILE *fp = stdin) {
        const int LEN = 1234;
        char n[LEN];
        int v;

        if (fscanf(fp, "%s%d", n, &v) != 2) {
            if (!feof(fp))
                return io_status::format;
            return io_status::eof;
        }
        erase();
        return init(n, v);
    }

    io_status init_by_formula(int s, int n, int i) {
        erase();
        const char *student_name = "Student";
        int val = f(s, n, i);
        return init(student_name, val);
    }

    io_status set(const char *n, int v) {
        erase();
        return init(n, v);
    }

    int operator>(const student &x) const { return (cmp(x) > 0 ? 1 : 0); }
    int operator<(const student &x) const { return (cmp(x) < 0 ? 1 : 0); }
    int operator==(const student &x) const { return (cmp(x) == 0 ? 1 : 0); }

private:
    io_status init(const char *n, int v) {
        value = v;
        if (n != nullptr) {
            size_t len = strlen(n);
            name = new char[len + 1];
            if (name != nullptr) {
                for (size_t i = 0; i <= len; i++)
                    name[i] = n[i];
            } else {
                return io_status::memory;
            }
        } else {
            name = nullptr;
        }
        return io_status::success;
    }

    void erase() {
        value = 0;
        if (name != nullptr) {
            delete[] name;
            name = nullptr;
        }
    }

    int cmp(const student &x) const {
        if (name == nullptr) {
            if (x.name != nullptr)
                return -1;
            return value - x.value;
        }
        if (x.name == nullptr)
            return 1;
        int res = strcmp(name, x.name);
        if (res)
            return res;
        return value - x.value;
    }
};

inline int student::f(int s, int n, int i) {
    switch (s) {
        case 1:
            return i;
        case 2:
            return n - i;
        case 3:
            return i / 2;
        case 4:
            return n - i / 2;
        default:
            return 0;
    }
}

#endif
