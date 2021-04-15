#ifndef _SSTL_MAP_H
#define _SSTL_MAP_H

#include "sstl_rb_tree.hpp"
#include <utility>
/**
 * @author Xiaoxi Sun
 **/

/**
 * Concepts:
 *  - explicit keyword: avoid unintentional type conversion in
 *    calling constructor
 **/

namespace sup {

template<class Key, class Value, class Compare=std::less<Key>, class Alloc=alloc>
class map {
public:
  typedef Key key_type;
  typedef Value data_type;
  typedef Value mapped_type;
  typedef std::pair<const key_type, mapped_type> pair_type;
  typedef Compare key_compare;

  // value comparator
  class value_compare
    : public std::binary_function<pair_type, pair_type, bool> {
    friend class map<Key, Value, Compare, Alloc>;
  protected:
    Compare comp;
    value_compare(Compare c):comp(c) {}
  public:
    bool operator() (pair_type v1, pair_type v2) {
      return comp(v1.first, v2.first);
    }
  };

private: 
  typedef rb_tree<key_type, pair_type, std::_Select1st<pair_type>, key_compare, Alloc> container_type;
  container_type t;

public:
  typedef typename container_type::pointer pointer;
  typedef typename container_type::const_pointer const_pointer;
  typedef typename container_type::reference reference;
  typedef typename container_type::const_reference const_reference;
  typedef typename container_type::iterator iterator;
  typedef typename container_type::const_iterator const_iterator;
  typedef typename container_type::reverse_iterator reverse_iterator;
  typedef typename container_type::const_reverse_iterator const_reverse_iterator;
  typedef typename container_type::size_type size_type;
  typedef typename container_type::difference_type difference_type;

  /************ De-construcotrs ************/
  map(): t(Compare()) {}
  explicit map(const Compare& comp): t(comp) {}

  template<class InputIterator>
  map(InputIterator first, InputIterator last): t(Compare()) {
    t.insert_unique(first, last);
  }
  template<class InputIterator>
  map(InputIterator first, InputIterator last, const Compare& comp): t(comp) {
    t.insert_unique(first, last);
  }
  map(const map<Key, Value, Compare, Alloc>& m): t(m.t) {}
  map<Key, Value, Compare, Alloc>& operator= (map<Key, Value, Compare, Alloc>& c) {
    t = c.t;
    return *this;
  }

  /************** Accessors **************/
  key_compare key_comp() { return t.key_comp(); }
  value_compare value_comp() { return value_compare(t.key_comp()); }
  iterator begin() { return t.begin(); }
  const_iterator begin() const { return t.begin(); }
  iterator end() { return t.end(); }
  const_iterator end() const { return t.end(); }
  reverse_iterator rbegin() { return t.rbegin(); }
  const_reverse_iterator rbegin() const { return t.rbegin(); }
  reverse_iterator rend() { return t.rend(); }
  const_reverse_iterator rend() const { return t.rend(); }
  bool empty() const { return t.empty(); }
  size_type size() const { return t.size(); }
  size_type max_size() const { return t.max_size(); }
  data_type& operator[] (const key_type& k) {
    return *(t.insert_unique(value_type(k, Value()).first)).second;
  }
  void swap(map<Key, Value, Compare, Alloc>& m2) { t.swap(m2.t); }
  iterator find(const key_type& k) { return t.find(k); }
  const_iterator find(const key_type& k) const { return t.find(k); }
  size_type count(const key_type& k) { return t.count(k); }

  iterator lower_bound(const key_type& k) { return t.lower_bound(k); }
  const_iterator lower_bound(const key_type& k) const { return t.lower_bound(k); }

  iterator upper_bound(const key_type& k) { return t.upper_bound(k); }
  const_iterator upper_bound(const key_type& k) const { return t.upper_bound(k); }

  std::pair<iterator, iterator> 
  equal_range(const key_type& k) { return t.equal_range(k); }
  std::pair<const_iterator, const_iterator> 
  equal_range(const key_type& k) const { return t.equal_range(k); }

  /************** Modifiers **************/
  std::pair<iterator, bool> insert(const pair_type& pair_val) {
    return t.insert_unique(pair_val); 
  }
  iterator insert(iterator position, const pair_type& pair_val) {
    t.isnert(position, pair_val);
  }
  template<class InputIterator>
  void insert(InputIterator first, InputIterator last) {
    t.insert_unique(first, last);
  }

  void erase(iterator position) {
    t.erase(position); 
  }
  size_type erase(const key_type& key) { return t.erase(key); } 
  void erase(iterator first, iterator last) { t.erase(first, last); }
  void clear() { t.clear(); }

  template<class Key_, class Value_, class Compare_, class Alloc_>
  friend bool operator == (const map<Key_, Value_, Compare_, Alloc_>& m1, 
                           const map<Key_, Value_, Compare_, Alloc_>& m2);
};

template<class Key, class Value, class Compare, class Alloc>
bool operator == (map<Key, Value, Compare, Alloc>& m1, 
                  map<Key, Value, Compare, Alloc>& m2) {
  return m1.t == m2.t;
}
}

#endif