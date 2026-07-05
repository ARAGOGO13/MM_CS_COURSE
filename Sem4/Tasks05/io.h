#ifndef IO_H
#define IO_H

#define LEN 1234

enum class io_status {
    success,
    eof,
    format,
    memory,
    open,
    create
};

#endif