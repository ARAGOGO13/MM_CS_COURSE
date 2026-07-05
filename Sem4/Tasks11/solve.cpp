#include "solve.h"
#include <cstdio>
#include <cstring>
#include <map>
#include <set>
#include <string>

#define LEN 1234

io_status task01(const char *a, const char *b, int &r) {
  FILE *fr = fopen(a, "r");
  if (!fr)
    return io_status::open;

  FILE *fw = fopen(b, "w");
  if (!fw) {
    fclose(fr);
    return io_status::open;
  }

  std::set<std::string> seen;
  char buf[LEN];
  r = 0;

  while (fgets(buf, LEN, fr)) {
    size_t len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n')
      buf[len - 1] = '\0';
    std::string s(buf);
    if (seen.find(s) == seen.end()) {
      seen.insert(s);
      fprintf(fw, "%s\n", s.c_str());
      ++r;
    }
  }

  fclose(fr);
  fclose(fw);
  return io_status::success;
}

io_status task02(const char *a, const char *b, int &r) {
  FILE *fr = fopen(a, "r");
  if (!fr)
    return io_status::open;

  FILE *fw = fopen(b, "w");
  if (!fw) {
    fclose(fr);
    return io_status::open;
  }

  std::map<std::string, int> seen;
  char buf[LEN];
  r = 0;

  while (fgets(buf, LEN, fr)) {
    size_t len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n')
      buf[len - 1] = '\0';
    std::string s(buf);
    if (seen.find(s) == seen.end()) {
      seen.insert(std::make_pair(s, 0));
      fprintf(fw, "%s\n", s.c_str());
      ++r;
    }
  }

  fclose(fr);
  fclose(fw);
  return io_status::success;
}

io_status task03(const char *a, const char *b, int &r) {
  FILE *fr = fopen(a, "r");
  if (!fr)
    return io_status::open;

  FILE *fw = fopen(b, "w");
  if (!fw) {
    fclose(fr);
    return io_status::open;
  }

  std::multiset<std::string> ms;
  char buf[LEN];
  int line_no = 0;
  r = 0;

  while (fgets(buf, LEN, fr)) {
    size_t len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n')
      buf[len - 1] = '\0';
    std::string s(buf);
    ++line_no;
    bool is_new = (ms.find(s) == ms.end());
    ms.insert(s);
    auto range = ms.equal_range(s);
    int cnt = static_cast<int>(std::distance(range.first, range.second));
    if (is_new)
      ++r;
    fprintf(fw, "%d %d %s\n", line_no, cnt, s.c_str());
  }

  fclose(fr);
  fclose(fw);
  return io_status::success;
}

io_status task04(const char *a, const char *b, int &r) {
  FILE *fr = fopen(a, "r");
  if (!fr)
    return io_status::open;

  FILE *fw = fopen(b, "w");
  if (!fw) {
    fclose(fr);
    return io_status::open;
  }

  std::map<std::string, int> counts;
  char buf[LEN];
  int line_no = 0;
  r = 0;

  while (fgets(buf, LEN, fr)) {
    size_t len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n')
      buf[len - 1] = '\0';
    std::string s(buf);
    ++line_no;
    auto it = counts.find(s);
    if (it == counts.end()) {
      counts.insert(std::make_pair(s, 1));
      ++r;
      fprintf(fw, "%d %d %s\n", line_no, 1, s.c_str());
    } else {
      it->second += 1;
      fprintf(fw, "%d %d %s\n", line_no, it->second, s.c_str());
    }
  }

  fclose(fr);
  fclose(fw);
  return io_status::success;
}

io_status task05(const char *a, const char *b, const char *c, std::string &t,
                 int &r) {
  FILE *fr = fopen(a, "r");
  if (!fr)
    return io_status::open;

  std::set<std::string> words;
  char buf[LEN];

  while (fgets(buf, LEN, fr)) {
    size_t len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n')
      buf[len - 1] = '\0';
    char *s = strtok(buf, t.c_str());
    while (s) {
      words.insert(std::string(s));
      s = strtok(nullptr, t.c_str());
    }
  }
  fclose(fr);

  fr = fopen(b, "r");
  if (!fr)
    return io_status::open;

  FILE *fw = fopen(c, "w");
  if (!fw) {
    fclose(fr);
    return io_status::open;
  }

  r = 0;
  char bufcp[LEN];

  while (fgets(buf, LEN, fr)) {
    size_t len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n')
      buf[len - 1] = '\0';
    strcpy(bufcp, buf);
    int count = 0;
    char *s = strtok(buf, t.c_str());
    while (s) {
      if (words.find(std::string(s)) != words.end())
        ++count;
      s = strtok(nullptr, t.c_str());
    }
    r += count;
    fprintf(fw, "%d %s\n", count, bufcp);
  }

  fclose(fr);
  fclose(fw);
  return io_status::success;
}

io_status task06(const char *a, const char *b, const char *c, std::string &t,
                 int &r) {
  FILE *fr = fopen(a, "r");
  if (!fr)
    return io_status::open;

  std::map<std::string, int> words;
  char buf[LEN];

  while (fgets(buf, LEN, fr)) {
    size_t len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n')
      buf[len - 1] = '\0';
    char *s = strtok(buf, t.c_str());
    while (s) {
      words.insert(std::make_pair(std::string(s), 0));
      s = strtok(nullptr, t.c_str());
    }
  }
  fclose(fr);

  fr = fopen(b, "r");
  if (!fr)
    return io_status::open;

  FILE *fw = fopen(c, "w");
  if (!fw) {
    fclose(fr);
    return io_status::open;
  }

  r = 0;
  char bufcp[LEN];

  while (fgets(buf, LEN, fr)) {
    size_t len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n')
      buf[len - 1] = '\0';
    strcpy(bufcp, buf);
    int count = 0;
    char *s = strtok(buf, t.c_str());
    while (s) {
      if (words.find(std::string(s)) != words.end())
        ++count;
      s = strtok(nullptr, t.c_str());
    }
    r += count;
    fprintf(fw, "%d %s\n", count, bufcp);
  }

  fclose(fr);
  fclose(fw);
  return io_status::success;
}