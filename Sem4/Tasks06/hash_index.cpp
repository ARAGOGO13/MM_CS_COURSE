#include "hash_index.h"
#include "list.h"
#include <cstring>

hash_index::hash_index(int k) : size(k) {
    buckets = new list_node *[size]();
}

hash_index::~hash_index() {
    delete[] buckets;
}

int hash_index::hash(const char *name) const {
    unsigned long long h = 0;
    while (*name) {
        h = h * 131ULL + static_cast<unsigned char>(*name);
        ++name;
    }
    return static_cast<int>(h % static_cast<unsigned long long>(size));
}

void hash_index::insert(list_node *rec) {
    int h = hash(rec->get_name());
    rec->hash_next = buckets[h];
    buckets[h] = rec;
}

void hash_index::remove(list_node *rec) {
    int h = hash(rec->get_name());
    list_node *cur = buckets[h];
    list_node *prev = nullptr;
    while (cur) {
        if (cur == rec) {
            if (prev) prev->hash_next = cur->hash_next;
            else buckets[h] = cur->hash_next;
            return;
        }
        prev = cur;
        cur = cur->hash_next;
    }
}
