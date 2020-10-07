#ifndef _SSTL_LIST_H
#define _SSTL_LIST_H


#include "sstl_allocator.hpp"
#include "sstl_construct.hpp"
#include "sstl_iterator.hpp"

/**
 * @author Xiaoxi Sun
 */

/**
 * Concepts:
 *
 **/

/**
 * Questions:
 *  - GNU source uses lit_node_base to reduce code duplication.
 *  - no list_const_iterator implemented
 *  - no reverse_const_iterator
 *  - std::allocator_traits used
 *  - no max_size()
 **/

namespace sup {

template <class T>
struct _list_node {
  _list_node* prev;
  _list_node* next;
  T data;
};

template <class T>
struct _list_iterator {
  // for convenience
  typedef _list_iterator<T> iterator;
  typedef _list_node<T>* link_type;

  typedef _list_iterator<T> _self;
  typedef _list_node<T> _node;

  // iterator traits
  typedef bidirectional_iterator_tag iterator_category;
  typedef T value_type;
  typedef T* pointer;
  typedef T& reference;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;

  link_type node;  // pointer to the current data entity

  _list_iterator(link_type x) : node(x) {}

  _list_iterator() {}
  _list_iterator(const iterator& x) : node(x.node) {}

  // iterator comparison
  bool operator==(const _self& x) const { return node == x.node; }
  bool operator!=(const _self& x) const { return node != x.node; }

  reference operator*() const { return node->data; }
  pointer operator->() const { return &(operator*()); }

  _self& operator++() {
    node = node->next;
    return *this;
  }

  _self operator++(int) {
    _self temp = *this;
    ++*this;
    return temp;
  }

  _self& operator--() {
    node = node->prev;
    return *this;
  }

  _self operator--(int) {
    _self temp = *this;
    --*this;
    return temp;
  }
};

/**
template <class T>
struct _list_const_iterator {};
**/

template <class T, class Alloc = sup::allocator<T>>
class list {
 protected:
  typedef typename std::allocator_traits<Alloc> _T_alloc_traits;
  typedef _list_node<T> list_node;
  typedef _list_node<T>* link_type;
  typedef sup::allocator<_list_node<T>> list_node_allocator;

 public:
  typedef T value_type;
  typedef Alloc allocator_type;
  typedef size_t size_type;

  typedef value_type& reference;
  typedef const value_type& const_reference;

  typedef typename _T_alloc_traits::pointer pointer;
  typedef typename _T_alloc_traits::const_pointer const_pointer;
  typedef typename _list_iterator<T>::iterator iterator;
  // there should be list_const_iterator
  typedef typename sup::reverse_iterator<iterator> reverse_iterator;
  // thus no reverse_const_iterator
  typedef
      typename sup::iterator_traits<iterator>::difference_type difference_type;

  /******** Constructors ********/
  list() {
    node = get_node();
    node->next = node;
    node->prev = node;
  }

  /******** Iterators ********/
  iterator begin();
  iterator end();
  reverse_iterator rbegin();
  reverse_iterator rend();

  /******** Size ********/
  bool empty() const;
  size_type size() const;
  size_type max_size() const;

  /******** element access ********/
  reference front();
  reference back();

  /********* modifiers ********/
  template <class InputIterator>
  void assign(InputIterator first, InputIterator last);
  void assign(size_type n, const value_type& val);

  void push_front(const value_type& val);
  void pop_front();
  void push_back(const value_type& val);
  void pop_back();

  iterator insert(iterator position, const value_type& val);
  void insert(iterator position, size_type n, const value_type& val);
  template <class InputIterator>
  void insert(iterator position, InputIterator first, InputIterator last);

  iterator erase(iterator position);
  iterator erase(iterator first, iterator last);

  void swap(list& x);

  void resize(size_type n, value_type val = value_type());

  void clear();

  /******** Operations ********/
  void splice(iterator position, list& x);
  void splice(iterator position, list& x, iterator i);
  void splice(iterator position, list& x, iterator first, iterator last);

  void remove(const value_type& val);
  template <class Predicate>
  void remove_if(Predicate pred);

  void unique();
  template <class BinaryPredicate>
  void unique(BinaryPredicate binary_pred);

  void merge(list& x);
  template <class Compare>
  void merge(list& x, Compare comp);

  void sort();
  template <class Compare>
  void sort(Compare comp);

  void reverse();

  allocator_type get_allocator() const;

 protected:
  // these functions are for convenience & efficiency
  link_type get_node() { return list_node_allocator::allocate(1); }
  void put_node(link_type p) { list_node_allocator::deallocate(p); }
  link_type create_node(const T& x) {
    link_type p = get_node();
    sup::_construct(&p->data, x);
    return p;
  }
  void destory_node(link_type p) {
    sup::_destroy(&p->data);
    put_node(p);
  }
  void empty_initialiatize() {
    node = get_node();
    node->next = node;
    node->prev = node;
  }

  link_type node;
};

template <class T, class Alloc>
typename list<T, Alloc>::allocator_type list<T, Alloc>::get_allocator() const {}

template <class T, class Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::begin() {
  return (node->next);
}

template <class T, class Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::end() {
  return node;
}

template <class T, class Alloc>
typename list<T, Alloc>::reverse_iterator list<T, Alloc>::rbegin() {
  return node->prev;
}

template <class T, class Alloc>
typename list<T, Alloc>::reverse_iterator list<T, Alloc>::rend() {
  return node;
}

/******** Size ********/
template <class T, class Alloc>
bool list<T, Alloc>::empty() const {
  return node->next == node;
}

template <class T, class Alloc>
typename list<T, Alloc>::size_type list<T, Alloc>::size() const {
  size_type sz = 0;
  sup::distance(begin(), end(), sz);
  return sz;
}

/******** element access ********/
template <class T, class Alloc>
typename list<T, Alloc>::reference list<T, Alloc>::front() {
  return *begin();
}

template <class T, class Alloc>
typename list<T, Alloc>::reference list<T, Alloc>::back() {
  return *(--end());
}

/********* modifiers ********/
template <class T, class Alloc>
template <class InputIterator>
void list<T, Alloc>::assign(InputIterator first, InputIterator last) {}

template <class T, class Alloc>
void list<T, Alloc>::assign(size_type n, const value_type& val) {}

template <class T, class Alloc>
void list<T, Alloc>::push_front(const value_type& val) {}
template <class T, class Alloc>
void list<T, Alloc>::pop_front() {}
/**
 * @brief push an element at the back of val
 * 
 * @tparam T - element type parameter
 * @tparam Alloc - allocator type
 * @param val - value to be pushed back
 */
template <class T, class Alloc>
void list<T, Alloc>::push_back(const value_type& val) {
  insert(end(), val);
}
template <class T, class Alloc>
void list<T, Alloc>::pop_back() {}

/**
 * @brief insert an element before the element pointed by position
 *
 * @tparam T - element type parameter
 * @tparam Alloc - allocator type
 * @param position - position of insertion
 * @param val - value of the element
 * @return list<T, Alloc>::iterator - an iterator to the inserted element
 */
template <class T, class Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::insert(
    iterator position, const value_type& val) {
  link_type tmp = create_node(val);
  tmp->next = position.node;
  tmp->prev = position.node->prev;
  position.node->prev = tmp;
  tmp->prev->next = tmp;
  return tmp;
}
template <class T, class Alloc>
void list<T, Alloc>::insert(iterator position, size_type n,
                            const value_type& val) {}
template <class T, class Alloc>
template <class InputIterator>
void list<T, Alloc>::insert(iterator position, InputIterator first,
                            InputIterator last) {}

template <class T, class Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::erase(iterator position) {}

template <class T, class Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::erase(iterator first,
                                                        iterator last) {}

template <class T, class Alloc>
void list<T, Alloc>::swap(list& x) {}

template <class T, class Alloc>
void list<T, Alloc>::resize(size_type n, value_type val) {}

template <class T, class Alloc>
void list<T, Alloc>::clear() {}

/******** Operations ********/
template <class T, class Alloc>
void list<T, Alloc>::splice(iterator position, list& x) {}
template <class T, class Alloc>
void list<T, Alloc>::splice(iterator position, list& x, iterator i) {}
template <class T, class Alloc>
void list<T, Alloc>::splice(iterator position, list& x, iterator first,
                            iterator last) {}

template <class T, class Alloc>
void list<T, Alloc>::remove(const value_type& val) {}
template <class T, class Alloc>
template <class Predicate>
void list<T, Alloc>::remove_if(Predicate pred) {}

template <class T, class Alloc>
void list<T, Alloc>::unique() {}

template <class T, class Alloc>
template <class BinaryPredicate>
void list<T, Alloc>::unique(BinaryPredicate binary_pred) {}

template <class T, class Alloc>
void list<T, Alloc>::merge(list& x) {}

template <class T, class Alloc>
template <class Compare>
void list<T, Alloc>::merge(list& x, Compare comp) {}

template <class T, class Alloc>
void list<T, Alloc>::sort() {}

template <class T, class Alloc>
template <class Compare>
void list<T, Alloc>::sort(Compare comp) {}

template <class T, class Alloc>
void list<T, Alloc>::reverse() {}

template <class T, class Alloc>
typename list<T, Alloc>::allocator_type get_allocator() {}
}  // namespace sup

#endif