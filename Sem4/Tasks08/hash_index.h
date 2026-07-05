#ifndef HASH_INDEX_H
#define HASH_INDEX_H

class list_node;

template <typename Key>
class hash_index {
 private:
  list_node** buckets;
  int size;
  bool is_group_hash;

 public:
  hash_index(int k, bool group = false);
  ~hash_index();

  int get_hash(const Key& key) const;
  void insert(list_node* rec);
  void remove(list_node* rec);
  list_node* get_bucket_head(const Key& key) const;
};

#endif