#ifndef _SSTL_HASHTABLE_H
#define _SSTL_HASHTABLE_H

#include <algorithm>

#include "../../src/sstl_allocator.hpp"
#include "../../src/sstl_iterator.hpp"
#include "../../src/sstl_vector.hpp"

namespace sup {

// declaration
template <class Key, class Value, class HashFunc, 
          class ExtractKey, class EqualKey, class Alloc=alloc>
class hashtable;

template <class Value>
struct __hashtable_node {
  __hashtable_node *next;
  Value val;
};

template <class Key, class Value, class HashFunc, 
          class ExtractKey, class EqualKey, class Alloc>
struct __hashtable_iterator {
  typedef hashtable<Key, Value, HashFunc, ExtractKey, EqualKey, Alloc>
    _hashtable;
  typedef __hashtable_iterator<Key, Value, HashFunc, ExtractKey, EqualKey, Alloc>
    iterator;
  typedef __hashtable_iterator<Key, const Value, HashFunc, ExtractKey, EqualKey, Alloc>
    const_iterator;
  typedef __hashtable_node<Value> node; 

  typedef forward_iterator_tag iterator_category; 
  typedef Value value_type;
  typedef ptrdiff_t difference_type;
  typedef size_t size_type;
  typedef Value* pointer;
  typedef Value& reference; 

  node* cur;
  _hashtable* table; 

  __hashtable_iterator(node* n, _hashtable* tb): cur(n), table(tb) {}
  __hashtable_iterator() {}

  reference operator*() const { return cur->val; }
  pointer operator->() const { return &(operator*());}
  iterator& operator++() {
    node* pre = cur;
    cur = cur->next;
    if (cur == nullptr) {
      size_type bucket = table->bkt_num(pre->val);
      while (cur == nullptr && ++bucket < table->buckets.size()) {
        cur = table->buckets[bucket];
      }
    }
    return *this;
  }
  iterator operator++(int) {
    iterator tmp = *this
    ++(*this);
    return tmp;
  }

  bool operator==(const iterator& it) { return cur == it.cur; }
  bool operator!=(const iterator& it) { return cur != it.cur; }
};

enum { sstl_num_of_primes = 29 };

template<class __PrimeType>
struct __hashtable_prime_list {
  static const __PrimeType __sstl_prime_list[sstl_num_of_primes];
  static const __PrimeType* __get_prime_list() {
    return __sstl_prime_list;
  }
};

template<class __PrimeType>
const __PrimeType __hashtable_prime_list<__PrimeType>::__sstl_prime_list[sstl_num_of_primes] = {
  5ul,          53ul,         97ul,         193ul,       389ul,
  769ul,        1543ul,       3079ul,       6151ul,      12289ul,
  24593ul,      49157ul,      98317ul,      196613ul,    393241ul,
  786433ul,     1572869ul,    3145739ul,    6291469ul,   12582917ul,
  25165843ul,   50331653ul,   100663319ul,  201326611ul, 402653189ul,
  805306457ul,  1610612741ul, 3221225473ul, 4294967291ul
};

inline unsigned long __sstl_next_prime(unsigned long num) {
  const unsigned long *first = __hashtable_prime_list<unsigned long>::__get_prime_list();
  const unsigned long *last = first + sstl_num_of_primes;
  const unsigned long *position = std::upper_bound(first, last, num);

  return (position == last) ? *(position - 1) : *position;
}

template <class Key, class Value, class HashFunc, 
          class ExtractKey, class EqualKey, class Alloc>
class hashtable {
public: 
  typedef Key key_type;
  typedef HashFunc hasher;
  typedef EqualKey key_equal;
  typedef size_t size_type;
  typedef Value value_type;
  typedef __hashtable_iterator<Key, Value, HashFunc, ExtractKey, EqualKey, Alloc> iterator;

private:
  hasher hash;
  key_equal equals;
  ExtractKey get_key;

  typedef __hashtable_node<Value> node;
  typedef simple_alloc<node, Alloc> node_allocator;
  typedef hashtable<Key, Value, HashFunc, ExtractKey, EqualKey, Alloc> _self;
  typedef sup::vector<node*, Alloc> bucket_type;
  sup::vector<node*, Alloc> buckets;
  size_type num_of_elements;
  float max_load_factor;

public:
  /*************** De-constructors ***************/
  hashtable(size_type n, const HashFunc hfc, const EqualKey eq_k)
    :hash(hfc), equals(eq_k), get_key(ExtractKey()), num_of_elements(0), load_factor(1.0) {
    initialize_buckets(n);
  }
  ~hashtable() { clear(); }
  
  /*************** Accessors ***************/
  size_type size() const { return num_of_elements; }
  bool empty() const { return num_of_elements == 0; }
  size_type count(const key_type& key) {
    std::pair<iterator, iterator> tmp_range = equal_range(key);
    size_type num = 0;
    while (tmp_range.first != tmp_range.second) {
      ++tmp_range.first;
      ++num;
    }
    return num;
  }
  size_type bucket_count() const { return buckets.size(); }
  size_type max_bucket_count() const 
    { return __hashtable_prime_list<unsigned long>::__get_prime_list()[sstl_num_of_primes-1]; }

  // iterators
  iterator begin() { 
    if (num_of_elements == 0)
      return end();
    
    for (size_type i = 0; i < buckets.size(); ++i) {
      if (buckets[i] != nullptr)
        return iterator(buckets[i], this);
    }

    return end();
  }
  iterator end() { return iterator(nullptr, this); }
  iterator find(const key_type& k) {
    size_type bucket = bkt_num_key(k);
    node* cur = buckets[bucket];

    while (cur != nullptr && !equals(k, get_key(cur->val))) {
      cur = cur->next;
    }

    return iterator(cur, this);
  }
  std::pair<iterator, iterator> equal_range(const key_type& key) {
    iterator first = find(key);
    if (first == end())
      return std::pair<iterator, iterator>(first, first);
    iterator last = first;
    ++last;
    while (last != end() && get_key(*last) == key) {
      ++last;
    }

    return std::pair<iterator, iterator>(first, last);
  }

  // operator [] overloading
  value_type& operator[] (key_type key) {
    size_type bucket = bkt_num_key(key);
    node* cur = buckets[bucket];
    if (cur != nullptr) { // there are other elements
      while (cur != nullptr) {
        if (equals(get_key(cur->val), key)) // there exists the exact key
          return cur->val;
        cur = cur->next;
      }
    }
    // there are no other elements or 
    // there are no elements of the same key
    resize(num_of_elements + 1); 
    bucket = bkt_num_key(key);
    cur = new_node(value_type());
    cur->next = buckets[bucket];
    buckets[bucket] = cur;
    ++num_of_elements;
    return cur->val;
  }
  
  // traits
  hasher hash_function() const { return hasher(); }
  key_equal key_eq() const { return key_equal(); }
  node_allocator get_allocator() const { return node_allocator(); }

  /*************** Modifiers ***************/
  iterator insert_equal(const value_type& val);
  template<class InputIterator>
  void insert_equal(InputIterator first, InputIterator last);

  std::pair<iterator, bool> insert_unique(const value_type& val);
  template<class InputIterator>
  void insert_unique(InputIterator first, InputIterator last);

  size_type erase(const key_type& key);
  void erase(iterator& position);
  void erase(iterator first, iterator last);

  void clear();

  // getter and setter for load factors
  float load_factor() const { return (float) num_of_elements/buckets.size(); }
  float max_load_factor() const { return max_load_factor; }
  void max_load_factor(float max_factor) { max_load_factor = max_factor; }

  // modifiers for space
  void rehash(size_type n);
  void reserve(size_type n);

  // assignment operator overloading
  hashtable<Key, Value, HashFunc, ExtractKey, EqualKey, Alloc>&
    operator= (hashtable<Key, Value, HashFunc, ExtractKey, EqualKey, Alloc>& table);


private:
  // helper functions
  // create & delete node
  node* new_node(const value_type& val) {
    node *n = node_allocator::allocate();
    n->next = nullptr;
    try {
      sup::_construct(&(n->val), val);
      return n; 
    } catch (...) {
      // commit or rollback
      node_allocator::deallocate(n); 
      throw;
    }
    return n;
  }
  void delete_node(node* n) {
    sup::_destroy(&(n->val));
    node_allocator::deallocate(n);
  }
  // init buckts when table is first constructed
  void initialize_buckets(size_type n) {
    const size_type n_buckets = next_size(n);
    buckets.reserve(n_buckets);
    buckets.insert(buckets.end(), n_buckets, nullptr);
    num_of_elements = 0;
  }
  // helper function that returns the next prime capacity
  inline size_type next_size(unsigned long num) { return __sstl_next_prime(num); }
  // helper function that calls hash
  size_type bkt_num_key (const key_type& key) const {
    return hash(key) % buckets.size();
  }
  size_type bkt_num_key (const key_type& key, size_type bkt_num) const {
    return hash(key) % bkt_num;
  }
  // wrapper for hash function
  size_type bkt_num(const value_type& val) const { return bkt_num_key(get_key(val)); }
  size_type bkt_num(const value_type& val, size_type bkt_size) const 
    { return bkt_num_key(get_key(val), bkt_size); }
  void resize(size_type num_of_element_hint) {
    if (((float) num_of_elements / buckets.size()) > load_factor) {
      // trigger resize
      bucket_type new_buckets(next_size(buckets.size()));
      
      size_type bucket = 0;
      while (bucket < buckets.size()) {
        node* cur = buckets[bucket];
        while (cur != nullptr) {
          node* tmp = cur;
          cur = cur->next;
          insert_into_new_bucket(tmp, new_buckets);
        }
        buckets[bucket] = nullptr;
        ++bucket;
      }
      buckets.swap(new_buckets);
      new_buckets.clear();
    }
  }
  // insert the new node into the corresponding bucket such that 
  // the same keys are in a continuous range
  inline void insert_into_new_bucket(node* position, bucket_type& new_buckets) {
    size_type bucket = bkt_num(position->val, new_buckets.size());
    node* cur = new_buckets[bucket];
    if (cur == nullptr) {
      new_buckets[bucket] = position;
      position->next = nullptr;
    } else {
      // node* pre = cur;
      // while (cur != nullptr && !equals(get_key(position->val), get_key(cur->val))) {
      //   pre = cur;
      //   cur = cur->next;
      // }
      // pre->next = position;
      // position->next = cur;
      position->next = new_buckets[bucket];
      new_buckets[bucket] = position;
    }
  }
  friend __hashtable_iterator<Key, Value, HashFunc, ExtractKey, EqualKey, Alloc>;
};

/**
 * @brief given a value, insert it into the hashtable and returns 
 *  an iterator pointing to the value
 * 
 * @tparam Key - of the hashtable
 * @tparam Value - of the hashtable
 * @tparam HashFunc - of the hashtable
 * @tparam ExtractKey - extract key from the value of Value type
 * @tparam EqualKey - determine whether two keys are equal
 * @tparam Alloc - allocator type
 * @param val - the given value
 * @return hashtable<Key, Value, HashFunc, ExtractKey, EqualKey, Alloc>::iterator 
 *  to the inserted element
 */
template <class Key, class Value, class HashFunc, 
          class ExtractKey, class EqualKey, class Alloc>
typename hashtable<Key, Value, HashFunc, ExtractKey, EqualKey, Alloc>::iterator 
hashtable<Key, Value, HashFunc, ExtractKey, EqualKey, Alloc>::insert_equal(
  const typename hashtable<Key, Value, HashFunc, ExtractKey, EqualKey, Alloc>::value_type& val) {
  resize(num_of_elements + 1);
  
  size_type bucket = bkt_num(val);
  node* cur = buckets[bucket];
  node* new_value_node = new_node(val);
  if (cur == nullptr) {
    buckets[bucket] = new_value_node;
  } else {
    node* pre = cur;
    while (cur != nullptr && !equals(get_key(new_value_node->val), get_key(cur->val))) {
      pre = cur;
      cur = cur->next;
    }
    if (pre != cur) {
      pre->next = new_value_node;
      new_value_node->next = cur;
    } else {
      new_value_node->next = buckets[bucket];
      buckets[bucket] = new_value_node;
    }
  }

  ++num_of_elements;
  return iterator(new_value_node, this);
}

/**
 * @brief given a range [first, last) of values to insert 
 *  (duplicate keys are allowed)
 * 
 * @tparam Key - of the hashtable
 * @tparam Value - of the hashtable
 * @tparam HashFunc - of the hashtable
 * @tparam ExtractKey - extract key from the value of Value type
 * @tparam EqualKey - determine whether two keys are equal
 * @tparam Alloc - allocator type
 * @tparam InputIterator - iterator type
 * @param first - the start of the range
 * @param last - the end of the range
 */
template <class Key, class Value, class HashFunc, 
          class ExtractKey, class EqualKey, class Alloc>
template<class InputIterator>
void hashtable<Key, Value, HashFunc, ExtractKey, EqualKey, Alloc>::insert_equal(
  InputIterator first, InputIterator last) {
  while (first != last) {
    insert_equal(*first);
    ++first;
  }
}

/**
 * @brief Given a value, insert it into the hashtable. Return 
 *  a pair of iterator pointing to the inserted value, and a 
 *  bool varaible that denotes whether the insertion is 
 *  successful. If there is a duplicate key, the bool variable
 *  will be false and the iterator would point to the already
 *  inserted element.
 * 
 * @tparam Key - of the hashtable
 * @tparam Value - of the hashtable
 * @tparam HashFunc - of the hashtable
 * @tparam ExtractKey - extract key from the value of Value type
 * @tparam EqualKey - determine whether two keys are equal
 * @tparam Alloc - allocator type
 * @param val - the given value
 * @return std::pair<
 *  typename hashtable<Key, Value, HashFunc, ExtractKey, EqualKey, Alloc>::iterator, bool>
 *  - as described in the brief 
 */
template <class Key, class Value, class HashFunc, 
          class ExtractKey, class EqualKey, class Alloc>
std::pair<typename hashtable<Key, Value, HashFunc, ExtractKey, EqualKey, Alloc>::iterator, bool> 
hashtable<Key, Value, HashFunc, ExtractKey, EqualKey, Alloc>::insert_unique(
  const typename hashtable<Key, Value, HashFunc, ExtractKey, EqualKey, Alloc>::value_type& val) {
  resize(num_of_elements + 1);
  
  size_type bucket = bkt_num(val);
  node* cur = buckets[bucket];
  node* new_value_node = nullptr;
  if (cur == nullptr) {
    buckets[bucket] = new_node(val);
  } else {
    while (cur != nullptr) {
      // if element of the same key is found, just return
      if (equals(get_key(val), get_key(cur->val)))
        return std::pair<iterator, bool>(iterator(cur, this), false);
      cur = cur->next;
    }
    new_value_node = new_node(val);
    new_value_node->next = buckets[bucket];
    buckets[bucket] = new_value_node;
  }

  ++num_of_elements;
  return std::pair<iterator, bool>(iterator(new_value_node, this), true);
}

/**
 * @brief  given a range [first, last) of values to insert 
 *  (duplicate keys are not allowed)
 * 
 * @tparam Key - of the hashtable
 * @tparam Value - of the hashtable
 * @tparam HashFunc - of the hashtable
 * @tparam ExtractKey - extract key from the value of Value type
 * @tparam EqualKey - determine whether two keys are equal
 * @tparam Alloc - allocator type
 * @tparam InputIterator - iterator type
 * @param first - start of the range
 * @param last - end of the range 
 */
template <class Key, class Value, class HashFunc, 
          class ExtractKey, class EqualKey, class Alloc>
template<class InputIterator>
void hashtable<Key, Value, HashFunc, ExtractKey, EqualKey, Alloc>::insert_unique(
  InputIterator first, InputIterator last) {
  while (first != last) {
    insert_unique(*first);
    ++first;
  }
}

/**
 * @brief erase elements with the given key. return the number of elements 
 *  erased
 * 
 * @tparam Key - of the hashtable
 * @tparam Value - of the hashtable
 * @tparam HashFunc - of the hashtable
 * @tparam ExtractKey - extract key from the value of Value type
 * @tparam EqualKey - determine whether two keys are equal
 * @tparam Alloc - allocator type
 * @param key - the given key
 * @return hashtable<Key, Value, HashFunc, ExtractKey, EqualKey, Alloc>::size_type 
 *  - the number of elements removed
 */
template <class Key, class Value, class HashFunc, 
          class ExtractKey, class EqualKey, class Alloc>
typename hashtable<Key, Value, HashFunc, ExtractKey, EqualKey, Alloc>::size_type 
hashtable<Key, Value, HashFunc, ExtractKey, EqualKey, Alloc>::erase(
  const key_type& key) {
  size_type num_of_erased = 0;
  size_type bucket = bkt_num_key(key);
  node* first = buckets[bucket];

  if (first != nullptr) {
    // find the equal key
    while (first != nullptr && !equals(get_key(first->val), key)) {
      first = first->next;
    }
    if (first == nullptr)  // no such key
      return num_of_erased;
    
    node* next = first->next;

    // remove elements with the same key value 
    while (equals(get_key(first->val), key)) {
      delete_node(first);
      ++num_of_erased;
      --num_of_elements;
      first = next;
      if (first == nullptr)
        break;
      next = next->next;
    }

    buckets[bucket] = first;
  }

  return num_of_erased;
}

/**
 * @brief erase an element given its position
 * 
 * @tparam Key - of the hashtable
 * @tparam Value - of the hashtable
 * @tparam HashFunc - of the hashtable
 * @tparam ExtractKey - extract key from the value of Value type
 * @tparam EqualKey - determine whether two keys are equal
 * @tparam Alloc - allocator type
 * @param position - the given position of the element that 
 *  would be erased
 */
template <class Key, class Value, class HashFunc, 
          class ExtractKey, class EqualKey, class Alloc>
void hashtable<Key, Value, HashFunc, ExtractKey, EqualKey, Alloc>::erase(
  iterator& position) {
  node* n = position.cur;
  size_type bucket = bkt_num(*position);
  node* first = buckets[bucket];
  if (first != n) {
    node* pre;
    while (first != n) {
      pre = first;
      first = first->next;
    }
    pre->next = first->next;
    delete_node(first);
  } else {
    buckets[bucket] = n->next;
    delete_node(n);
    --num_of_elements;
  }
}

/**
 * @brief erase elements in a given range[first, last)
 * 
 * @tparam Key - of the hashtable
 * @tparam Value - of the hashtable
 * @tparam HashFunc - of the hashtable
 * @tparam ExtractKey - extract key from the value of Value type
 * @tparam EqualKey - determine whether two keys are equal
 * @tparam Alloc - allocator type
 * @param first - the start of the range
 * @param last - the end of the range
 */
template <class Key, class Value, class HashFunc, 
          class ExtractKey, class EqualKey, class Alloc>
void hashtable<Key, Value, HashFunc, ExtractKey, EqualKey, Alloc>::erase(
  iterator first, iterator last) {
  size_type start_bucket = bkt_num(*first);
  size_type end_bucket = bkt_num(*last);
  node * cur;
  // remove concecutive elements within the same bucket
  for (size_type bucket = start_bucket + 1; bucket < end_bucket; ++bucket) {
    cur = buckets[bucket];
    while (cur != nullptr) {
      node * tmp = cur->next;
      delete_node(cur);
      --num_of_elements;
      cur = tmp; 
    }
    buckets[bucket] = nullptr;
  }

  // remove elements after first but within the same bucket
  cur = buckets[start_bucket];
  if (cur != first.cur) {
    while(cur->next != first.cur) {
      cur = cur->next;
    }

    node* pre = cur;
    node* next = cur->next;
    while (cur != last.cur) {
      delete_node(cur);
      --num_of_elements;
      cur = next;
      if (cur == nullptr)
        break;
      next = next->next;
    }
    pre = next;
  } else { // remove the entire bucket before last
    node* next = cur->next;
    while (cur != last.cur) {
      delete_node(cur);
      --num_of_elements;
      cur = next;
      if (cur == nullptr) 
        break;
      next = next->next;
    }
    buckets[start_bucket] = next;
  }
  
  // remove elements before last but within the same bucket
  cur = buckets[end_bucket];

  if (cur == last.cur) {
    return;
  } else {
    node* next = cur->next;
    while (cur != last.cur) {
      delete_node(cur);
      --num_of_elements;
      cur = next;
      if (cur == nullptr) 
        break;
      next = next->next;
    }
    buckets[end_bucket] = next;
  }
}

/**
 * @brief clear the hashtable; keep the buckets untouched
 * 
 * @tparam Key - of the hashtable
 * @tparam Value - of the hashtable
 * @tparam HashFunc - of the hashtable
 * @tparam ExtractKey - extract key from the value of Value type
 * @tparam EqualKey - determine whether two keys are equal
 * @tparam Alloc - allocator type
 */
template <class Key, class Value, class HashFunc, 
          class ExtractKey, class EqualKey, class Alloc>
void hashtable<Key, Value, HashFunc, ExtractKey, EqualKey, Alloc>::clear() {
  if (num_of_elements > 0) {
    size_type bucket = 0;
    while (bucket < buckets.size()) {
      node* cur = buckets[bucket];
      while (cur != nullptr) {
        node* tmp = cur->next;
        delete_node(cur);
        cur = tmp;
      }
      buckets[bucket] = nullptr;
      ++bucket;
    }

    num_of_elements = 0;
  }
}

/**
 * @brief have n buckets. If the hash table already has 
 *  more than n buckets, the function has no effect.
 * 
 * @tparam Key - of the hashtable
 * @tparam Value - of the hashtable
 * @tparam HashFunc - of the hashtable
 * @tparam ExtractKey - extract key from the value of Value type
 * @tparam EqualKey - determine whether two keys are equal
 * @tparam Alloc - allocator type
 * @param n - the given size of the buckets
 */
template <class Key, class Value, class HashFunc, 
          class ExtractKey, class EqualKey, class Alloc>
void hashtable<Key, Value, HashFunc, ExtractKey, EqualKey, Alloc>::rehash(size_type n) {
  if (n > buckets.size()) {
    bucket_type new_buckets(n);
      size_type bucket = 0;
      while (bucket < buckets.size()) {
        node* cur = buckets[bucket];
        while (cur != nullptr) {
          node* tmp = cur;
          cur = cur->next;
          insert_into_new_bucket(tmp, new_buckets);
        }
        buckets[bucket] = nullptr;
        ++bucket;
      }
      buckets.swap(new_buckets);
      new_buckets.clear();
  }
}

/**
 * @brief reserve space for n elements.The bucket size would be
 *  next_size(n) If the hash table already has more than 
 *  next_size(n) buckets, the function has no effect.
 * 
 * @tparam Key - of the hashtable
 * @tparam Value - of the hashtable
 * @tparam HashFunc - of the hashtable
 * @tparam ExtractKey - extract key from the value of Value type
 * @tparam EqualKey - determine whether two keys are equal
 * @tparam Alloc - allocator type
 * @param n - the given number of the elements
 */
template <class Key, class Value, class HashFunc, 
          class ExtractKey, class EqualKey, class Alloc>
void hashtable<Key, Value, HashFunc, ExtractKey, EqualKey, Alloc>::reserve(size_type n) {
  if (next_size(n) > buckets.size()) {
    bucket_type new_buckets(next_size(n));
      size_type bucket = 0;
      while (bucket < buckets.size()) {
        node* cur = buckets[bucket];
        while (cur != nullptr) {
          node* tmp = cur;
          cur = cur->next;
          insert_into_new_bucket(tmp, new_buckets);
        }
        buckets[bucket] = nullptr;
        ++bucket;
      }
      buckets.swap(new_buckets);
      new_buckets.clear();
  }
}

/**
 * @brief assignment operator overloading
 * 
 * @tparam Key - of the hashtable
 * @tparam Value - of the hashtable
 * @tparam HashFunc - of the hashtable
 * @tparam ExtractKey - extract key from the value of Value type
 * @tparam EqualKey - determine whether two keys are equal
 * @tparam Alloc - allocator type
 * @param table - another hash table of the same type
 * @return hashtable<Key, Value, HashFunc, ExtractKey, EqualKey, Alloc>& 
 *  - reference to *this
 */
template <class Key, class Value, class HashFunc, 
          class ExtractKey, class EqualKey, class Alloc>
hashtable<Key, Value, HashFunc, ExtractKey, EqualKey, Alloc>& 
hashtable<Key, Value, HashFunc, ExtractKey, EqualKey, Alloc>::operator= 
(hashtable<Key, Value, HashFunc, ExtractKey, EqualKey, Alloc>& table) {
  clear();
  if (bucket_count() > table.bucket_count()) {
    iterator it = table.begin();
    while(it != table.end())
      insert_equal(*it);
  } else {
    buckets.reserve(table.buckets.size());
    iterator it = table.begin();
    while(it != table.end())
      insert_equal(*it);
  }
  return *this; 
}

}

#endif