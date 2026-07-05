#include "hash_index.h"
#include "list.h"
#include <cstring>

template<>
int hash_index<const char *>::get_hash(const char *const&key) const {
    if (!key) return 0;
    unsigned long long h = 0;
    for (const char *p = key; *p; ++p) {
        h = h * 131ULL + static_cast<unsigned char>(*p);
    }
    return (size > 0) ? (int) (h % (unsigned long long) size) : 0;
}

template<>
int hash_index<int>::get_hash(const int &key) const {
    unsigned long long h = 0;
    int n = key < 0 ? -key : key;
    do {
        h = h * 131ULL + (n % 10);
        n /= 10;
    } while (n > 0);
    return (size > 0) ? (int) (h % (unsigned long long) size) : 0;
}

template<>
void hash_index<const char *>::insert(list_node *rec) {
    int h = get_hash(rec->get_name());
    rec->name_hash_next = buckets[h];
    buckets[h] = rec;
}

template<>
void hash_index<const char *>::remove(list_node *rec) {
    int h = get_hash(rec->get_name());
    list_node *cur = buckets[h];
    list_node *prev = nullptr;

    while (cur) {
        if (cur == rec) {
            if (prev) prev->name_hash_next = cur->name_hash_next;
            else buckets[h] = cur->name_hash_next;
            return;
        }
        prev = cur;
        cur = cur->name_hash_next;
    }
}

template<>
list_node *hash_index<const char *>::get_bucket_head(const char *const&key) const {
    int h = get_hash(key);
    return buckets[h];
}

template<>
void hash_index<int>::insert(list_node *rec) {
    int h = get_hash(rec->get_phone());
    rec->phone_hash_next = buckets[h];
    buckets[h] = rec;
}

template<>
void hash_index<int>::remove(list_node *rec) {
    int h = get_hash(rec->get_phone());
    list_node *cur = buckets[h];
    list_node *prev = nullptr;

    while (cur) {
        if (cur == rec) {
            if (prev) prev->phone_hash_next = cur->phone_hash_next;
            else buckets[h] = cur->phone_hash_next;
            return;
        }
        prev = cur;
        cur = cur->phone_hash_next;
    }
}

template<>
list_node *hash_index<int>::get_bucket_head(const int &key) const {
    int h = get_hash(key);
    return buckets[h];
}

template<typename Key>
hash_index<Key>::hash_index(int k) : size(k > 0 ? k : 1) {
    buckets = new list_node *[size]();
}

template<typename Key>
hash_index<Key>::~hash_index() {
    delete[] buckets;
}

template class hash_index<const char *>;
template class hash_index<int>;
