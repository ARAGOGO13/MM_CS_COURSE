#include "command.h"
#include <string.h>
#include <stdlib.h>

static void skip_spaces(const char *&p) {
    while (*p && is_space((unsigned char) *p)) ++p;
}

bool command::parse(const char *str) {
    c_name = c_phone = c_group = condition::none;
    init(nullptr, 0, 0);

    if (!str) return false;
    const char *p = str;
    skip_spaces(p);
    if (!*p) return false;

    char field[64] = {0};
    int fi = 0;
    while (*p && !is_space((unsigned char) *p) && fi + 1 < (int) sizeof(field)) {
        field[fi++] = *p++;
    }
    field[fi] = '\0';
    skip_spaces(p);
    if (!*p) return false;

    char opbuf[16] = {0};
    int oi = 0;
    if (strncmp(p, "like", 4) == 0 && (p[4] == '\0' || is_space((unsigned char) p[4]))) {
        strcpy(opbuf, "like");
        p += 4;
    } else {
        if (*p == '<' || *p == '>' || *p == '=') {
            opbuf[oi++] = *p++;
            if ((*p == '=' || (opbuf[0] == '<' && *p == '>')) && oi + 1 < (int) sizeof(opbuf)) {
                opbuf[oi++] = *p++;
            }
            opbuf[oi] = '\0';
        } else {
            return false;
        }
    }
    skip_spaces(p);
    if (!*p) return false;

    char rhs[LEN];
    const char *q = p;
    while (*q) ++q;
    while (q > p && is_space((unsigned char) *(q - 1))) --q;
    int len = (int) (q - p);
    if (len >= (int) sizeof(rhs)) return false;
    strncpy(rhs, p, len);
    rhs[len] = '\0';

    if (strcmp(field, "name") == 0) {
        if (strcmp(opbuf, "like") != 0 && strcmp(opbuf, "=") != 0 && strcmp(opbuf, "<>") != 0
            && strcmp(opbuf, "<") != 0 && strcmp(opbuf, ">") != 0 && strcmp(opbuf, "<=") != 0 &&
            strcmp(opbuf, ">=") != 0) {
            return false;
        }
        if (init(rhs, 0, 0)) return false;
        if (strcmp(opbuf, "like") == 0) c_name = condition::like;
        else if (strcmp(opbuf, "=") == 0) c_name = condition::eq;
        else if (strcmp(opbuf, "<>") == 0) c_name = condition::ne;
        else if (strcmp(opbuf, "<") == 0) c_name = condition::lt;
        else if (strcmp(opbuf, ">") == 0) c_name = condition::gt;
        else if (strcmp(opbuf, "<=") == 0) c_name = condition::le;
        else if (strcmp(opbuf, ">=") == 0) c_name = condition::ge;
    } else if (strcmp(field, "phone") == 0) {
        int val = atoi(rhs);
        init(nullptr, val, 0);
        if (strcmp(opbuf, "=") == 0) c_phone = condition::eq;
        else if (strcmp(opbuf, "<>") == 0) c_phone = condition::ne;
        else if (strcmp(opbuf, "<") == 0) c_phone = condition::lt;
        else if (strcmp(opbuf, ">") == 0) c_phone = condition::gt;
        else if (strcmp(opbuf, "<=") == 0) c_phone = condition::le;
        else if (strcmp(opbuf, ">=") == 0) c_phone = condition::ge;
        else return false;
    } else if (strcmp(field, "group") == 0) {
        int val = atoi(rhs);
        init(nullptr, 0, val);
        if (strcmp(opbuf, "=") == 0) c_group = condition::eq;
        else if (strcmp(opbuf, "<>") == 0) c_group = condition::ne;
        else if (strcmp(opbuf, "<") == 0) c_group = condition::lt;
        else if (strcmp(opbuf, ">") == 0) c_group = condition::gt;
        else if (strcmp(opbuf, "<=") == 0) c_group = condition::le;
        else if (strcmp(opbuf, ">=") == 0) c_group = condition::ge;
        else return false;
    } else {
        return false;
    }

    return true;
}


void command::print(FILE *fp) const {
    fprintf(fp, "CMD: name_cond=%d phone_cond=%d group_cond=%d ; name='%s' phone=%d group=%d\n",
            (int) c_name, (int) c_phone, (int) c_group,
            get_name(), get_phone(), get_group());
}

bool command::apply(const record &x) const {
    if (!x.compare_name(c_name, *this)) return false;
    if (!x.compare_phone(c_phone, *this)) return false;
    if (!x.compare_group(c_group, *this)) return false;
    return true;
}