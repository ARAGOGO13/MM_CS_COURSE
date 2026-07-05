#ifndef RECORD_H
#define RECORD_H

#include <memory>
#include <stdio.h>
#include "condition.h"
#include "ordering.h"
#include "io.h"

class record {
private:
    std::unique_ptr<char[]> name = nullptr;
    int phone = 0;
    int group = 0;

public:
    record() = default;

    ~record() = default;

    const char *get_name() const { return name ? name.get() : ""; }
    int get_phone() const { return phone; }
    int get_group() const { return group; }

    int init(const char *n, int p, int g);

    record(record &&) = default;

    record &operator=(record &&) = default;

    record(const record &) = delete;

    record &operator=(const record &) = delete;

    bool compare_name(condition x, const record &y) const;

    bool compare_phone(condition x, const record &y) const;

    bool compare_group(condition x, const record &y) const;

    bool is_less(const record &other, ordering field) const;

    bool equals_all(const record &other) const;

    void print(const ordering order[] = nullptr, FILE *fp = stdout) const;

    io_status read(FILE *fp = stdin);
};

#endif
