#include "solve.h"
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <list>
#include <string>
#include <utility>
#include <vector>

#define LEN 1234

io_status task01(const char *a, const char *b, int &r) {
  FILE *fr = fopen(a, "r");
  if (!fr)
    return io_status::open;
  std::vector<std::pair<std::string, int>> data;
  char buf[LEN];
  int idx = 0;
  while (fgets(buf, LEN, fr)) {
    size_t len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n')
      buf[len - 1] = '\0';
    data.emplace_back(buf, idx++);
  }
  fclose(fr);

  std::sort(data.begin(), data.end(),
            [](const auto &a, const auto &b) { return a.first < b.first; });

  std::vector<std::pair<std::string, int>> uniq;
  for (size_t i = 0; i < data.size();) {
    size_t j = i;
    int min_idx = data[i].second;
    while (j < data.size() && data[j].first == data[i].first) {
      if (data[j].second < min_idx)
        min_idx = data[j].second;
      ++j;
    }
    uniq.emplace_back(data[i].first, min_idx);
    i = j;
  }

  std::sort(uniq.begin(), uniq.end(),
            [](const auto &a, const auto &b) { return a.second < b.second; });

  FILE *fw = fopen(b, "w");
  if (!fw)
    return io_status::open;
  for (const auto &p : uniq)
    fprintf(fw, "%s\n", p.first.c_str());
  fclose(fw);

  r = static_cast<int>(uniq.size());
  return io_status::success;
}

io_status task02(const char *a, const char *b, int &r) {
  FILE *fr = fopen(a, "r");
  if (!fr)
    return io_status::open;
  std::list<std::pair<std::string, int>> data;
  char buf[LEN];
  int idx = 0;
  while (fgets(buf, LEN, fr)) {
    size_t len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n')
      buf[len - 1] = '\0';
    data.emplace_back(buf, idx++);
  }
  fclose(fr);

  data.sort([](const auto &a, const auto &b) { return a.first < b.first; });
  data.unique([](const auto &a, const auto &b) { return a.first == b.first; });

  data.sort([](const auto &a, const auto &b) { return a.second < b.second; });

  FILE *fw = fopen(b, "w");
  if (!fw)
    return io_status::open;
  for (const auto &p : data)
    fprintf(fw, "%s\n", p.first.c_str());
  fclose(fw);

  r = static_cast<int>(data.size());
  return io_status::success;
}

io_status task03(const char *a, const char *b, int &r) {
  FILE *fr = fopen(a, "r");
  if (!fr)
    return io_status::open;
  std::vector<std::pair<std::string, std::pair<int, int>>> data;
  char buf[LEN];
  int line_no = 1;
  while (fgets(buf, LEN, fr)) {
    size_t len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n')
      buf[len - 1] = '\0';
    data.emplace_back(buf, std::make_pair(line_no++, 1));
  }
  fclose(fr);

  if (data.empty()) {
    r = 0;
    FILE *fw = fopen(b, "w");
    fclose(fw);
    return io_status::success;
  }

  std::sort(data.begin(), data.end(),
            [](const auto &a, const auto &b) { return a.first < b.first; });

  for (size_t i = 0; i < data.size();) {
    size_t j = i;
    while (j < data.size() && data[j].first == data[i].first)
      ++j;
    int cnt = static_cast<int>(j - i);
    for (size_t k = i; k < j; ++k)
      data[k].second.second = cnt;
    i = j;
  }

  r = 0;
  for (auto it = data.begin(); it != data.end();) {
    auto start = it;
    while (it != data.end() && it->first == start->first)
      ++it;
    ++r;
  }

  std::sort(data.begin(), data.end(), [](const auto &a, const auto &b) {
    return a.second.first < b.second.first;
  });

  FILE *fw = fopen(b, "w");
  if (!fw)
    return io_status::open;
  for (const auto &p : data)
    fprintf(fw, "%d %d %s\n", p.second.first, p.second.second, p.first.c_str());
  fclose(fw);

  return io_status::success;
}

io_status task04(const char *a, const char *b, int &r) {
  FILE *fr = fopen(a, "r");
  if (!fr)
    return io_status::open;
  std::list<std::pair<std::string, std::pair<int, int>>> data;
  char buf[LEN];
  int line_no = 1;
  while (fgets(buf, LEN, fr)) {
    size_t len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n')
      buf[len - 1] = '\0';
    data.emplace_back(buf, std::make_pair(line_no++, 1));
  }
  fclose(fr);

  if (data.empty()) {
    r = 0;
    FILE *fw = fopen(b, "w");
    fclose(fw);
    return io_status::success;
  }

  data.sort([](const auto &a, const auto &b) { return a.first < b.first; });

  for (auto it = data.begin(); it != data.end();) {
    auto start = it;
    int cnt = 0;
    std::string cur = it->first;
    while (it != data.end() && it->first == cur) {
      ++cnt;
      ++it;
    }
    for (auto p = start; p != it; ++p)
      p->second.second = cnt;
  }

  r = 0;
  for (auto it = data.begin(); it != data.end();) {
    auto start = it;
    while (it != data.end() && it->first == start->first)
      ++it;
    ++r;
  }

  data.sort([](const auto &a, const auto &b) {
    return a.second.first < b.second.first;
  });

  FILE *fw = fopen(b, "w");
  if (!fw)
    return io_status::open;
  for (const auto &p : data)
    fprintf(fw, "%d %d %s\n", p.second.first, p.second.second, p.first.c_str());
  fclose(fw);

  return io_status::success;
}

io_status task05(const char *a, const char *b, const char *c, std::string &t,
                 int &r) {
  FILE *fr = fopen(a, "r");
  if (!fr)
    return io_status::open;
  std::vector<std::string> words;
  char buf[LEN];
  while (fgets(buf, LEN, fr)) {
    size_t len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n')
      buf[len - 1] = '\0';
    char *s = strtok(buf, t.c_str());
    while (s) {
      words.emplace_back(s);
      s = strtok(nullptr, t.c_str());
    }
  }
  fclose(fr);

  std::sort(words.begin(), words.end());
  words.erase(std::unique(words.begin(), words.end()), words.end());

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
      if (std::binary_search(words.begin(), words.end(), s))
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
  std::list<std::string> words;
  char buf[LEN];
  while (fgets(buf, LEN, fr)) {
    size_t len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n')
      buf[len - 1] = '\0';
    char *s = strtok(buf, t.c_str());
    while (s) {
      words.emplace_back(s);
      s = strtok(nullptr, t.c_str());
    }
  }
  fclose(fr);

  words.sort();
  words.unique();

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
      if (std::find(words.begin(), words.end(), s) != words.end())
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