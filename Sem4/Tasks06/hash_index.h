#ifndef HASH_INDEX_H
#define HASH_INDEX_H

class list_node;

class hash_index {
private:
    list_node **buckets;
    int size;

public:
    hash_index(int k);

    ~hash_index();

    int hash(const char *name) const;

    void insert(list_node *rec);

    void remove(list_node *rec);

    list_node *get_bucket_head(const char *name) const { return buckets[hash(name)]; }
    int get_size() const { return size; }
};

#endif
