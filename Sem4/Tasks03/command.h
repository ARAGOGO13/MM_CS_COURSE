#ifndef COMMAND_H
#define COMMAND_H

#include <stdio.h>
#include "record.h"
#include "condition.h"

static inline bool is_space(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v';
}

class command : public record {
private:
    condition c_name = condition::none;
    condition c_phone = condition::none;
    condition c_group = condition::none;

public:
    command() = default;

    ~command() = default;

    bool parse(const char *str);

    void print(FILE *fp = stdout) const;

    bool apply(const record &x) const;
};

#endif