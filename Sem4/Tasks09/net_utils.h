#ifndef NET_UTILS_H
#define NET_UTILS_H

int writeToClient (int fd, const char *buf);
int readFromClient (int fd, char *buf, int buf_size);

#endif