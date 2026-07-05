#include "solve.h"
#include "query_classes.h"
#include "pattern_utils.h"

io_status task1(const char *fin, const char *fout, const char *s, const char *t, int m, int *r) {
    b_tree_query query(t, m);
    io_status st = query.build(s);
    if (st != io_status::success) return st;

    FILE *in = fopen(fin, "r");
    if (!in) return io_status::open;
    FILE *out = fopen(fout, "w");
    if (!out) {
        fclose(in);
        return io_status::open;
    }

    char line[4096];
    int count = 0;
    while (fgets(line, sizeof(line), in)) {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') line[len - 1] = '\0';
        if (query.apply(line)) {
            fprintf(out, "%s\n", line);
            count++;
        }
    }
    if (ferror(in)) {
        fclose(in);
        fclose(out);
        return io_status::format;
    }
    fclose(in);
    fclose(out);
    *r = count;
    return io_status::success;
}

io_status task2(const char *fin, const char *fout, const char *s, const char *t, int *r) {
    words s_words(t);
    parse_result pres = s_words.parse(s);
    if (pres == parse_result::buffer_overflow) return io_status::memory;
    int s_count = (pres == parse_result::empty) ? 0 : s_words.get_count();
    FILE *in = fopen(fin, "r");
    if (!in) return io_status::open;
    FILE *out = fopen(fout, "w");
    if (!out) {
        fclose(in);
        return io_status::open;
    }
    char line[4096];
    int count = 0;
    while (fgets(line, sizeof(line), in)) {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') line[len - 1] = '\0';
        words lw(t);
        parse_result pres2 = lw.parse(line);
        if (pres2 == parse_result::buffer_overflow) continue;
        if (pres2 == parse_result::empty) {
            fprintf(out, "%s\n", line);
            ++count;
            continue;
        }
        if (pres2 != parse_result::success) continue;
        bool all_ok = true;
        int wc = lw.get_count();
        for (int i = 0; i < wc && all_ok; ++i) {
            const char *w = lw.get_word(i);
            bool found = false;
            for (int j = 0; j < s_count; ++j) {
                const char *sw = s_words.get_word(j);
                if (strcmp(sw, w) == 0) {
                    found = true;
                    break;
                }
            }
            if (!found) all_ok = false;
        }
        if (all_ok) {
            fprintf(out, "%s\n", line);
            ++count;
        }
    }
    if (ferror(in)) {
        fclose(in);
        fclose(out);
        return io_status::format;
    }
    fclose(in);
    fclose(out);
    *r = count;
    return io_status::success;
}

io_status task3(const char *fin, const char *fout, const char *s, const char *t, const char *x, int *r) {
    avl_tree_query query(t);
    io_status st = query.build(s, x);
    if (st != io_status::success) return st;

    FILE *in = fopen(fin, "r");
    if (!in) return io_status::open;
    FILE *out = fopen(fout, "w");
    if (!out) {
        fclose(in);
        return io_status::open;
    }

    char line[4096];
    char out_line[4096];
    int count = 0;
    while (fgets(line, sizeof(line), in)) {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') line[len - 1] = '\0';

        words w(t);
        parse_result pres = w.parse(line);
        if (pres == parse_result::buffer_overflow) continue;
        if (pres == parse_result::empty) {
            fprintf(out, "%s\n", line);
            ++count;
            continue;
        }
        if (pres != parse_result::success) continue;

        if (query.apply(line, out_line, sizeof(out_line))) {
            fprintf(out, "%s\n", out_line);
            ++count;
        }
    }
    if (ferror(in)) {
        fclose(in);
        fclose(out);
        return io_status::format;
    }
    fclose(in);
    fclose(out);
    *r = count;
    return io_status::success;
}

io_status task4(const char *fin, const char *fout, const char *s, const char *t, const char *x, int *r) {
    words s_words(t);
    parse_result res = s_words.parse(s);
    if (res == parse_result::buffer_overflow) return io_status::memory;
    if (res == parse_result::empty) {
        *r = 0;
        return io_status::success;
    }
    int s_count = s_words.get_count();

    condition *conds = nullptr;
    int cond_count = 0;
    res = parse_conditions(x, t, &conds, &cond_count);
    if (res == parse_result::buffer_overflow) return io_status::format;
    if (res != parse_result::success) return io_status::memory;
    if (s_count != cond_count) {
        delete[] conds;
        return io_status::format;
    }

    FILE *in = fopen(fin, "r");
    if (!in) {
        delete[] conds;
        return io_status::open;
    }
    FILE *out = fopen(fout, "w");
    if (!out) {
        fclose(in);
        delete[] conds;
        return io_status::open;
    }

    char line[4096];
    int count = 0;
    while (fgets(line, sizeof(line), in)) {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') line[len - 1] = '\0';

        words line_words(t);
        if (line_words.parse(line) != parse_result::success) continue;

        bool found = false;
        for (int i = 0; i < line_words.get_count() && !found; ++i) {
            const char *w = line_words.get_word(i);
            for (int j = 0; j < s_count; ++j) {
                if (check_condition(w, s_words.get_word(j), conds[j].oper)) {
                    found = true;
                    break;
                }
            }
        }
        if (found) {
            fprintf(out, "%s\n", line);
            count++;
        }
    }
    if (ferror(in)) {
        fclose(in);
        fclose(out);
        delete[] conds;
        return io_status::format;
    }
    fclose(in);
    fclose(out);
    delete[] conds;
    *r = count;
    return io_status::success;
}

io_status task5(const char *fin, const char *fout, const char *s, const char *t, const char *x, int *r) {
    words s_words(t);
    parse_result res = s_words.parse(s);
    if (res == parse_result::buffer_overflow) return io_status::memory;
    if (res == parse_result::empty) {
        *r = 0;
        return io_status::success;
    }
    int s_count = s_words.get_count();
    condition *conds = nullptr;
    int cond_count = 0;
    res = parse_conditions(x, t, &conds, &cond_count);
    if (res == parse_result::buffer_overflow) return io_status::format;
    if (res != parse_result::success) return io_status::memory;
    if (s_count != cond_count) {
        delete[] conds;
        return io_status::format;
    }
    FILE *in = fopen(fin, "r");
    if (!in) {
        delete[] conds;
        return io_status::open;
    }
    FILE *out = fopen(fout, "w");
    if (!out) {
        fclose(in);
        delete[] conds;
        return io_status::open;
    }
    char line[4096];
    int count = 0;
    while (fgets(line, sizeof(line), in)) {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') line[len - 1] = '\0';
        words line_words(t);
        parse_result pres = line_words.parse(line);
        if (pres == parse_result::buffer_overflow) continue;
        if (pres == parse_result::empty) {
            fprintf(out, "%s\n", line);
            ++count;
            continue;
        }
        if (pres != parse_result::success) continue;
        bool all_ok = true;
        for (int i = 0; i < line_words.get_count() && all_ok; ++i) {
            const char *w = line_words.get_word(i);
            for (int j = 0; j < s_count; ++j) {
                if (!check_condition(w, s_words.get_word(j), conds[j].oper)) {
                    all_ok = false;
                    break;
                }
            }
        }
        if (all_ok) {
            fprintf(out, "%s\n", line);
            ++count;
        }
    }
    if (ferror(in)) {
        fclose(in);
        fclose(out);
        delete[] conds;
        return io_status::format;
    }
    fclose(in);
    fclose(out);
    delete[] conds;
    *r = count;
    return io_status::success;
}

io_status task6(const char *fin, const char *fout, const char *s, const char *t, int *r) {
    io_status err;
    pattern6 *pat = parse_pattern6(s, &err);
    if (!pat) return err;

    FILE *in = fopen(fin, "r");
    if (!in) {
        free_pattern6(pat);
        return io_status::open;
    }
    FILE *out = fopen(fout, "w");
    if (!out) {
        fclose(in);
        free_pattern6(pat);
        return io_status::open;
    }

    char line[4096];
    int count = 0;
    while (fgets(line, sizeof(line), in)) {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') line[len - 1] = '\0';

        words w(t);
        parse_result pres = w.parse(line);
        if (pres == parse_result::buffer_overflow) continue;
        if (pres != parse_result::success) continue;

        bool found = false;
        for (int i = 0; i < w.get_count(); ++i) {
            if (match_pattern6(w.get_word(i), pat)) {
                found = true;
                break;
            }
        }
        if (found) {
            fprintf(out, "%s\n", line);
            ++count;
        }
    }
    if (ferror(in)) {
        fclose(in);
        fclose(out);
        free_pattern6(pat);
        return io_status::format;
    }
    fclose(in);
    fclose(out);
    free_pattern6(pat);
    *r = count;
    return io_status::success;
}

io_status task7(const char *fin, const char *fout, const char *s, const char *t, int *r) {
    io_status err;
    pattern7 *pat = parse_pattern7(s, &err);
    if (!pat) return err;

    FILE *in = fopen(fin, "r");
    if (!in) {
        free_pattern7(pat);
        return io_status::open;
    }
    FILE *out = fopen(fout, "w");
    if (!out) {
        fclose(in);
        free_pattern7(pat);
        return io_status::open;
    }

    char line[4096];
    int count = 0;
    while (fgets(line, sizeof(line), in)) {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') line[len - 1] = '\0';

        words w(t);
        parse_result pres = w.parse(line);
        if (pres == parse_result::buffer_overflow) {
            continue;
        }
        bool found = false;

        int wc = w.get_count();
        if (wc == 0) {
            if (match_pattern7("", pat)) {
                found = true;
            }
        } else {
            for (int i = 0; i < wc; ++i) {
                if (match_pattern7(w.get_word(i), pat)) {
                    found = true;
                    break;
                }
            }
        }

        if (found) {
            fprintf(out, "%s\n", line);
            ++count;
        }
    }
    if (ferror(in)) {
        fclose(in);
        fclose(out);
        free_pattern7(pat);
        return io_status::format;
    }
    fclose(in);
    fclose(out);
    free_pattern7(pat);
    *r = count;
    return io_status::success;
}

io_status task8(const char *fin, const char *fout, const char *s, const char *t, int *r) {
    io_status err;
    int pat_len;
    pattern8_item *items = parse_pattern8(s, &pat_len, &err);
    if (!items) return err;

    FILE *in = fopen(fin, "r");
    if (!in) {
        delete[] items;
        return io_status::open;
    }
    FILE *out = fopen(fout, "w");
    if (!out) {
        fclose(in);
        delete[] items;
        return io_status::open;
    }

    char line[4096];
    int count = 0;
    while (fgets(line, sizeof(line), in)) {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') line[len - 1] = '\0';

        words w(t);
        if (w.parse(line) != parse_result::success) continue;

        bool found = false;
        for (int i = 0; i < w.get_count(); ++i) {
            if (match_pattern8(w.get_word(i), items, pat_len)) {
                found = true;
                break;
            }
        }
        if (found) {
            fprintf(out, "%s\n", line);
            count++;
        }
    }
    if (ferror(in)) {
        fclose(in);
        fclose(out);
        delete[] items;
        return io_status::format;
    }
    fclose(in);
    fclose(out);
    delete[] items;
    *r = count;
    return io_status::success;
}

io_status task9(const char *fin, const char *fout, const char *s, const char *t, int *r) {
    io_status err;
    int pat_len;
    pattern9_item *items = parse_pattern9(s, &pat_len, &err);
    if (!items) return err;

    FILE *in = fopen(fin, "r");
    if (!in) {
        delete[] items;
        return io_status::open;
    }
    FILE *out = fopen(fout, "w");
    if (!out) {
        fclose(in);
        delete[] items;
        return io_status::open;
    }

    char line[4096];
    int count = 0;
    while (fgets(line, sizeof(line), in)) {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') line[len - 1] = '\0';

        words w(t);
        if (w.parse(line) != parse_result::success) continue;

        bool found = false;
        for (int i = 0; i < w.get_count(); ++i) {
            if (match_pattern9(w.get_word(i), items, pat_len)) {
                found = true;
                break;
            }
        }
        if (found) {
            fprintf(out, "%s\n", line);
            count++;
        }
    }

    if (ferror(in)) {
        fclose(in);
        fclose(out);
        delete[] items;
        return io_status::format;
    }
    fclose(in);
    fclose(out);
    delete[] items;
    *r = count;
    return io_status::success;
}
