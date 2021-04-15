#ifndef _SSTL_SET_H
#define _SSTL_SET_H

#include "sstl_allocator.hpp"
#include "sstl_rb_tree.hpp"

/**
 * @author Xiaoxi Sun
 **/

/**
 * Concepts:
 *  - explicit keyword: avoid unintentional type conversion in
 *    calling constructor
 **/

namespace sup {

template <class Key, class Compare=std::less<Key>, class Alloc=alloc>
class set {
public:
  typedef Key key_type;
  typedef Key value_type;

  typedef Compare key_compare;
  typedef Compare value_compare;

private:
  typedef rb_tree<key_type, value_type, 
    std::_Identity<value_type>, key_compare, Alloc> container_type;
  container_type t;

public:
  typedef typename container_type::const_pointer pointer;
  typedef typename container_type::const_pointer const_pointer;
  typedef typename container_type::const_reference reference;
  typedef typename container_type::const_reference const_reference;
  typedef typename container_type::const_iterator iterator;
  typedef typename container_type::const_iterator const_iterator;
  typedef typename container_type::const_reverse_iterator reverse_iterator;
  typedef typename container_type::const_reverse_iterator const_reverse_iterator;
  typedef typename container_type::size_type size_type;
  typedef typename container_type::difference_type difference_type;

  /*************** De-Constructors ***************/
  set(): t(Compare()) {}
  explicit set(const Compare& cmp): t(cmp) {}  
  template<class InputIterator>
  set(InputIterator first, InputIterator last)
    :t(Compare()) { t.insert_unique(first, last); }
  template<class InputIterator>
  set(InputIterator first, InputIterator last, const Compare& cmp)
    :t(cmp) { t.insert_unique(first, last); }
  // copy constructor
  set(const set<Key, Compare, Alloc>& s): t(s.t) {}

  ~set(){ clear(); }

  /*************** Accessors ***************/
  iterator begin() { return t.begin(); }
  iterator end() { return t.end(); }
  reverse_iterator rbegin() { return t.rbegin(); }
  reverse_iterator rend() { return t.rend(); }
  bool empty() { return t.empty(); }
  size_type size() { return t.size(); }
  size_type max_size() { return t.max_size(); }
  key_compare key_comp() { return t.key_comp(); }
  value_type value_comp() { return t.key_comp(); }
  void swap(set<Key, Compare, Alloc>& s) { t.swap(*this, s); }

  iterator find(const Key& k) { return t.find(k); }
  size_type count(const Key& k) { return t.count(k); }

  iterator lower_bound(const Key& k) { return t.lower_bound(k); }
  const_iterator lower_bound(const Key& k) const { return t.lower_bound(k); }
  iterator upper_bound(const Key& k) { return t.upper_bound(k); }
  const_iterator upper_bound(const Key& k) const { return t.upper_bound(k); }

  std::pair<iterator, iterator> 
  equal_range(const Key& k) { return t.equal_range(k); }
  std::pair<const_iterator, const_iterator> 
  equal_range(const Key& k) const { return t.equal_range(k); }

  /*************** Modifiers ***************/
  std::pair<iterator, bool> insert(const value_type& val) {
    std::pair<typename container_type::iterator, bool> result = t.insert_unique(val);
    return std::pair<iterator, bool>(result.first, result.second);
  }

  std::pair<iterator, bool> insert(iterator position, const value_type& val) {
    std::pair<typename container_type::iterator, bool> result =
      t.insert_unique((typename container_type::iterator&) position, val);
    return std::pair<iterator, bool>(result.first, result.second);
  }

  template<class InputIterator>
  void insert(InputIterator first, InputIterator last) {
    t.insert_unique(first, last);
  }

  void clear() { t.clear(); }

  /*************** Overloading ***************/
  set<Key, Compare, Alloc>& operator=(const set<Key, Compare, Alloc>&s) {
    t = s.t;
    return *this;
  }
};

}
#endif