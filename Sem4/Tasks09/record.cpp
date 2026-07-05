#include "record.h"

#include <cstdio>
#include <cstring>

#include "net_utils.h"

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

bool record::compare_name(condition x, const record &y) const {
  const char *a = get_name();
  const char *b = y.get_name();
  int cmp = strcmp(a ? a : "", b ? b : "");

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
  }
  return false;
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
  }
  return false;
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
  }
  return false;
}

void record::print(const ordering order[], FILE *fp) const {
  const int max_items = 3;
  const ordering default_ordering[max_items] = {ordering::name, ordering::phone,
                                                ordering::group};

  const ordering *p = (order ? order : default_ordering);

  for (int i = 0; i < max_items; i++) {
    switch (p[i]) {
      case ordering::name:
        fprintf(fp, " %s", get_name());
        break;
      case ordering::phone:
        fprintf(fp, " %d", get_phone());
        break;
      case ordering::group:
        fprintf(fp, " %d", get_group());
        break;
      case ordering::none:
        continue;
    }
  }
  fprintf(fp, "\n");
}

void record::print_to_fd(int fd, const ordering order[]) const {
  const int max_items = 3;
  const ordering default_ordering[max_items] = {ordering::name, ordering::phone,
                                                ordering::group};

  const ordering *p = (order ? order : default_ordering);

  char buf[LEN];
  buf[0] = '\0';

  for (int i = 0; i < max_items; i++) {
    int pos = (int)strlen(buf);
    switch (p[i]) {
      case ordering::name:
        snprintf(buf + pos, (size_t)(LEN - pos), "%s%s", (pos ? " " : ""),
                 get_name());
        break;
      case ordering::phone:
        snprintf(buf + pos, (size_t)(LEN - pos), "%s%d", (pos ? " " : ""),
                 get_phone());
        break;
      case ordering::group:
        snprintf(buf + pos, (size_t)(LEN - pos), "%s%d", (pos ? " " : ""),
                 get_group());
        break;
      case ordering::none:
        continue;
    }
  }

  int pos = (int)strlen(buf);
  if (pos < LEN - 1) {
    buf[pos++] = '\n';
    buf[pos] = '\0';
  } else {
    buf[LEN - 1] = '\0';
  }

  writeToClient(fd, buf);
}