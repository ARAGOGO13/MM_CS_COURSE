#include "query_classes.h"

b_tree_query::b_tree_query(const char *delim, int m) : words(delim), tree(m) {}

io_status b_tree_query::build(const char *s) {
    parse_result res = parse(s);
    if (res == parse_result::buffer_overflow) return io_status::memory;
    if (res == parse_result::empty) return io_status::success;

    for (int i = 0; i < get_count(); ++i) {
        word w;
        w.set(get_word(i));
        io_status st = tree.add_value(w);
        if (st != io_status::success) return st;
    }
    return io_status::success;
}

bool b_tree_query::apply(const char *line) {
    if (parse(line) != parse_result::success) return false;
    for (int i = 0; i < get_count(); ++i) {
        if (tree.contains(get_word(i)))
            return true;
    }
    return false;
}

rb_tree_query::rb_tree_query(const char *delim) : words(delim) {}

io_status rb_tree_query::build(const char *s) {
    parse_result res = parse(s);
    if (res == parse_result::buffer_overflow) return io_status::memory;
    if (res == parse_result::empty) return io_status::success;

    for (int i = 0; i < get_count(); ++i) {
        word w;
        w.set(get_word(i));
        tree.insert(std::move(w));
    }
    return io_status::success;
}

bool rb_tree_query::apply(const char *line) {
    if (parse(line) != parse_result::success) return false;
    for (int i = 0; i < get_count(); ++i) {
        if (!tree.contains(get_word(i)))
            return false;
    }
    return get_count() > 0;
}

avl_tree_query::avl_tree_query(const char *delim) : words(delim) {
}

io_status avl_tree_query::build(const char *s, const char *x) {
    parse_result res = parse(s);
    if (res == parse_result::buffer_overflow) return io_status::memory;
    if (res == parse_result::empty) return io_status::success;

    words xwords(spaces);
    res = xwords.parse(x ? x : "");
    if (res == parse_result::buffer_overflow) return io_status::memory;

    for (int i = 0; i < get_count(); ++i) {
        pair_string p;
        p.set_key(get_word(i));
        if (i < xwords.get_count())
            p.set_value(xwords.get_word(i));
        else
            p.set_value("");
        tree.insert(std::move(p));
    }
    return io_status::success;
}

bool avl_tree_query::apply(const char *line, char *out_buf, size_t out_size) {
    const char *in_ptr = line;
    char *out_ptr = out_buf;
    const char * const out_end = out_buf + out_size - 1;

    while (*in_ptr && out_ptr < out_end) {
        while (*in_ptr && strchr(spaces, *in_ptr) && out_ptr < out_end) {
            *out_ptr++ = *in_ptr++;
        }
        if (!*in_ptr) break;

        const char *word_start = in_ptr;
        while (*in_ptr && !strchr(spaces, *in_ptr)) {
            in_ptr++;
        }
        char word_buf[4096];
        size_t word_len = in_ptr - word_start;
        if (word_len >= sizeof(word_buf)) return false;
        strncpy(word_buf, word_start, word_len);
        word_buf[word_len] = '\0';

        const pair_string *found = tree.find(word_buf);
        const char *replacement = (found && found->value) ? found->value.get() : word_buf;

        size_t repl_len = strlen(replacement);
        if (out_ptr + repl_len >= out_end) return false;
        strcpy(out_ptr, replacement);
        out_ptr += repl_len;
    }
    *out_ptr = '\0';
    return true;
}
