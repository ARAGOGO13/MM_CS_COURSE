#ifndef COMMAND_H
#define COMMAND_H

#include <stdio.h>

#include "command_type.h"
#include "operation.h"
#include "ordering.h"
#include "record.h"

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
  condition get_name_cond() const { return c_name; }
  condition get_phone_cond() const { return c_phone; }
  condition get_group_cond() const { return c_group; }
  operation get_op() const { return op; }

  bool parse(const char *str);

  void print(FILE *fp = stdout) const;

  bool apply(const record &x) const;
};

#endif
