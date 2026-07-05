#include "net_utils.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int write_all(int fd, const void *buf, int count) {
  const char *p = (const char *)buf;
  int remaining = count;
  while (remaining > 0) {
    int n = (int)write(fd, p, (size_t)remaining);
    if (n < 0) {
      if (errno == EINTR) continue;
      perror("write_all");
      return -1;
    }
    if (n == 0) {
      fprintf(stderr, "write_all: connection closed\n");
      return -1;
    }
    p += n;
    remaining -= n;
  }
  return 0;
}

int read_all(int fd, void *buf, int count) {
  char *p = (char *)buf;
  int remaining = count;
  while (remaining > 0) {
    int n = (int)read(fd, p, (size_t)remaining);
    if (n < 0) {
      if (errno == EINTR) continue;
      perror("read_all");
      return -1;
    }
    if (n == 0) return -1;
    p += n;
    remaining -= n;
  }
  return 0;
}

int send_string(int fd, const char *s, int len) {
  if (write_all(fd, &len, sizeof(int)) != 0) return -1;
  if (len > 0 && write_all(fd, s, len) != 0) return -1;
  return 0;
}

int recv_string(int fd, char *buf, int buf_size) {
  int len = 0;
  if (read_all(fd, &len, sizeof(int)) != 0) return -1;
  if (len < 0 || len >= buf_size) {
    fprintf(stderr, "recv_string: bad length %d\n", len);
    return -1;
  }
  if (len > 0 && read_all(fd, buf, len) != 0) return -1;
  buf[len] = '\0';
  return len;
}

int writeToClient(int fd, const char *buf) {
  return send_string(fd, buf ? buf : "", buf ? (int)strlen(buf) : 0);
}

int readFromClient(int fd, char *buf, int buf_size) {
  return recv_string(fd, buf, buf_size);
}
