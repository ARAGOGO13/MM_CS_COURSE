#include "io_status.h"

#define LEN (1234)

io_status processing_s (const char *s, char *s1 ,char *s2, char ss, int flag);
io_status processing_s_7_9 (const char *s, char *s1 ,char *s2, char ss);
io_status processing_s_10 (const char *s, char *s1, char *s2);
int words_div (const char *buff, const char *t, int *words_index);
io_status task01 (const char *filename_a, const char *filename_b, const char *s, int *r);
io_status task02 (const char *filename_a, const char *filename_b, const char *s, int *r);
io_status task03 (const char *filename_a, const char *filename_b, const char *s, const char *t, int *r);
io_status task04 (const char *filename_a, const char *filename_b, const char *s, const char *t, int *r);
io_status task05 (const char *filename_a, const char *filename_b, const char *s, const char *t, int *r);
io_status task06 (const char *filename_a, const char *filename_b, const char *s, int *r);
io_status task07 (const char *filename_a, const char *filename_b, const char *s, int *r);
io_status task08 (const char *filename_a, const char *filename_b, const char *s, int *r);
io_status task09 (const char *filename_a, const char *filename_b, const char *s, int *r);
io_status task10 (const char *filename_a, const char *filename_b, const char *s, int *r);
