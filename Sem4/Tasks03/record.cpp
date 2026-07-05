#include "record.h"
#include <string.h>

int record::init(const char *n, int p, int g) {
    phone = p;
    group = g;
    if (n) {
        size_t l = strlen(n);
        name = std::make_unique<char[]>(l + 1);
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
    int rphone = 0;
    int rgroup = 0;
    if (fscanf(fp, "%s%d%d", buf, &rphone, &rgroup) != 3) {
        if (feof(fp)) return io_status::eof;
        return io_status::format;
    }
    if (init(buf, rphone, rgroup)) return io_status::memory;
    return io_status::success;
}

void record::print(FILE *fp) const {
    fprintf(fp, "%s %d %d\n", get_name(), phone, group);
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
            char startc = a, endc = b;
            if (startc <= endc) {
                if (text_char >= startc && text_char <= endc) matched = true;
            } else {
                if (text_char >= startc || text_char <= endc) matched = true;
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

bool record::compare_name(condition x, const record &y) const {
    switch (x) {
        case condition::none:
            return true;
        case condition::eq:
            return strcmp(get_name(), y.get_name()) == 0;
        case condition::ne:
            return strcmp(get_name(), y.get_name()) != 0;
        case condition::lt:
            return strcmp(get_name(), y.get_name()) < 0;
        case condition::gt:
            return strcmp(get_name(), y.get_name()) > 0;
        case condition::le:
            return strcmp(get_name(), y.get_name()) <= 0;
        case condition::ge:
            return strcmp(get_name(), y.get_name()) >= 0;
        case condition::like:
            return match_like_rec(get_name(), y.get_name());
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
            return false;
        default:
            return false;
    }
}