#ifndef COMMAND_H
#define COMMAND_H

#include <stdio.h>
#include "record.h"
#include "operation.h"

class command : public record {
private:
    condition c_name = condition::none;
    condition c_phone = condition::none;
    condition c_group = condition::none;
    operation op = operation::none;
    ordering order[3] = {ordering::none, ordering::none, ordering::none};

public:
    command() = default;

    ~command() = default;

    const ordering *get_order() const { return order; }

    bool parse(const char *str);

    void print(FILE *fp = stdout) const;

    bool apply(const record &x) const;
};

#endif