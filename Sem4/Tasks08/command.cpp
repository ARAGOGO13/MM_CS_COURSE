#include "command.h"

#include <stdlib.h>
#include <string.h>

#include "io.h"

static inline bool is_space(char c) {
  return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' ||
         c == '\f';
}

static const char *skip_spaces(const char *p) {
  while (p && *p && is_space(*p)) ++p;
  return p;
}

static bool starts_with(const char *s, const char *prefix) {
  while (*prefix) {
    if (*s != *prefix) return false;
    ++s;
    ++prefix;
  }
  return true;
}

static bool is_integer(const char *s) {
  if (*s == '-' || *s == '+') ++s;
  if (!*s) return false;
  while (*s) {
    if (*s < '0' || *s > '9') return false;
    ++s;
  }
  return true;
}

static bool read_pattern_char(const char *&p, char &out, bool &ok) {
  if (*p == '\0') {
    ok = false;
    return false;
  }
  if (*p == '\\') {
    ++p;
    if (*p == '\0') {
      ok = false;
      return false;
    }
    out = *p++;
    ok = true;
    return true;
  } else {
    out = *p++;
    ok = true;
    return true;
  }
}

static bool match_bracket_class(const char *&pat, char text_char, bool &ok) {
  ok = false;
  if (*pat == '\0') return false;
  bool invert = false;
  if (*pat == '^' && *(pat + 1) != '-') {
    invert = true;
    ++pat;
  }
  bool matched = false;
  while (*pat != '\0' && *pat != ']') {
    char a;
    bool r;
    if (!read_pattern_char(pat, a, r)) return false;
    if (!r) return false;
    if (*pat == '-' && *(pat + 1) != ']' && *(pat + 1) != '\0') {
      ++pat;
      char b;
      if (!read_pattern_char(pat, b, r)) return false;
      if (!r) return false;
      if (a <= b) {
        if (text_char >= a && text_char <= b) matched = true;
      } else {
        if (text_char >= a || text_char <= b) matched = true;
      }
    } else {
      if (text_char == a) matched = true;
    }
  }
  if (*pat != ']') return false;
  ++pat;
  ok = true;
  return invert ? !matched : matched;
}

static bool match_like_rec(const char *s, const char *p) {
  if (*p == '\0') return *s == '\0';

  if (*p == '\\') {
    ++p;
    if (*p == '\0') return false;
    if (*s == '\0') return false;
    if (*s != *p) return false;
    ++s;
    ++p;
    return match_like_rec(s, p);
  }

  if (*p == '_') {
    if (*s == '\0') return false;
    ++s;
    ++p;
    return match_like_rec(s, p);
  }

  if (*p == '%') {
    ++p;
    if (*p == '\0') return true;
    const char *ss = s;
    while (true) {
      if (match_like_rec(ss, p)) return true;
      if (*ss == '\0') break;
      ++ss;
    }
    return false;
  }

  if (*p == '[') {
    ++p;
    if (*s == '\0') return false;
    bool ok = false;
    bool matched = match_bracket_class(p, *s, ok);
    if (!ok) return false;
    if (!matched) return false;
    ++s;
    return match_like_rec(s, p);
  }

  if (*s == '\0') return false;
  if (*p != *s) return false;
  ++s;
  ++p;
  return match_like_rec(s, p);
}

static bool parse_ordering(const char *&ptr, ordering order[3]) {
  order[0] = ordering::none;
  order[1] = ordering::none;
  order[2] = ordering::none;
  ptr = skip_spaces(ptr);
  if (*ptr == '*') {
    order[0] = ordering::name;
    order[1] = ordering::phone;
    order[2] = ordering::group;
    ptr++;
    ptr = skip_spaces(ptr);
    return true;
  }
  int idx = 0;
  while (idx < 3) {
    ptr = skip_spaces(ptr);
    if (starts_with(ptr, "name") &&
        (is_space(ptr[4]) || ptr[4] == ',' || ptr[4] == '\0')) {
      order[idx++] = ordering::name;
      ptr += 4;
    } else if (starts_with(ptr, "phone") &&
               (is_space(ptr[5]) || ptr[5] == ',' || ptr[5] == '\0')) {
      order[idx++] = ordering::phone;
      ptr += 5;
    } else if (starts_with(ptr, "group") &&
               (is_space(ptr[5]) || ptr[5] == ',' || ptr[5] == '\0')) {
      order[idx++] = ordering::group;
      ptr += 5;
    } else {
      return false;
    }
    ptr = skip_spaces(ptr);
    if (*ptr == ',') {
      ptr++;
    } else {
      break;
    }
  }
  return idx > 0;
}

static const char *parse_one_condition(const char *ptr, int &field,
                                       condition &cond, char *value_str,
                                       int &value_int, bool &is_like) {
  ptr = skip_spaces(ptr);
  char field_buf[64];
  int i = 0;
  while (*ptr && !is_space(*ptr) && i < 63) {
    field_buf[i++] = *ptr++;
  }
  field_buf[i] = '\0';
  if (i == 0) return nullptr;
  ptr = skip_spaces(ptr);

  char op_buf[16] = {0};
  if (starts_with(ptr, "not") && is_space(ptr[3])) {
    const char *tmp = skip_spaces(ptr + 3);
    if (starts_with(tmp, "like") && (is_space(tmp[4]) || tmp[4] == '\0')) {
      strcpy(op_buf, "not like");
      ptr = tmp + 4;
    } else {
      return nullptr;
    }
  } else if (starts_with(ptr, "like") && (is_space(ptr[4]) || ptr[4] == '\0')) {
    strcpy(op_buf, "like");
    ptr += 4;
  } else {
    i = 0;
    if (*ptr == '<') {
      op_buf[i++] = *ptr++;
      if (*ptr == '>' || *ptr == '=') op_buf[i++] = *ptr++;
    } else if (*ptr == '>') {
      op_buf[i++] = *ptr++;
      if (*ptr == '=') op_buf[i++] = *ptr++;
    } else if (*ptr == '=') {
      op_buf[i++] = *ptr++;
    } else {
      return nullptr;
    }
    op_buf[i] = '\0';
  }
  if (op_buf[0] == '\0') return nullptr;
  ptr = skip_spaces(ptr);

  char val_buf[LEN];
  i = 0;
  while (*ptr && i < LEN - 1) {
    if (is_space(*ptr)) {
      const char *tmp = skip_spaces(ptr);
      if (starts_with(tmp, "and") && is_space(tmp[3])) break;
      if (starts_with(tmp, "or") && is_space(tmp[2])) break;
      if (starts_with(tmp, "order")) break;
    }
    val_buf[i++] = *ptr++;
  }
  val_buf[i] = '\0';
  while (i > 0 && is_space(val_buf[i - 1])) val_buf[--i] = '\0';
  if (i == 0) return nullptr;

  if (strcmp(field_buf, "name") == 0)
    field = 0;
  else if (strcmp(field_buf, "phone") == 0)
    field = 1;
  else if (strcmp(field_buf, "group") == 0)
    field = 2;
  else
    return nullptr;

  if (strcmp(op_buf, "like") == 0)
    cond = condition::like;
  else if (strcmp(op_buf, "not like") == 0)
    cond = condition::nlike;
  else if (strcmp(op_buf, "=") == 0)
    cond = condition::eq;
  else if (strcmp(op_buf, "<>") == 0)
    cond = condition::ne;
  else if (strcmp(op_buf, "<") == 0)
    cond = condition::lt;
  else if (strcmp(op_buf, ">") == 0)
    cond = condition::gt;
  else if (strcmp(op_buf, "<=") == 0)
    cond = condition::le;
  else if (strcmp(op_buf, ">=") == 0)
    cond = condition::ge;
  else
    return nullptr;

  if (field == 0) {
    strcpy(value_str, val_buf);
    is_like = (cond == condition::like || cond == condition::nlike);
    value_int = 0;
  } else {
    if (cond == condition::like || cond == condition::nlike) return nullptr;
    if (!is_integer(val_buf)) return nullptr;
    value_int = atoi(val_buf);
    value_str[0] = '\0';
    is_like = false;
  }
  return ptr;
}

bool command::parse(const char *str) {
  type = command_type::none;
  c_name = c_phone = c_group = condition::none;
  op = operation::none;
  for (int i = 0; i < 3; ++i) order[i] = order_by[i] = ordering::none;
  init("", 0, 0);

  if (!str) return false;
  const char *p = skip_spaces(str);
  if (!*p) return false;

  if (starts_with(p, "quit") &&
      (is_space(p[4]) || p[4] == ';' || p[4] == '\0')) {
    type = command_type::quit;
    return true;
  }

  if (starts_with(p, "insert")) {
    type = command_type::insert;
    p = skip_spaces(p + 6);
    if (*p != '(') return false;
    p = skip_spaces(p + 1);

    char name_buf[LEN] = {0};
    int i = 0;
    while (*p && !is_space(*p) && *p != ',' && i < LEN - 1)
      name_buf[i++] = *p++;
    name_buf[i] = '\0';
    p = skip_spaces(p);
    if (*p != ',') return false;
    p = skip_spaces(p + 1);

    char ph_buf[32] = {0};
    i = 0;
    while (*p && !is_space(*p) && *p != ',' && i < 31) ph_buf[i++] = *p++;
    ph_buf[i] = '\0';
    if (!is_integer(ph_buf)) return false;
    int ph = atoi(ph_buf);
    p = skip_spaces(p);
    if (*p != ',') return false;
    p = skip_spaces(p + 1);

    char gr_buf[32] = {0};
    i = 0;
    while (*p && !is_space(*p) && *p != ')' && i < 31) gr_buf[i++] = *p++;
    gr_buf[i] = '\0';
    if (!is_integer(gr_buf)) return false;
    int gr = atoi(gr_buf);
    p = skip_spaces(p);
    if (*p != ')') return false;

    init(name_buf, ph, gr);
    return true;
  }

  if (starts_with(p, "delete")) {
    type = command_type::del;
    p += 6;
  } else if (starts_with(p, "select")) {
    type = command_type::select;
    p += 6;
    if (!parse_ordering(p, order)) return false;
  } else {
    return false;
  }

  p = skip_spaces(p);
  if (starts_with(p, "where") && is_space(p[5])) {
    p += 5;
    p = skip_spaces(p);

    int fields_set = 0;
    char name_val[LEN] = "";
    int phone_val = 0, group_val = 0;
    condition c_name_tmp = condition::none;
    condition c_phone_tmp = condition::none;
    condition c_group_tmp = condition::none;
    operation op_tmp = operation::none;

    while (*p) {
      int field;
      condition cond;
      char val_str[LEN];
      int val_int;
      bool is_like;
      const char *next =
          parse_one_condition(p, field, cond, val_str, val_int, is_like);
      if (!next) return false;
      p = next;

      int bit = 1 << field;
      if (fields_set & bit) return false;
      fields_set |= bit;

      if (field == 0) {
        c_name_tmp = cond;
        strcpy(name_val, val_str);
      } else if (field == 1) {
        c_phone_tmp = cond;
        phone_val = val_int;
      } else {
        c_group_tmp = cond;
        group_val = val_int;
      }

      p = skip_spaces(p);
      if (!*p) break;

      if (starts_with(p, "order") && is_space(p[5])) {
        break;
      }

      operation next_op = operation::none;
      if (starts_with(p, "and") && is_space(p[3])) {
        next_op = operation::land;
        p += 3;
      } else if (starts_with(p, "or") && is_space(p[2])) {
        next_op = operation::lor;
        p += 2;
      } else {
        return false;
      }
      if (op_tmp == operation::none)
        op_tmp = next_op;
      else if (op_tmp != next_op)
        return false;
      p = skip_spaces(p);
    }
    c_name = c_name_tmp;
    c_phone = c_phone_tmp;
    c_group = c_group_tmp;
    op = op_tmp;
    const char *final_name = (c_name != condition::none) ? name_val : "";
    init(final_name, phone_val, group_val);
  }

  if (type == command_type::select) {
    p = skip_spaces(p);
    if (starts_with(p, "order") && is_space(p[5])) {
      p += 5;
      p = skip_spaces(p);
      if (starts_with(p, "by") && (is_space(p[2]) || p[2] == ';')) {
        p += 2;
        p = skip_spaces(p);
        if (!parse_ordering(p, order_by)) return false;
      }
    }
  }

  return true;
}

void command::print(FILE *fp) const {
  fprintf(fp,
          "CMD: type=%d name_cond=%d phone_cond=%d group_cond=%d op=%d "
          "order_by[0]=%d ; name='%s' phone=%d group=%d\n",
          (int)type, (int)c_name, (int)c_phone, (int)c_group, (int)op,
          (int)order_by[0], get_name(), get_phone(), get_group());
}

bool command::apply(const record &x) const {
  bool name_ok = true;
  if (c_name != condition::none) {
    if (c_name == condition::like) {
      name_ok = match_like_rec(x.get_name(), get_name());
    } else if (c_name == condition::nlike) {
      name_ok = !match_like_rec(x.get_name(), get_name());
    } else {
      name_ok = x.compare_name(c_name, *this);
    }
  }

  bool phone_ok = true;
  if (c_phone != condition::none) {
    phone_ok = x.compare_phone(c_phone, *this);
  }

  bool group_ok = true;
  if (c_group != condition::none) {
    group_ok = x.compare_group(c_group, *this);
  }

  if (op == operation::lor) {
    return (c_name != condition::none && name_ok) ||
           (c_phone != condition::none && phone_ok) ||
           (c_group != condition::none && group_ok);
  } else {
    return (c_name == condition::none || name_ok) &&
           (c_phone == condition::none || phone_ok) &&
           (c_group == condition::none || group_ok);
  }
}
