#include "record.h"

#include <stdio.h>
#include <string.h>

using namespace std;

int record::init(const char *n, int p, int g) {
  phone = p;
  group = g;
  if (n) {
    size_t l = strlen(n);
    name = make_unique<char[]>(l + 1);
    if (!name) return -1;
    strcpy(name.get(), n);
  } else {
    name.reset();
  }
  return 0;
}

io_status record::read(FILE *fp) {
  char buf[LEN];
  name.reset();
  int rphone = 0, rgroup = 0;
  if (fscanf(fp, "%s%d%d", buf, &rphone, &rgroup) != 3) {
    if (feof(fp)) return io_status::eof;
    return io_status::format;
  }
  if (init(buf, rphone, rgroup)) return io_status::memory;
  return io_status::success;
}

void record::print(const ordering order[], FILE *fp) const {
  const int max_items = 3;
  const ordering default_ordering[max_items] = {ordering::name, ordering::phone,
                                                ordering::group};
  const ordering *p = (order ? order : default_ordering);
  bool first = true;
  for (int i = 0; i < max_items; ++i) {
    switch (p[i]) {
      case ordering::name:
        if (!first) fprintf(fp, " ");
        fprintf(fp, "%s", get_name());
        first = false;
        break;
      case ordering::phone:
        if (!first) fprintf(fp, " ");
        fprintf(fp, "%d", phone);
        first = false;
        break;
      case ordering::group:
        if (!first) fprintf(fp, " ");
        fprintf(fp, "%d", group);
        first = false;
        break;
      case ordering::none:
        continue;
    }
  }
  fprintf(fp, "\n");
}

bool record::compare_name(condition x, const record &y) const {
  int cmp = strcmp(get_name(), y.get_name());
  switch (x) {
    case condition::none:
      return true;
    case condition::eq:
      return cmp == 0;
    case condition::ne:
      return cmp != 0;
    case condition::lt:
      return cmp < 0;
    case condition::gt:
      return cmp > 0;
    case condition::le:
      return cmp <= 0;
    case condition::ge:
      return cmp >= 0;
    case condition::like:
    case condition::nlike:
      return false;
    default:
      return false;
  }
}

bool record::compare_phone(condition x, const record &y) const {
  switch (x) {
    case condition::none:
      return true;
    case condition::eq:
      return phone == y.phone;
    case condition::ne:
      return phone != y.phone;
    case condition::lt:
      return phone < y.phone;
    case condition::gt:
      return phone > y.phone;
    case condition::le:
      return phone <= y.phone;
    case condition::ge:
      return phone >= y.phone;
    case condition::like:
    case condition::nlike:
      return false;
    default:
      return false;
  }
}

bool record::compare_group(condition x, const record &y) const {
  switch (x) {
    case condition::none:
      return true;
    case condition::eq:
      return group == y.group;
    case condition::ne:
      return group != y.group;
    case condition::lt:
      return group < y.group;
    case condition::gt:
      return group > y.group;
    case condition::le:
      return group <= y.group;
    case condition::ge:
      return group >= y.group;
    case condition::like:
    case condition::nlike:
      return false;
    default:
      return false;
  }
}

bool record::is_less(const record &other, ordering field) const {
  switch (field) {
    case ordering::name:
      return strcmp(get_name(), other.get_name()) < 0;
    case ordering::phone:
      return phone < other.phone;
    case ordering::group:
      return group < other.group;
    default:
      return false;
  }
}

bool record::equals_all(const record &other) const {
  return compare_name(condition::eq, other) &&
         compare_phone(condition::eq, other) &&
         compare_group(condition::eq, other);
}
