#ifndef _SSTL_FORWARD_LIST_H
#define _SSTL_FORWARD_LIST_H

#include <stddef.h>

#include "sstl_allocator.hpp"
#include "sstl_iterator_base.hpp"
#include "sstl_construct.hpp"

/**
 * @author Xiaoxi Sun
 **/

/**
 * Concepts:
 *  - None
 **/

namespace sup {

/**
 * @brief helper class for forward_list_node
 * 
 */
struct __fwd_list_node_base {
  __fwd_list_node_base* next; 
};

/**
 * @brief Helper class for forward_list
 * 
 * @tparam _Tp - data type
 */
template<class T>
struct __fwd_list_node
  : public __fwd_list_node_base {
  T data; 
};

// helper function that inserts new_node after prev and 
// return a pointer to the new node 
inline __fwd_list_node_base* __fwd_list_make_link( 
  __fwd_list_node_base* prev, __fwd_list_node_base* new_node) {
  new_node->next = prev->next;
  prev->next = new_node;
  return new_node;
}

// helper function that counts the number of elements in a 
// forward list
inline size_t __fwd_list_size(__fwd_list_node_base* node) {
  size_t sz = 0;
  while (node != nullptr) {
    ++sz;
    node = node->next;
  }
  return sz;
}

template <class T>
struct __fwd_list_iterator {
  typedef __fwd_list_iterator<T>		_self;
  typedef __fwd_list_node<T> _Node;

  typedef T value_type;
  typedef T* pointer;
  typedef T& reference;
  typedef ptrdiff_t difference_type;
  typedef sup::forward_iterator_tag iterator_category;

  // constructors
  __fwd_list_iterator() {}
  __fwd_list_iterator(__fwd_list_node_base* nd): node(nd) {}
  
  reference operator*() const { return ((__fwd_list_node<T>*)node)->data; }
  pointer operator->() const 
  { return &(operator*()); }

  _self operator++() {
    node = node->next;
    return node;
  }
  _self operator++(int) {
    _self tmp = node; 
    node = node->next;
    return tmp;
  }

  _self next() {
    if (node) 
      return node->next;
    else 
      return _self(nullptr);
  }

  __fwd_list_node_base* node; 

  /**
   * @brief operator overload for iterator comparison
   */
  friend bool operator==(const _self& x, const _self& y) {
    return x.node == y.node;
  }

  /**
   * @brief operator overload for iterator comparison
   */
  friend bool operator!=(const _self& x, const _self& y) {
    return x.node != y.node;
  }
};

template <class T>
struct __fwd_list_const_iterator {
  typedef __fwd_list_const_iterator<T>		_self;
  typedef const __fwd_list_node<T> _Node;

  typedef T value_type;
  typedef const T* pointer;
  typedef const T& reference;
  typedef ptrdiff_t difference_type;
  typedef sup::forward_iterator_tag iterator_category;

  // constructors
  __fwd_list_const_iterator(): node(nullptr) {}
  __fwd_list_const_iterator(__fwd_list_node_base* nd): node(nd) {}

  reference operator*() const { return ((__fwd_list_node<T>*)node)->data; }
  pointer operator->() const 
  { return &(operator*()); }

  _self operator++() {
    node = node->next;
    return node;
  }
  _self operator++(int) {
    _self tmp = node; 
    node = node->next;
    return tmp;
  }

  _self next() {
    if (node) 
      return node->next;
    else 
      return _self(nullptr);
  }

  const __fwd_list_node_base* node; 

  /**
   * @brief operator overload for iterator comparison
   */
  friend bool operator==(const _self& x, const _self& y) {
    return x.node == y.node;
  }

  /**
   * @brief operator overload for iterator comparison
   */
  friend bool operator!=(const _self& x, const _self& y) {
    return x.node != y.node;
  }
};

template<typename T, typename _Alloc=sup::allocator<T>>
class forward_list {
public:
  typedef T value_type;
  typedef value_type* pointer;
  typedef const value_type* const_pointer ;
  typedef value_type& reference;
  typedef const value_type& const_reference ;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type ;

  typedef __fwd_list_iterator<T> iterator;
  typedef __fwd_list_const_iterator<T> const_iterator;

private:
  typedef __fwd_list_node<T> list_node;
  typedef __fwd_list_node_base list_node_base; 
  typedef typename sup::allocator<__fwd_list_node<T>> list_node_allocator;

public: 

  /******** De-constructors ********/
  forward_list() { head.next=0; }
  ~forward_list() { clear(); }

  /********** Accessors **********/

  /**
   * @brief return the iterator to the beginning of the list
   * 
   * @return iterator - to the beginning
   */
  iterator begin() { return iterator((list_node*)head.next); }

  /**
   * @brief return the iterator to the end of the list
   * 
   * @return iterator - to the end
   */
  iterator end() { return iterator(nullptr); }

  /**
   * @brief return the size of the list
   * 
   * @return size_type - the size
   */
  size_type size() { return __fwd_list_size(head.next);  }

  /**
   * @brief return whether the list is empty
   * 
   * @return true - the list is empty
   * @return false - the list is not empty
   */
  bool empty() { return head.next == nullptr; }

  /**
   * @brief clear the list
   */
  void clear () {
    list_node_base *cur = head.next;
    while (cur != nullptr) {
      list_node_base* tmp = cur;
      cur = cur->next;
      destory_node((list_node*) tmp);
    }
    head.next = nullptr;
  } 

  /**
   * @brief swap the list with another list
   * 
   * @param l - another list
   */
  void swap(forward_list& l) {
    list_node_base* tmp = head.next;
    head.next = l.head.next;
    l.head.next = tmp;
  }

  /**
   * @brief return the data at the front of the list
   * 
   * @return reference - to the data
   */
  reference front() {
    return ((list_node*) head.next)->data;
  }

  /**
   * @brief push an element from the front
   * 
   * @param val - to be pushed
   */
  void push_front(const value_type& val) {
    __fwd_list_make_link(&head, create_node(val));
  }

  /**
   * @brief pop an element out from the front
   */
  void pop_front() {
    list_node_base* tmp = head.next;
    head.next = tmp->next;
    destory_node((list_node*)tmp);
  }

private:
  list_node_base head;

  // create a node with value
  static list_node* create_node(const value_type& val) {
    list_node* node = list_node_allocator::allocate(1);
    try{ // stl catch
      sup::_construct(&(node->data), val);
      node->next = nullptr;
    } catch(...) {
      list_node_allocator::deallocate(node, 1);
      throw;
    }

    return node;
  }

  // destroy a node and deallocate the space
  static void destory_node(list_node* node) {
    sup::_destroy(&(node->data));
    list_node_allocator::deallocate(node, 1);
  }
}; 

}

#endif