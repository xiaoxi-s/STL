#ifndef _SSTL_LIST_H
#define _SSTL_LIST_H

#include "type_traits"

#include "sstl_allocator.hpp"
#include "sstl_construct.hpp"
#include "sstl_iterator.hpp"
#include "sstl_type_tratis.hpp"

/**
 * @author Xiaoxi Sun
 */

/**
 * Concepts:
 *   Very interesting Quick Sort implementation!
 **/

/**
 * Questions:
 *  - GNU source uses lit_node_base to reduce code duplication.
 *  - no list_const_iterator implemented
 *  - no reverse_const_iterator
 *  - std::allocator_traits used
 *  - std::distance()
 *  - no max_size()
 *  - std::__is_integer, std::__true_type, and std::__false_type used
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
  ~list() {
    clear();
    list_node_allocator::deallocate(node, 1);
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
  template<class InputIterator1, class InputIterator2>
  void assign(InputIterator1 first, InputIterator2 last);

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
  void put_node(link_type p) { list_node_allocator::deallocate(p, 1); }
  link_type create_node(const T& x) {
    link_type p = get_node();
    sup::_construct(&p->data, x);
    return p;
  }
  // destory data in a node & release the space of the node
  void destory_node(link_type p) {
    sup::_destroy(&p->data);
    put_node(p);
  }
  // initialize the list when being constructed
  void empty_initialiatize() {
    node = get_node();
    node->next = node;
    node->prev = node;
  }
  // transfer the content [first, last) to the front of position
  void transfer(iterator position, iterator first, iterator last) {
    if (position != last) {
      (last.node->prev)->next = position.node;
      (first.node->prev)->next = last.node;
      (position.node->prev)->next = first.node;
      link_type tmp = last.node->prev;
      last.node->prev = first.node->prev;
      first.node->prev = position.node->prev;
      position.node->prev =  tmp;
    }
  }
  // for assign
  template<class InputIterator1, class InputIterator2>
  void assign_aux(InputIterator1 first, InputIterator2 last, std::__false_type);
  template<class Size>
  void assign_aux(Size first, const T& last, std::__true_type);

  link_type node;
};

/**
 * @brief get allocator
 * 
 * @tparam T - element type parameter
 * @tparam Alloc - allocator type
 * @return list<T, Alloc>::allocator_type - allocator of the type
 */
template <class T, class Alloc>
typename list<T, Alloc>::allocator_type list<T, Alloc>::get_allocator() const {
  return allocator_type();
}

/**
 * @brief begin iterator
 * 
 * @tparam T - element type parameter
 * @tparam Alloc - allocator type
 * @return list<T, Alloc>::iterator - begin iterator
 */
template <class T, class Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::begin() {
  return (node->next);
}

/**
 * @brief end iterator
 * 
 * @tparam T - element type parameter
 * @tparam Alloc - allocator type
 * @return list<T, Alloc>::iterator - end iterator
 */
template <class T, class Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::end() {
  return node;
}

/**
 * @brief reverse begin iterator
 * 
 * @tparam T - element type parameter
 * @tparam Alloc - allocator type
 * @return list<T, Alloc>::reverse_iterator  - the instance of
 */
template <class T, class Alloc>
typename list<T, Alloc>::reverse_iterator list<T, Alloc>::rbegin() {
  return node->prev;
}

/**
 * @brief reverse end iterator
 * 
 * @tparam T - element type parameter
 * @tparam Alloc - allocator type
 * @return list<T, Alloc>::reverse_iterator - the instance of
 */
template <class T, class Alloc>
typename list<T, Alloc>::reverse_iterator list<T, Alloc>::rend() {
  return node;
}

/******** Size ********/

/**
 * @brief Retur whether this list is empty
 * 
 * @tparam T - element type parameter
 * @tparam Alloc - allocator type
 * @return true - the list is empty
 * @return false - otherwise
 */
template <class T, class Alloc>
bool list<T, Alloc>::empty() const {
  return node->next == node;
}

/**
 * @brief return the size of the list
 * 
 * @tparam T - element type parameter
 * @tparam Alloc - allocator type
 * @return list<T, Alloc>::size_type - the size
 */
template <class T, class Alloc>
typename list<T, Alloc>::size_type list<T, Alloc>::size() const {
  size_type result = 0;
  iterator it = this->node;

  while (it != this->node->prev) 
    ++result, ++it;

  return result;
}

/******** element access ********/

/**
 * @brief Return the reference to the first element
 * 
 * @tparam T - element type parameter
 * @tparam Alloc - allocator type
 * @return list<T, Alloc>::reference - the first element
 */
template <class T, class Alloc>
typename list<T, Alloc>::reference list<T, Alloc>::front() {
  return *begin();
}

/**
 * @brief return the reference to last element 
 * 
 * @tparam T - element type parameter
 * @tparam Alloc - allocator type
 * @return list<T, Alloc>::reference - the last element
 */
template <class T, class Alloc>
typename list<T, Alloc>::reference list<T, Alloc>::back() {
  return *(--end());
}

/********* modifiers ********/

// look for below for documents
template <class T, class Alloc>
template<class InputIterator1, class InputIterator2>
void list<T, Alloc>::assign(InputIterator1 first, InputIterator2 last) {
  typedef typename std::__is_integer<InputIterator1>::__type _Integral;

  assign_aux(first, last, _Integral());
}

/**
 * @brief assign the value based on the values in range [first, last)
 * 
 * @tparam T - element type parameter
 * @tparam Alloc - allocator type
 * @tparam InputIterator1 - iterator type
 * @tparam InputIterator2 - iterator type
 * @param first - start of the range
 * @param last - end of the range
 */
template <class T, class Alloc>
template <class InputIterator1, class InputIterator2>
void list<T, Alloc>::assign_aux(InputIterator1 first, InputIterator2 last, std::__false_type) {
  if (!this->empty()) {
    clear();
  }

  while (first != last) {
    this->push_back(*first);
    ++first;
  }
}

/**
 * @brief assign the list to contain n values.
 * 
 * @tparam T - element type parameter
 * @tparam Alloc - allocator type
 * @tparam Size - size type
 * @param n - number of values
 * @param val - the value
 */
template <class T, class Alloc>
template <class Size>
void list<T, Alloc>::assign_aux(Size n, const T& val, std::__true_type) {
  if (!this->empty()) {
    clear();
  }

  for (size_t i = 0; i < n; ++i) {
    this->push_back(val);
  }
}

/**
 * @brief push an element at the front
 * 
 * @tparam T - element type parameter
 * @tparam Alloc - allocator type
 * @param val - value to be pushed in 
 */
template <class T, class Alloc>
void list<T, Alloc>::push_front(const value_type& val) {
  insert(begin(), val);
}

/**
 * @brief pop one element from the front
 * 
 * @tparam T - element type parameter
 * @tparam Alloc - allocator type
 */
template <class T, class Alloc>
void list<T, Alloc>::pop_front() {
  erase(begin());
}
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
/**
 * @brief pop one element out from the back of the list
 * 
 * @tparam T 
 * @tparam Alloc 
 */
template <class T, class Alloc>
void list<T, Alloc>::pop_back() {
  iterator tmp = end();
  erase(--tmp);
}

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

/**
 * @brief insert a value n times
 *
 * @tparam T - element type parameter
 * @tparam Alloc - allocator type
 * @param position - position of insertion
 * @param val - value of the element
 */
template <class T, class Alloc>
void list<T, Alloc>::insert(iterator position, size_type n,
                            const value_type& val) {
  for(size_type i = 0; i < n; ++i) 
    insert(position, val);
}

/**
 * @brief insert a range of values in the range [first, last)
 *
 * @tparam T - element type parameter
 * @tparam Alloc - allocator type
 * @param position - position of insertion
 * @param val - value of the element
 */

/**
 * @brief insert a range of values in the range [first, last)
 *
 * @tparam T - element type parameter
 * @tparam Alloc - allocator type
 * @tparam InputIterator - iterator type 
 * @param position - after the value to be inserted
 * @param first - start of the range 
 * @param last - end of the range
 */
template <class T, class Alloc>
template <class InputIterator>
void list<T, Alloc>::insert(iterator position, InputIterator first,
                            InputIterator last) {
  while(first != last) 
    insert(position, *first);
}

/**
 * @brief remove and destory the element pointed by position
 * 
 * @tparam T - element type parameter
 * @tparam Alloc - allocator type
 * @param position - position of insertion
 * @return list<T, Alloc>::iterator - an iterator to the element after the removed element
 */
template <class T, class Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::erase(iterator position) {
  link_type next = position.node->next;
  link_type curr = position.node;
  curr->prev->next = curr->next;
  curr->next->prev = curr->prev;
  destory_node(curr);

  return iterator(next);
}

/**
 * @brief remove elements in the range [first, last)
 * 
 * @tparam T - element type parameter
 * @tparam Alloc - allocator type
 * @param first - start position 
 * @param last - end position
 * @return list<T, Alloc>::iterator 
 */
template <class T, class Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::erase(iterator first,
                                                        iterator last) {
  while(first != last) {
    first = erase(first);
  }
  return last;
}

/**
 * @brief swap the content of two lists
 * 
 * @tparam T - element type parameter
 * @tparam Alloc - allocator type
 * @param x - another list
 */
template <class T, class Alloc>
void list<T, Alloc>::swap(list& x) {
  link_type temp = x.node;
  x.node = this->node;
  this->node = temp;
}

/**
 * @brief clear the list and fill in with n vals
 * 
 * @tparam T - element type parameter
 * @tparam Alloc - allocator type
 * @param n - the number of vals
 * @param val - the value
 */
template <class T, class Alloc>
void list<T, Alloc>::resize(size_type n, value_type val) {
  if (!empty()) 
    clear();

  for (size_type i = 0; i < n; ++i) 
    push_back(val);
}

/**
 * @brief clear the list
 * 
 * @tparam T - element type parameter
 * @tparam Alloc - allocator type
 */
template <class T, class Alloc>
void list<T, Alloc>::clear() {
  link_type curr = node->next;
  while (curr != node->prev) {
    link_type tmp = curr;
    curr = curr->next;
    destory_node(tmp);
  }
  node->prev = node;
  node->next = node;
}

/******** Operations ********/

/**
 * @brief transfer the list x into the place before position in *this
 * 
 * @tparam T - element type parameter
 * @tparam Alloc - allocator type
 * @param position - the place before which x would be inserted
 * @param x - the input list
 */
template <class T, class Alloc>
void list<T, Alloc>::splice(iterator position, list& x) {
  if (!x.empty()) 
    transfer(position, x.begin(), x.end());
}

/**
 * @brief transfer a single element into the place before position in *this
 * 
 * @tparam T - element type parameter
 * @tparam Alloc - allocator type
 * @param position - the place before which the element would be inserted
 * @param x - dummy variable
 * @param i - transfer a single element pointed by iterator i
 */
template <class T, class Alloc>
void list<T, Alloc>::splice(iterator position, list&, iterator i) {
  iterator j = i;
  ++j;
  if (position == i || position == j) return; 
  transfer(position, i, j);
}

/**
 * @brief transfer a range [first, last) of elements into the place before position in *this
 *  position should not be in the range of [first, last)
 * 
 * @tparam T - element type parameter
 * @tparam Alloc - allocator type
 * @param position - the place before which the element would be inserted
 * @param first - the start element 
 * @param last - the end element
 */
template <class T, class Alloc>
void list<T, Alloc>::splice(iterator position, list&, iterator first,
                            iterator last) {
  if (first != last) {
    transfer(position, first, last);
  }
}

/**
 * @brief remove all vals
 * 
 * @tparam T - element type parameter
 * @tparam Alloc - allocator type
 * @param val - the values to be removed
 */
template <class T, class Alloc>
void list<T, Alloc>::remove(const value_type& val) {
  iterator curr = begin();
  iterator last = end();
  while (curr != last) {
    iterator tmp = curr;
    ++curr;
    if (*tmp == val) {
      erase(tmp);
    }
  }
}
/**
 * @brief remove according to a predicate
 * 
 * @tparam T - element type parameter
 * @tparam Alloc - allocator type
 * @tparam Predicate - predicate type
 * @param pred - the predicate applied to value
 */
template <class T, class Alloc>
template <class Predicate>
void list<T, Alloc>::remove_if(Predicate pred) {
  iterator curr = begin();
  iterator last = end();
  while (curr != last) {
    iterator tmp = curr;
    ++curr;
    if (pred(*tmp)) {
      erase(tmp);
    }
  }
}

/**
 * @brief remove consecutive same values
 * 
 * @tparam T - element type parameter
 * @tparam Alloc - allocator type
 */
template <class T, class Alloc>
void list<T, Alloc>::unique() {
  if (node->next == node || node->next->next == node) return;

  iterator first = begin();
  iterator second = first;
  ++second;

  while(second != end()) {
    if (*first == *second) {
      second = erase(second);
    } else {
      ++first;
      ++second;
    }
  }
}

/**
 * @brief remove consecutive same values according to a predicate
 * 
 * @tparam T - element type parameter
 * @tparam Alloc - allocator type
 * @tparam BinaryPredicate - a predicate
 */
template <class T, class Alloc>
template <class BinaryPredicate>
void list<T, Alloc>::unique(BinaryPredicate binary_pred) {
    if (node->next == node || node->next->next == node) return;

  iterator first = begin();
  iterator second = first;
  ++second;

  while(second != end()) {
    if (binary_pred(*first, *second)) {
      second = erase(second);
    } else {
      ++first;
      ++second;
    }
  }
}

/**
 * @brief merge this list and the input list assuming they are in an increasing order
 * 
 * @tparam T - element type parameter
 * @tparam Alloc - allocator type
 * @param x the sorted list to be merged
 */
template <class T, class Alloc>
void list<T, Alloc>::merge(list& x) {
  iterator first1 = this->begin();
  iterator last1 = this->end();
  iterator first2 = x.begin();
  iterator last2 = x.end();

  while(first1 != last1 && first2 != last2) {
    if (*first1 < *first2) {
      ++first1;
    } else {
      iterator next = first2;
      transfer(first1, first2, ++next);
      first2 = next;
    }
  }

  if (first2 != last2) {
    transfer(last1, first2, last2);
  }
}

/**
 * @brief merge with comparator
 * 
 * @tparam T - element type parameter
 * @tparam Alloc - allocator type
 * @tparam Compare - function type
 * @param x - the listed to be merged with *this
 * @param comp - the comparator function
 */
template <class T, class Alloc>
template <class Compare>
void list<T, Alloc>::merge(list& x, Compare comp) {
  iterator first1 = this->begin();
  iterator last1 = this->end();
  iterator first2 = x.begin();
  iterator last2 = x.end();

  while(first1 != last1 && first2 != last2) {
    if (comp(*first1, *first2)) {
      ++first1;
    } else {
      iterator next = first2;
      transfer(first1, first2, ++next);
      first2 = next;
    }
  }

  if (first2 != last2) {
    transfer(last1, first2, last2);
  }
}

/**
 * @brief sort the list in an infreasing order
 * 
 * @tparam T - element type parameter
 * @tparam Alloc - allocator type
 */
template <class T, class Alloc>
void list<T, Alloc>::sort() {
  if (node->next == node || node->next->next == node) return;

  // itermediate lists
  list<T, Alloc> carry;
  list<T, Alloc> counter[64];

  int fill = 0;

  while(!empty()) {
    carry.splice(carry.begin(), *this, begin());
    int i = 0;
    while( i < fill && !counter[i].empty()) {
      counter[i].merge(carry);
      carry.swap(counter[i++]);
    }
    carry.swap(counter[i]);
    if (i == fill) ++fill;
  }

  for (int i = 1; i < fill; ++i) 
    counter[i].merge(counter[i-1]);

  swap(counter[fill-1]);
}

/**
 * @brief sort with a comparator
 * 
 * @tparam T - element type parameter
 * @tparam Alloc - allocator type
 * @tparam Compare - function type
 * @param comp - the comparator function
 */
template <class T, class Alloc>
template <class Compare>
void list<T, Alloc>::sort(Compare comp) {
  if (node->next == node || node->next->next == node) return;

  // itermediate lists
  list<T, Alloc> carry;
  list<T, Alloc> counter[64];

  int fill = 0;

  while(!empty()) {
    carry.splice(carry.begin(), *this, begin());
    int i = 0;
    while( i < fill && !counter[i].empty()) {
      counter[i].merge(carry, comp);
      carry.swap(counter[i++]);
    }
    carry.swap(counter[i]);
    if (i == fill) ++fill;
  }

  for (int i = 1; i < fill; ++i) 
    counter[i].merge(counter[i-1], comp);
    
  swap(counter[fill-1]);
}

/**
 * @brief reverse the list
 * 
 * @tparam T - element type parameter
 * @tparam Alloc - allocator type
 */
template <class T, class Alloc>
void list<T, Alloc>::reverse() {
  if (node->next == node || node->next->next == node) return;

  iterator first = begin();
  ++first;
  
  while(first != end()) {
    iterator old = first;
    ++first;
    transfer(begin(), old, first);
  }
}

}  // namespace sup

#endif