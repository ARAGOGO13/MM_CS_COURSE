#ifndef COMMAND_H
#define COMMAND_H

#include <stdio.h>
#include "record.h"
#include "operation.h"
#include "ordering.h"
#include "command_type.h"

class command : public record {
private:
    static const int max_items = 3;
    command_type type = command_type::none;
    condition c_name = condition::none;
    condition c_phone = condition::none;
    condition c_group = condition::none;
    operation op = operation::none;
    ordering order[max_items] = {};
    ordering order_by[max_items] = {};

public:
    command() = default;

    ~command() = default;

    command_type get_type() const { return type; }

    const ordering *get_order() const { return order; }

    const ordering *get_order_by() const { return order_by; }

    bool parse(const char *str);

    void print(FILE *fp = stdout) const;

    bool apply(const record &x) const;
};

#endif