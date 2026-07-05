#include "common_utils.h"
#include <cstring>

void word::set(const char *s) {
    if (s) {
        size_t len = strlen(s);
        str = std::make_unique<char[]>(len + 1);
        strcpy(str.get(), s);
    } else {
        str.reset();
    }
}

const char *word::get() const {
    return str ? str.get() : "";
}

int word::cmp(const word &other) const {
    return strcmp(get(), other.get());
}

bool word::operator<(const word &other) const { return cmp(other) < 0; }

bool word::operator<=(const word &other) const { return cmp(other) <= 0; }

bool word::operator>(const word &other) const { return cmp(other) > 0; }

bool word::operator>=(const word &other) const { return cmp(other) >= 0; }

bool word::operator==(const word &other) const { return cmp(other) == 0; }

bool word::operator!=(const word &other) const { return cmp(other) != 0; }

const char *word::get_name() const { return get(); }

io_status word::read(FILE *) { return io_status::eof; }

void word::print(FILE *) const {
}

void pair_string::set_key(const char *k) {
    if (k) {
        size_t len = strlen(k);
        key = std::make_unique<char[]>(len + 1);
        strcpy(key.get(), k);
    } else {
        key.reset();
    }
}

void pair_string::set_value(const char *v) {
    if (v) {
        size_t len = strlen(v);
        value = std::make_unique<char[]>(len + 1);
        strcpy(value.get(), v);
    } else {
        value.reset();
    }
}

int pair_string::cmp(const pair_string &other) const {
    const char *a = key ? key.get() : "";
    const char *b = other.key ? other.key.get() : "";
    return strcmp(a, b);
}

bool pair_string::operator<(const pair_string &other) const { return cmp(other) < 0; }

bool pair_string::operator<=(const pair_string &other) const { return cmp(other) <= 0; }

bool pair_string::operator>(const pair_string &other) const { return cmp(other) > 0; }

bool pair_string::operator>=(const pair_string &other) const { return cmp(other) >= 0; }

bool pair_string::operator==(const pair_string &other) const { return cmp(other) == 0; }

bool pair_string::operator!=(const pair_string &other) const { return cmp(other) != 0; }

const char *pair_string::get_name() const { return key ? key.get() : ""; }

io_status pair_string::read(FILE *) { return io_status::eof; }

void pair_string::print(FILE *) const {
}

words::words(const char *delim) {
    count = 0;
    strncpy(spaces, delim, sizeof(spaces) - 1);
    spaces[sizeof(spaces) - 1] = '\0';
    word_ptrs = std::make_unique<char *[]>(MAX_WORDS);
}

parse_result words::parse(const char *str) {
    count = 0;
    char *dst = buff;
    const char *src = str;

    while (*src) {
        while (*src && strchr(spaces, *src)) ++src;
        if (!*src) break;

        word_ptrs[count] = dst;
        while (*src && !strchr(spaces, *src)) {
            if (dst - buff >= LEN - 1)
                return parse_result::buffer_overflow;
            *dst++ = *src++;
        }
        *dst++ = '\0';
        count++;
        if (count >= MAX_WORDS)
            return parse_result::buffer_overflow;
    }
    return (count > 0) ? parse_result::success : parse_result::empty;
}

int words::get_count() const { return count; }

const char *words::get_word(int i) const {
    return (i >= 0 && i < count) ? word_ptrs[i] : nullptr;
}
