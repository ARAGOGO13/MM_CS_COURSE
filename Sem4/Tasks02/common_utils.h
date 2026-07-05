#ifndef COMMON_UTILS_H
#define COMMON_UTILS_H

#include <cstdio>
#include <memory>
#include "io.h"

enum class parse_result {
    success,
    buffer_overflow,
    empty
};

class word {
private:
    std::unique_ptr<char[]> str;

public:
    word() = default;

    word(const word &) = delete;

    word(word &&) = default;

    word &operator=(const word &) = delete;

    word &operator=(word &&) = default;

    void set(const char *s);

    const char *get() const;

    int cmp(const word &other) const;

    bool operator<(const word &other) const;

    bool operator<=(const word &other) const;

    bool operator>(const word &other) const;

    bool operator>=(const word &other) const;

    bool operator==(const word &other) const;

    bool operator!=(const word &other) const;

    const char *get_name() const;

    io_status read(FILE *);

    void print(FILE *) const;
};

class pair_string {
public:
    std::unique_ptr<char[]> key;
    std::unique_ptr<char[]> value;

    pair_string() = default;

    pair_string(const pair_string &) = delete;

    pair_string(pair_string &&) = default;

    pair_string &operator=(const pair_string &) = delete;

    pair_string &operator=(pair_string &&) = default;

    void set_key(const char *k);

    void set_value(const char *v);

    int cmp(const pair_string &other) const;

    bool operator<(const pair_string &other) const;

    bool operator<=(const pair_string &other) const;

    bool operator>(const pair_string &other) const;

    bool operator>=(const pair_string &other) const;

    bool operator==(const pair_string &other) const;

    bool operator!=(const pair_string &other) const;

    const char *get_name() const;

    io_status read(FILE *);

    void print(FILE *) const;
};

class words {
protected:
    static const int LEN = 4096;
    static const int MAX_WORDS = 1024;

    int count;
    std::unique_ptr<char *[]> word_ptrs;
    char buff[LEN];
    char spaces[256];

public:
    words(const char *delim);

    words(const words &) = delete;

    words &operator=(const words &) = delete;

    parse_result parse(const char *str);

    int get_count() const;

    const char *get_word(int i) const;
};

#endif
