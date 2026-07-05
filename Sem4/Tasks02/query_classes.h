#ifndef QUERY_CLASSES_H
#define QUERY_CLASSES_H

#include "common_utils.h"
#include "b_tree.h"
#include "rb_tree.h"
#include "avl_tree.h"

class b_tree_query : public words {
private:
    b_tree<word> tree;

public:
    b_tree_query(const char *delim, int m);

    io_status build(const char *s);

    bool apply(const char *line);
};

class rb_tree_query : public words {
private:
    rb_tree<word> tree;

public:
    rb_tree_query(const char *delim);

    io_status build(const char *s);

    bool apply(const char *line);
};

class avl_tree_query : public words {
private:
    avl_tree<pair_string> tree;

public:
    avl_tree_query(const char *delim);

    io_status build(const char *s, const char *x);

    bool apply(const char *line, char *out_buf, size_t out_size);
};

#endif
