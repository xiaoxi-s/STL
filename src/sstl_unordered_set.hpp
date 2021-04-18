#ifndef _SSTL_HASH_SET_H
#define _SSTL_HASH_SET_H

#include <functional>

#include "sstl_hashtable.hpp"

namespace sup {

template <class Key, class HashFunc=std::hash<Key>, 
          class EqualKey=std::equal_to<Key>, class Alloc=alloc>
class unordered_set {
private:
  typedef sup::hashtable<Key, Key, HashFunc, std::_Identity<Key>, EqualKey, Alloc>
    container_type;
  container_type ht;

public:
  typedef typename container_type::key_type key_type;
  typedef typename container_type::value_type value_type;
  typedef typename container_type::hasher hasher;
  typedef typename container_type::key_equal key_equal;
  typedef typename container_type::size_type size_type;
  typedef typename container_type::difference_type difference_type;

  // not allowed to modify key
  typedef typename container_type::const_pointer pointer;
  typedef typename container_type::const_pointer const_pointer;
  typedef typename container_type::const_reference reference;
  typedef typename container_type::const_reference const_reference;
  typedef typename container_type::const_iterator iterator;
  typedef typename container_type::const_iterator const_iterator;

  /*************** de-constructors ***************/
  unordered_set(): ht(100, hasher(), key_equal()) {}
  explicit unordered_set(size_type n): ht(n, hasher(), key_equal()) {}
  unordered_set(size_type n, const hasher& hf): ht(n, hf, key_equal()) {}
  unordered_set(size_type n, const hasher& hf, const key_equal& eq)
    : ht(n, hf, eq) {}
  
  template <class InputIterator>
  unordered_set(InputIterator first, InputIterator last)
    : ht(100, hasher(), key_equal()) {
    ht.insert_unique(first, last);
  }

  template <class InputIterator>
  unordered_set(InputIterator first, InputIterator last, size_type n)
    : ht(n, hasher(), key_equal()) {
    ht.insert_unique(first, last);
  }

  template <class InputIterator>
  unordered_set(InputIterator first, InputIterator last, 
    size_type n, const hasher& hf) 
    : ht(n, hf, key_equal()) {
    ht.insert_unique(first, last);
  }

  template <class InputIterator>
  unordered_set(InputIterator first, InputIterator last, 
    size_type n, const hasher& hf, const key_equal& eq) 
    : ht(n, hf, eq) {
    ht.insert_unique(first, last);
  }

  /*************** Accessors ***************/
  // size
  size_type size() const { return ht.size(); }
  size_type max_size() const { return ht.max_size(); }
  bool empty() const { return ht.empty(); }
  size_type count(const key_type& key) { return ht.count(key); }

  // iterators
  iterator begin() { return ht.begin(); }
  iterator end() { return ht.end(); }
  const_iterator begin() const { return ht.begin(); }
  const_iterator end() const { return ht.end(); }
  iterator find(const key_type& key) { return ht.find(key); }
  const_iterator find(const key_type& key) const { return ht.find(key); } 

  std::pair<iterator, iterator> equal_range(const key_type& key) 
    { return ht.equal_range(key); }

  // load factor
  float load_factor() const { return ht.load_factor(); }
  float max_load_factor() const { return ht.max_load_factor(); }
  void max_load_factor(float max_factor) { ht.max_load_factor(max_factor); }

  hasher hash_function () const { return ht.hash_function(); }
  key_equal key_eq () const { return ht.key_eq(); }

  /*************** Modifiers ***************/
  // insert
  std::pair<iterator, bool> insert(const value_type& val) {
    std::pair<typename container_type::iterator, bool> result = ht.insert_unique(val);
    return std::pair<iterator, bool>(iterator(result.first), result.second);
  }

  template <class InputIterator>
  void insert(InputIterator first, InputIterator last) {
    ht.insert_unique(first, last);
  }
  // erase
  size_type erase(const value_type& val) { ht.erase(val); }
  void erase(iterator it) { ht.erase(it); }
  void erase(iterator first, iterator last) { ht.erase(first, last); }
  void clear() { ht.clear(); }

  // hashtable space
  void rehash(size_type n) { ht.rehash(n); }
  void reserve(size_type n) { ht.reserve(n); }
};

}

#endif