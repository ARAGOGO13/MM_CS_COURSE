#include "pattern_utils.h"
#include <cstring>

parse_result parse_conditions(const char *x, const char *delim, condition **conds, int *count) {
    if (!x || !delim) return parse_result::empty;
    words w(delim);
    parse_result res = w.parse(x);
    if (res != parse_result::success) return res;
    *count = w.get_count();
    if (*count == 0) return parse_result::empty;

    *conds = new condition[*count];
    for (int i = 0; i < *count; ++i) {
        const char *token = w.get_word(i);
        if (strcmp(token, "<") == 0) (*conds)[i].oper = op::lt;
        else if (strcmp(token, ">") == 0) (*conds)[i].oper = op::gt;
        else if (strcmp(token, "<=") == 0) (*conds)[i].oper = op::le;
        else if (strcmp(token, ">=") == 0) (*conds)[i].oper = op::ge;
        else if (strcmp(token, "=") == 0) (*conds)[i].oper = op::eq;
        else if (strcmp(token, "<>") == 0) (*conds)[i].oper = op::ne;
        else {
            delete[] *conds;
            return parse_result::buffer_overflow;
        }
        (*conds)[i].word = nullptr;
    }
    return parse_result::success;
}

bool check_condition(const char *w, const char *s_word, op op) {
    int cmp = strcmp(w, s_word);
    switch (op) {
        case op::lt:
            return cmp < 0;
        case op::gt:
            return cmp > 0;
        case op::le:
            return cmp <= 0;
        case op::ge:
            return cmp >= 0;
        case op::eq:
            return cmp == 0;
        case op::ne:
            return cmp != 0;
        default:
            return false;
    }
}

pattern6 *parse_pattern6(const char *s, io_status *err) {
    if (!s) {
        *err = io_status::format;
        return nullptr;
    }
    int len = strlen(s);
    char *pat = new char[len + 1];
    bool *exact = new bool[len];
    int pos = 0;

    for (int i = 0; i < len; ++i) {
        if (s[i] == '\\') {
            if (i + 1 < len) {
                char next = s[i + 1];
                pat[pos] = next;
                exact[pos] = true;
                ++pos;
                ++i;
            } else {
                delete[] pat;
                delete[] exact;
                *err = io_status::format;
                return nullptr;
            }
        } else if (s[i] == '_') {
            pat[pos] = '_';
            exact[pos] = false;
            ++pos;
        } else {
            pat[pos] = s[i];
            exact[pos] = true;
            ++pos;
        }
    }
    pat[pos] = '\0';

    pattern6 *result = new pattern6;
    result->pattern = pat;
    result->len = pos;
    result->exact = exact;
    *err = io_status::success;
    return result;
}

bool match_pattern6(const char *word, const pattern6 *pat) {
    int wlen = strlen(word);
    if (wlen != pat->len) return false;
    for (int i = 0; i < pat->len; ++i) {
        if (pat->exact[i] && word[i] != pat->pattern[i]) return false;
    }
    return true;
}

void free_pattern6(pattern6 *pat) {
    if (pat) {
        delete[] pat->pattern;
        delete[] pat->exact;
        delete pat;
    }
}


static bool match_helper(const char *w, const char *types, const char *symbs, int pos) {
    if (types[pos] == '\0') return *w == '\0';
    if (types[pos] == 's') {
        if (*w != symbs[pos]) return false;
        return match_helper(w + 1, types, symbs, pos + 1);
    } else {
        for (int k = 0; w[k] != '\0'; ++k) {
            if (match_helper(w + k, types, symbs, pos + 1)) return true;
        }
        return match_helper(w + strlen(w), types, symbs, pos + 1);
    }
}

pattern7 *parse_pattern7(const char *s, io_status *err) {
    if (!s) {
        *err = io_status::format;
        return nullptr;
    }
    int len = strlen(s);
    char *symbs = new char[len + 1];
    char *types = new char[len + 1];
    int pos = 0;

    for (int i = 0; i < len; ++i) {
        if (s[i] == '\\') {
            if (i + 1 < len) {
                char next = s[i + 1];
                symbs[pos] = next;
                types[pos] = 's';
                ++pos;
                ++i;
            } else {
                delete[] symbs;
                delete[] types;
                *err = io_status::format;
                return nullptr;
            }
        } else if (s[i] == '%') {
            symbs[pos] = '%';
            types[pos] = 'p';
            ++pos;
        } else {
            symbs[pos] = s[i];
            types[pos] = 's';
            ++pos;
        }
    }
    symbs[pos] = '\0';
    types[pos] = '\0';

    pattern7 *result = new pattern7;
    result->symbs = symbs;
    result->types = types;
    result->len = pos;
    *err = io_status::success;
    return result;
}

bool match_pattern7(const char *word, const pattern7 *pat) {
    return match_helper(word, pat->types, pat->symbs, 0);
}

void free_pattern7(pattern7 *pat) {
    if (pat) {
        delete[] pat->symbs;
        delete[] pat->types;
        delete pat;
    }
}

pattern8_item *parse_pattern8(const char *s, int *out_len, io_status *err) {
    if (!s) {
        *err = io_status::format;
        return nullptr;
    }
    int len = strlen(s);
    if (len == 0 || (len > 0 && s[len - 1] == '\\')) {
        *err = io_status::format;
        return nullptr;
    }

    pattern8_item *items = nullptr;
    int capacity = 0, count = 0;
    const char *p = s;

    while (*p) {
        if (*p == '\\') {
            p++;
            if (!*p) {
                delete[] items;
                *err = io_status::format;
                return nullptr;
            }
            if (count >= capacity) {
                capacity = capacity ? capacity * 2 : 8;
                pattern8_item *new_items = new pattern8_item[capacity];
                for (int i = 0; i < count; ++i) new_items[i] = items[i];
                delete[] items;
                items = new_items;
            }
            items[count].type = pattern8_item::CHAR;
            items[count].ch = *p;
            count++;
            p++;
        } else if (*p == '[') {
            p++;
            if (!*p) {
                delete[] items;
                *err = io_status::format;
                return nullptr;
            }
            bool invert = false;
            if (*p == '^') {
                invert = true;
                p++;
                if (!*p) {
                    delete[] items;
                    *err = io_status::format;
                    return nullptr;
                }
            }
            char from = *p;
            p++;
            if (!*p || *p != '-') {
                delete[] items;
                *err = io_status::format;
                return nullptr;
            }
            p++;
            if (!*p) {
                delete[] items;
                *err = io_status::format;
                return nullptr;
            }
            char to = *p;
            p++;
            if (!*p || *p != ']') {
                delete[] items;
                *err = io_status::format;
                return nullptr;
            }
            p++;
            if (count >= capacity) {
                capacity = capacity ? capacity * 2 : 8;
                pattern8_item *new_items = new pattern8_item[capacity];
                for (int i = 0; i < count; ++i) new_items[i] = items[i];
                delete[] items;
                items = new_items;
            }
            items[count].type = pattern8_item::RANGE;
            items[count].from = from;
            items[count].to = to;
            items[count].invert = invert;
            count++;
        } else {
            if (count >= capacity) {
                capacity = capacity ? capacity * 2 : 8;
                pattern8_item *new_items = new pattern8_item[capacity];
                for (int i = 0; i < count; ++i) new_items[i] = items[i];
                delete[] items;
                items = new_items;
            }
            items[count].type = pattern8_item::CHAR;
            items[count].ch = *p;
            count++;
            p++;
        }
    }
    *out_len = count;
    return items;
}

bool match_pattern8(const char *word, const pattern8_item *items, int len) {
    int wlen = strlen(word);
    if (wlen != len) return false;
    for (int i = 0; i < len; ++i) {
        if (items[i].type == pattern8_item::CHAR) {
            if (word[i] != items[i].ch) return false;
        } else {
            char c = word[i];
            if (c < items[i].from || c > items[i].to) return false;
        }
    }
    return true;
}

pattern9_item *parse_pattern9(const char *s, int *out_len, io_status *err) {
    if (!s) {
        *err = io_status::format;
        return nullptr;
    }
    int len = strlen(s);
    if (len == 0) {
        *err = io_status::format;
        return nullptr;
    }
    if (s[len - 1] == '\\') {
        *err = io_status::format;
        return nullptr;
    }

    pattern9_item *items = nullptr;
    int capacity = 0;
    int count = 0;
    const char *p = s;

    auto read_char = [&](char &out, bool &escaped) -> bool {
        if (*p == '\\') {
            p++;
            if (!*p) return false;
            out = *p;
            escaped = true;
            p++;
            return true;
        } else {
            out = *p;
            escaped = false;
            p++;
            return true;
        }
    };

    while (*p) {
        if (*p == '\\') {
            char c;
            bool esc;
            if (!read_char(c, esc)) {
                delete[] items;
                *err = io_status::format;
                return nullptr;
            }
            if (count >= capacity) {
                capacity = capacity ? capacity * 2 : 8;
                pattern9_item *new_items = new pattern9_item[capacity];
                for (int i = 0; i < count; ++i) new_items[i] = items[i];
                delete[] items;
                items = new_items;
            }
            items[count].type = pattern9_item::CHAR;
            items[count].ch = c;
            count++;
        } else if (*p == '[') {
            p++;
            if (!*p) {
                delete[] items;
                *err = io_status::format;
                return nullptr;
            }
            bool invert = false;
            if (*p == '^') {
                invert = true;
                p++;
                if (!*p) {
                    delete[] items;
                    *err = io_status::format;
                    return nullptr;
                }
            }
            char from;
            bool esc_from;
            if (!read_char(from, esc_from)) {
                delete[] items;
                *err = io_status::format;
                return nullptr;
            }
            if (*p != '-') {
                delete[] items;
                *err = io_status::format;
                return nullptr;
            }
            p++;
            if (!*p) {
                delete[] items;
                *err = io_status::format;
                return nullptr;
            }
            char to;
            bool esc_to;
            if (!read_char(to, esc_to)) {
                delete[] items;
                *err = io_status::format;
                return nullptr;
            }
            if (*p != ']') {
                delete[] items;
                *err = io_status::format;
                return nullptr;
            }
            p++;

            if (count >= capacity) {
                capacity = capacity ? capacity * 2 : 8;
                pattern9_item *new_items = new pattern9_item[capacity];
                for (int i = 0; i < count; ++i) new_items[i] = items[i];
                delete[] items;
                items = new_items;
            }
            items[count].type = pattern9_item::RANGE;
            items[count].from = from;
            items[count].to = to;
            items[count].invert = invert;
            count++;
        } else {
            if (count >= capacity) {
                capacity = capacity ? capacity * 2 : 8;
                pattern9_item *new_items = new pattern9_item[capacity];
                for (int i = 0; i < count; ++i) new_items[i] = items[i];
                delete[] items;
                items = new_items;
            }
            items[count].type = pattern9_item::CHAR;
            items[count].ch = *p;
            count++;
            p++;
        }
    }
    *out_len = count;
    return items;
}

bool match_pattern9(const char *word, const pattern9_item *items, int len) {
    int wlen = strlen(word);
    if (wlen != len) return false;
    for (int i = 0; i < len; ++i) {
        if (items[i].type == pattern9_item::CHAR) {
            if (word[i] != items[i].ch) return false;
        } else {
            char c = word[i];
            bool in_range = (c >= items[i].from && c <= items[i].to);
            if (items[i].invert) {
                if (in_range) return false;
            } else {
                if (!in_range) return false;
            }
        }
    }
    return true;
}
