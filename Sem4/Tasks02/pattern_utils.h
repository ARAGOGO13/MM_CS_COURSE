#ifndef PATTERN_UTILS_H
#define PATTERN_UTILS_H

#include "common_utils.h"

enum class op {
    lt, gt, le, ge, eq, ne
};

struct condition {
    char *word;
    op oper;
};

parse_result parse_conditions(const char *x, const char *delim, condition **conds, int *count);
bool check_condition(const char *w, const char *s_word, op op);

struct pattern6 {
    char *pattern;
    int len;
    bool *exact;
};

pattern6 *parse_pattern6(const char *s, io_status *err);
bool match_pattern6(const char *word, const pattern6 *pat);
void free_pattern6(pattern6 *pat);

struct pattern7 {
    char *symbs;
    char *types;
    int len;
};

pattern7 *parse_pattern7(const char *s, io_status *err);
bool match_pattern7(const char *word, const pattern7 *pat);
void free_pattern7(pattern7 *pat);

struct pattern8_item {
    enum { CHAR, RANGE } type;
    char ch;
    char from, to;
    bool invert;
};

pattern8_item *parse_pattern8(const char *s, int *out_len, io_status *err);
bool match_pattern8(const char *word, const pattern8_item *items, int len);

struct pattern9_item {
    enum { CHAR, RANGE } type;
    char ch;
    char from, to;
    bool invert;
};

pattern9_item *parse_pattern9(const char *s, int *out_len, io_status *err);
bool match_pattern9(const char *word, const pattern9_item *items, int len);

#endif