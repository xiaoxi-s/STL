#ifndef _SSTL_DEQUE_H
#define _SSTL_DEUQE_H

#include "sstl_allocator.hpp"
#include "sstl_iterator.hpp"
#include "sstl_uninitialized.hpp"

/**
 * @author Xiaoxi Sun
 */

/**
 * Concepts:
 *  - Now I see why the range [first, last) is a good assumption
 *  - std::copy and std::copy_backward are used
 **/

/**
 * Questions:
 *  - 
 **/

namespace sup {

inline size_t __deque_buf_size(size_t n, size_t sz) {
  return n!=0 ? n : (sz < 512 ? size_t(512/sz) : size_t(1));
}

template <class T, class Ref, class Ptr, size_t BufSiz>
struct __deque_iterator {
  typedef __deque_iterator<T, T&, T*, BufSiz> iterator;
  // typedef __deque_iterator<T, T&, T*, BufSiz> const_iterator;

  typedef random_access_iterator_tag iterator_category;
  typedef T value_type;
  typedef Ptr pointer;
  typedef Ref reference;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef T** map_pointer;
  typedef __deque_iterator _self;

  static size_t buffer_size(){ return __deque_buf_size(BufSiz, sizeof(T)); }
  void set_node(map_pointer new_node) {
    node = new_node;
    first = *new_node;
    last = first + difference_type(buffer_size());
  }

  reference operator* () const { return *cur; }
  pointer operator-> () const {return &(operator*());}
  difference_type operator - (const _self&x) const {
    return difference_type(buffer_size()) * (node - x.node - 1) +
      (cur - first) + (x.last - x.cur);
  }

  _self& operator++ () {
    ++cur;
    if (cur == last)  {
      set_node(node + 1);
      cur = first;
    }

    return *this;
  }
  _self operator++ (int) {
    _self tmp = *this; 
    ++(*this);
    return tmp;
  }
  _self& operator-- () {
    if (cur == first) {
      set_node(node - 1);
      cur = last; 
    }
    --cur;
    return *this;
  }
  _self operator--(int) {
    _self tmp = *this;
    --(*this);
    return tmp;
  }
  _self& operator+= (difference_type n) {
    difference_type offset = n + (cur-first);
    if (offset >=0 && offset < difference_type(buffer_size()))
      cur += n;
    else {
      difference_type node_offset = offset > 0 ? offset / difference_type(buffer_size())
        : - difference_type((-offset - 1)/buffer_size()) - 1;
      set_node(node + node_offset);

      cur = first + (offset - node_offset*difference_type(buffer_size()));
    }
    return *this;
  }
  _self operator+ (const difference_type n) const {
    _self tmp = *this;
    return tmp += n;
  }
  _self& operator-= (difference_type n) { return *this += (-n); }
  _self operator- (const difference_type n) const {
    _self tmp = *this;
    return tmp -= n;
  }

  reference operator[](difference_type n) const {
    return *(*this + n);
  }

  bool operator== (const _self& x) const { return cur == x.cur; }
  bool operator!= (const _self& x) const { return cur != x.cur; }
  bool operator< (const _self& x) const { return (node == x.node) ? (cur < x.cur) : (node < x.node); }

  T* cur;           // current element in the buffer
  T* first;         // first element in the buffer
  T* last;          // last element in the buffre
  map_pointer node; // to the map
};

template <class T, class Alloc=alloc, size_t BufSiz=0>
class deque {
public: 
  typedef T value_type;
  typedef value_type* pointer; 
  typedef T& reference;
  typedef ptrdiff_t difference_type;
  typedef size_t size_type;
  typedef __deque_iterator<T, T&, T*, BufSiz> iterator;

  /********** Con-destructors **********/
  deque() {create_map_and_nodes(0);}
  deque(int n, const value_type& val) { fill_initialize(n, val); }
  ~deque() {
    clear();
    data_allocator::deallocate(start.first, buffer_size());
    map_allocator::deallocate(map, map_size);
  }

  /*********** Accessors ***********/
  static size_t buffer_size(){ return __deque_buf_size(BufSiz, sizeof(T)); }
  iterator begin() { return start; }
  iterator end() { return finish; }
  
  reference operator[] (size_type n) { return start[difference_type(n)]; }
  reference front() { return *start; };
  reference back() { return *(finish-1); }
  size_type size() const { return finish - start; }
  size_type max_size() const { return size_type(-1); }
  bool empty() const { return finish == start; }

  /*********** Modifiers ***********/
  void push_back(const value_type& val);
  void push_front(const value_type& val);
  
  void pop_back();
  void pop_front();
  void clear();
  iterator erase(iterator position);
  iterator erase(iterator first, iterator last);
  iterator insert(iterator position, const value_type& val);

protected:
  typedef pointer* map_pointer; 
  typedef sup::simple_alloc<value_type, Alloc> data_allocator;
  typedef sup::simple_alloc<pointer, Alloc> map_allocator;

protected:
  // point to the first element and the last 
  iterator start;
  iterator finish;
  // to the pointers to value arrays
  map_pointer map;
  size_type map_size;

  // helper functions
  size_type initialize_map_size() { // minimum size of the map is 8
    return size_type(8); 
  }
  // create the map and initialize nodes
  void create_map_and_nodes(size_type num_elements) {
    size_type num_nodes = num_elements / buffer_size() + 1;
    size_type init_size = initialize_map_size();
    map_size = // create buffers at both sides
      (init_size > (num_nodes + 2)) ? init_size : (num_nodes + 2);

    map = map_allocator::allocate(map_size);
    map_pointer nstart = map + (map_size - num_nodes) / 2;
    // nfinish is the exact end as the exact ending of the data might be in the last buffer
    map_pointer nfinish = nstart + num_nodes - 1; 
    map_pointer cur;

    for (cur = nstart; cur <= nfinish; ++cur) {
      *cur = data_allocator::allocate(buffer_size());
    }

    start.set_node(nstart);
    finish.set_node(nfinish);
    start.cur = start.first;
    // note that is the cur that points to the position after the last element
    finish.cur = finish.first + num_elements%buffer_size();
  }
  // fill in values  during initialization
  void fill_initialize(size_type n, const value_type& val) {
    create_map_and_nodes(n);
    map_pointer cur;
    try {
      for (cur = start.node; cur < finish.node; ++cur) 
        sup::uninitialized_fill(*cur, *cur + buffer_size(), val);
      sup::uninitialized_fill(finish.first, finish.cur, val);
    } catch (...) {
      // should be commit or rollback
    }
  }
  // helper function for push back when buffer is not enough
  void push_back_aux(const value_type& val) {
    value_type val_copy = val;

    reserve_map_at_back();
    *(finish.node + 1) = data_allocator::allocate(buffer_size());
    try {
      sup::_construct(finish.cur, val_copy);
      finish.set_node(finish.node + 1);
      finish.cur = finish.first;
    } catch(...) {
      data_allocator::deallocate(*(finish.node + 1), 1);
      throw;
    }
  }
  // helper function for push front when buffer is not enough
  void push_front_aux(const value_type& val) {
    value_type val_copy = val;

    reserve_map_at_front();
    *(start.node - 1) = data_allocator::allocate(buffer_size());
    try {
      start.set_node(start.node - 1);
      start.cur = start.last - 1;
      sup::_construct(start.cur, val_copy);
    } catch(...) { // commit or rollback
      start.set_node(start.node + 1);
      start.cur = start.first;
      data_allocator::deallocate(*(finish.node + 1), 1);
      throw;
    }
  }
  // reserve more nodes at the back
  void reserve_map_at_back(size_type nodes_to_add=1) {
    if (nodes_to_add + 1 > map_size - (finish.node -map))
      reallocate_map(nodes_to_add, false);
  }
  // reserve more nodes at the front
  void reserve_map_at_front(size_type nodes_to_add=1) {
    if (nodes_to_add > start.node - map)
      reallocate_map(nodes_to_add, true);
  }
  // reallocate or relocate the map
  void reallocate_map(size_type nodes_to_add, bool add_at_front) {
    size_type old_num_nodes = finish.node - start.node + 1;
    size_type new_num_nodes = nodes_to_add + old_num_nodes;

    map_pointer new_nstart;
    if (map_size > 2*new_num_nodes) {
      new_nstart = map + (map_size - new_num_nodes)/2 + 
        (add_at_front ? nodes_to_add : 0);
      if (new_nstart < start.node) 
        std::copy(start.node, finish.node + 1, new_nstart);
      else 
        std::copy_backward(start.node, finish.node + 1, new_nstart + old_num_nodes);
    } else {
      size_type new_map_size = map_size + (map_size > new_num_nodes ? map_size : new_num_nodes) + 2; 
      
      map_pointer new_map = map_allocator::allocate(new_map_size);
      new_nstart = new_map + (new_map_size - new_num_nodes)/2 + 
        (add_at_front ? nodes_to_add : 0);
      std::copy(start.node, finish.node + 1, new_nstart);

      map_allocator::deallocate(map, map_size);
      map = new_map;
      map_size = new_map_size;
    }

    start.set_node(new_nstart);
    finish.set_node(new_nstart + old_num_nodes - 1);
  }
  // helper for insertion
  iterator insert_aux(iterator position, const value_type& val) {
    difference_type elem_before = position - start;
    value_type val_copy = val;
    if (elem_before > (size()>>1) ) { // before there are more elements
      push_back(back());
      iterator back1 = finish;
      --back1;
      iterator back2 = back1;
      --back2;
      std::copy_backward(position, back2, back1);
    } else { // otherwise
      push_front(front());
      iterator front1 = start;
      ++front1;
      iterator front2 = front1;
      ++front2;
      iterator new_pos = position;
      --new_pos;
      std::copy(front2, new_pos, front1);
    }
    *position = val;
    return position;
  }
};

/**
 * @brief push an element into the back of the deque
 * 
 * @tparam T - element type parameter
 * @tparam Alloc - allocator type
 * @tparam BufSiz - buffer size
 * @param val - to be pushed in
 */
template <class T, class Alloc, size_t BufSiz>
void sup::deque<T, Alloc, BufSiz>::push_back(const value_type& val) {
  if (finish.cur != finish.last - 1) { // there is space remaining 
    sup::_construct(finish.cur, val);
    ++finish.cur; // inplace ++ just for the pointer cur
  } else 
    push_back_aux(val);
}

/**
 * @brief push an element into the front of the deque
 * 
 * @tparam T - element type parameter
 * @tparam Alloc - allocator type
 * @tparam BufSiz - buffer size
 * @param val - to be pushed in
 */
template <class T, class Alloc, size_t BufSiz>
void sup::deque<T, Alloc, BufSiz>::push_front(const value_type& val) {
  if (start.cur != start.first) { // there is space remaining 
    sup::_construct(start.cur - 1, val);
    --start.cur;
  } else 
    push_front_aux(val);
}

/**
 * @brief pop an element out at the back
 * 
 * @tparam T - element type parameter
 * @tparam Alloc - allocator type
 * @tparam BufSiz - buffer size
 */
template <class T, class Alloc, size_t BufSiz>
void sup::deque<T, Alloc, BufSiz>::pop_back() {
  if (finish.cur != finish.first) {
    --finish.cur;
    sup::_destroy(finish.cur);
  } else {
    data_allocator::deallocate(finish.first, buffer_size());
    finish.set_node(finish.node-1);
    finish.cur = finish.last - 1;
    sup::_destroy(finish.cur);
  }
}

/**
 * @brief pop an element out at the front
 * 
 * @tparam T - element type parameter
 * @tparam Alloc - allocator type
 * @tparam BufSiz - buffer size
 */
template <class T, class Alloc, size_t BufSiz>
void sup::deque<T, Alloc, BufSiz>::pop_front() {
  if (start.cur + 1 != start.last) {
    sup::_destroy(start.cur);
    ++start.cur; 
  } else {
    sup::_destroy(start.cur);
    data_allocator::deallocate(start.first, buffer_size());
    start.set_node(start.node + 1);
    start.cur = start.first;
  }
}

/**
 * @brief clear the deque so that only one buffer remains
 * 
 * @tparam T - element type parameter
 * @tparam Alloc - allocator type
 * @tparam BufSiz - buffer size
 */
template <class T, class Alloc, size_t BufSiz>
void sup::deque<T, Alloc, BufSiz>::clear() {
  // full buffers in the middle
  for (map_pointer p_mid_buffer = start.node + 1; p_mid_buffer < finish.node; ++p_mid_buffer) 
    sup::_destroy(*p_mid_buffer, *p_mid_buffer + buffer_size());

  // start buffer and the finish buffer
  if (start.node != finish.node) { // keep the start buffer
    sup::_destroy(finish.first, finish.cur);
    sup::_destroy(start.cur, start.last); // good assumption about the range
    data_allocator::deallocate(finish.first, buffer_size());
  } else {
    sup::_destroy(start.cur, finish.cur);
  }

  finish = start;
}

/**
 * @brief erase the element pointed by the iterator
 * 
 * @tparam T - element type parameter
 * @tparam Alloc - allocator type
 * @tparam BufSiz - buffer size
 * @param position - of hte element that would be erased
 * @return sup::deque<T, Alloc, BufSiz>::iterator pointing to the 
 *  element after the removed element
 */
template <class T, class Alloc, size_t BufSiz>
typename sup::deque<T, Alloc, BufSiz>::iterator 
sup::deque<T, Alloc, BufSiz>::erase(iterator position) {
  iterator next = position;
  ++next;

  difference_type index = position - start;
  if (index < (size() >> 1)) {
    // copy to [start.cur + 1, next)
    std::copy_backward(start, position, next); 
    pop_front();
  } else {
    std::copy(next, finish, position);
    pop_back();
  }

  return start + index;
}

/**
 * @brief erase elements in a given range [first, last)
 * 
 * @tparam T - element type parameter
 * @tparam Alloc - allocator type
 * @tparam BufSiz - buffer size
 * @param first - the start of the range
 * @param last - the end of the range
 * @return sup::deque<T, Alloc, BufSiz>::iterator - points to the 
 *  element after the last removed elements. 
 */
template <class T, class Alloc, size_t BufSiz>
typename sup::deque<T, Alloc, BufSiz>::iterator 
sup::deque<T, Alloc, BufSiz>::erase(iterator first, iterator last) {
  if (first == start && last == finish) {
    clear();
    return finish;
  }

  difference_type n = last - first;
  difference_type elem_before = first - start;
  if (elem_before < (size() - n)/2) { // element before the range is less 
    std::copy_backward(start, first, last);
    iterator new_start = start + n;
    sup::_destroy(start, new_start);
    for (map_pointer cur = start.node; cur < new_start.node; ++cur) {
      map_allocator::deallocate(cur, buffer_size());
    }
    start = new_start;
  } else { // other wise
    std::copy(last, finish, first);
    iterator new_finish = finish - n;
    sup::_destroy(new_finish, finish);
    for (map_pointer cur = new_finish.node + 1; cur <= finish.node; ++cur) {
      map_allocator::deallocate(cur, buffer_size());
    }
    finish = new_finish;
  }

  return start + elem_before;
}

/**
 * @brief insert an element of the value before the element pointed 
 *  by position 
 * 
 * @tparam T - element type parameter
 * @tparam Alloc - allocator type
 * @tparam BufSiz - buffer size
 * @param position - the place after insertion happens
 * @param val - of the inserted element
 * @return sup::deque<T, Alloc, BufSiz>::iterator - pointing to the element 
 *  inserted
 */
template <class T, class Alloc, size_t BufSiz>
typename sup::deque<T, Alloc, BufSiz>::iterator 
sup::deque<T, Alloc, BufSiz>::insert(iterator position, const value_type& val) {
  if (position.cur == start.cur) {
    push_front(val);
    return start;
  } else if (position.cur == finish.cur) {
    push_back(val);
    iterator tmp = finish;
    --tmp;
    return tmp;
  } else {
    return insert_aux(position, val);
  }
}

}
#endif
