#ifndef _SSTL_RB_TREE_H
#define _SSTL_RB_TREE_H

#include <utility>

#include "sstl_allocator.hpp"
#include "sstl_construct.hpp"
#include "sstl_iterator_base.hpp"
#include "sstl_stack.hpp"
#include "sstl_iterator.hpp"

#include <iostream>

/**
 * @author Xiaoxi Sun
 **/

/**
 * Concepts:
 *  - _STL_TRY in create_node
 *  - uses utility library in std
 **/

namespace sup {

typedef bool __rb_tree_color_type;
const __rb_tree_color_type __rb_tree_red = false;
const __rb_tree_color_type __rb_tree_black = true;

struct __rb_tree_node_base {

  typedef __rb_tree_color_type color_type;
  typedef __rb_tree_node_base* base_ptr;

  color_type color;
  base_ptr parent;
  base_ptr left;
  base_ptr right;

  /**
   * @brief return the minimum value in a subtree starting 
   *  from cur
   * 
   * @param cur - the root of the subtree
   * @return base_ptr - to the minimum node base
   */
  static base_ptr minimum(base_ptr cur) {
    while(cur->left != nullptr) {
      cur = cur->left;
    }

    return cur;
  }

  /**
   * @brief return the maximum value in a subtree starting 
   *  from cur
   * 
   * @param cur - the root of the subtree
   * @return base_ptr - to the maximum node base
   */
  static base_ptr maximum(base_ptr cur) {
        while(cur->right != nullptr) {
      cur = cur->right;
    }

    return cur;
  }
};

template <class Value>
struct __rb_tree_node: public __rb_tree_node_base {
  typedef __rb_tree_node<Value>* link_type;
  Value value_field;
};

struct __rb_tree_iterator_base {
  typedef __rb_tree_node_base::base_ptr base_ptr; 
  typedef bidirectional_iterator_tag iterator_category;
  typedef ptrdiff_t difference_type; 

  base_ptr node;

  // move to the the next larger element
  void increment() {
    if (node->right != nullptr) {
      node = node->right;
      while (node->left != nullptr) 
        node = node->left;
    } else {
      base_ptr y = node->parent;
      while(y->right == node) {
        node = y;
        y = node->parent;
      }

      if (node->right != y) 
        node = y;
      // otherwise, node is end()
    }
  }

  // move to the before smaller element
  void decrement() {
    if (node->color == __rb_tree_red && 
        node->parent->parent == node)  //when node is end()
      node = node->right;
    else if (node->left != nullptr) {
      base_ptr y = node->left;

      while(y->right != nullptr) 
        y = y->right;

      node = y; 
    } else {
      base_ptr y = node->parent;
      while(node == y->left) {
        node = y;
        y = y->parent;
      }

      node = y;
    }
  }

  friend bool operator==(__rb_tree_iterator_base x, __rb_tree_iterator_base y) {
    return x.node == y.node;
  }
};

template <class Value, class Ref, class Ptr>
struct __rb_tree_iterator: public __rb_tree_iterator_base {
  typedef Value value_type;
  typedef Ref reference;
  typedef Ptr pointer; 
  typedef __rb_tree_iterator<Value, Value&, Value*> iterator;
  typedef __rb_tree_iterator<Value, const Value&, const Value*> const_iterator;
  typedef __rb_tree_iterator<Value, Ref, Ptr> _self;
  typedef __rb_tree_node<Value>* link_type; 

  __rb_tree_iterator() {}
  __rb_tree_iterator(link_type n) { node = n; }
  __rb_tree_iterator(const iterator& it) { node = it.node; }

  reference operator*() const { return ((link_type)node)->value_field; }
  pointer operator->() const { return &(operator*()); }

  _self& operator++() {increment(); return*this;}
  _self operator++(int) {
    _self tmp = *this;
    increment();
    return tmp;
  }

  _self& operator--() {decrement(); return*this;}
  _self operator--(int) {
    _self tmp = *this;
    decrement();
    return tmp;
  }

  friend bool operator==(
    __rb_tree_iterator x, __rb_tree_iterator y) {
    return x.node == y.node;
  }
};

// x->right is the node rotated up
inline void __rb_tree_rotate_left(__rb_tree_node_base* x, __rb_tree_node_base*& root) {
  __rb_tree_node_base* y = x->right;
  x->right = y->left;
  if (y->left != nullptr) {
    y->left->parent = x;
  }
  y->parent = x->parent;

  if (x == root) {
    root = y;
  } else if (x == x->parent->left) {
    x->parent->left = y;
  } else {
    x->parent->right = y;
  }

  x->parent = y;
  y->left = x;
}

// x->left is the node rotated up
inline void __rb_tree_rotate_right(__rb_tree_node_base* x, __rb_tree_node_base*& root) {
  __rb_tree_node_base* y = x->left;
  x->left = y->right;
  if (y->right != nullptr) {
    y->right->parent = x;
  }
  y->parent = x->parent;

  if (x == root) {
    root = y;
  } else if (x == x->parent->left) {
    x->parent->left = y;
  } else {
    x->parent->right = y;
  }

  x->parent = y;
  y->right = x;
}

// rebalance the rb tree given the inserted node and the root of the tree
inline void __rb_tree_insert_rebalance(
  __rb_tree_node_base* x, __rb_tree_node_base*& root) {
  x->color = __rb_tree_red;
  // x is not root, and the parent is red: we must rebalance the tree
  while ( x != root && (x->parent->color == __rb_tree_red)) { 
    if (x->parent == x->parent->parent->left) {
      __rb_tree_node_base* y = x->parent->parent->right; // y is parent's uncle
      if (y != nullptr && y->color == __rb_tree_red) {
        // uncle exists and it is red
        // the grand parent must be black!
        x->parent->color = __rb_tree_black;
        y->color = __rb_tree_black;
        x->parent->parent->color = __rb_tree_red;
        x = x->parent->parent;
      } else { // no uncle or uncle is black
        if ( x == x->parent->right) { // need double rotate
          x = x->parent;
          __rb_tree_rotate_left(x, root);
        }

        // x now is the child!
        x->parent->color = __rb_tree_black;
        x->parent->parent->color = __rb_tree_red;
        __rb_tree_rotate_right(x->parent->parent, root);
      }
    } else { // uncle is on the left side of parent
      __rb_tree_node_base* y = x->parent->parent->left; // y is uncle
      if (y != nullptr && y->color == __rb_tree_red) {
        // uncle exists and it is red
        // the grand parent must be black!
        x->parent->color = __rb_tree_black;
        y->color = __rb_tree_black;
        x->parent->parent->color = __rb_tree_red;
        x = x->parent->parent;
      } else {
        // no uncle or uncle is black
        if ( x == x->parent->left ) {
          x = x->parent;
          __rb_tree_rotate_right(x, root);
        }

        // x is now the child!
        x->parent->color = __rb_tree_black;
        x->parent->parent->color = __rb_tree_red;
        __rb_tree_rotate_left(x->parent->parent, root);
      }
    }

  } 

  root->color = __rb_tree_black;
}

inline void __rb_tree_erase_rebalance(
  __rb_tree_node_base* x, __rb_tree_node_base* parent, __rb_tree_node_base*& root) {
  // extract parent
  __rb_tree_node_base* other; 
  // x is the root of an empty sub tree, or x's color is black
  // and x is not a root
  while( ((!x) || x->color == __rb_tree_black) && x != root) {
    if (parent->left == x) { // x is on the left of its parent
      other = parent->right;
      if (other != nullptr && other->color == __rb_tree_red) {
        // case 1: sibling is red
        parent->color = __rb_tree_red;
        other->color = __rb_tree_black;
        __rb_tree_rotate_left(parent, root);
        other = parent->right;
      }
      if ( (other->left == nullptr || other->left->color == __rb_tree_black) && 
           (other->right == nullptr || other->right->color == __rb_tree_black) ) {
        // case 2: both children of the sibling are black
        // bottom up -> find red node elsewhere
        other->color = __rb_tree_red;
        x = parent;
        parent = x->parent;
      } else { // there is a child of the sibling that is red
        if (other->right == nullptr || other->right->color == __rb_tree_black) {
          // case 3: left is red
          other->color = __rb_tree_red;
          other->left->color = __rb_tree_black;
          __rb_tree_rotate_right(other, root);
          other = parent->right;
        } 
        // case 4: right is red
        
        other->color = parent->color;
        parent->color = __rb_tree_black;
        other->right->color = __rb_tree_black;
        __rb_tree_rotate_left(parent, root);
        x = root;
        break;
      }
    } else {// x is on the right of its parent
      other = parent->left;
      if (other != nullptr && other->color == __rb_tree_red) {
        // case 1: sibling is red
        other->color = __rb_tree_black;
        parent->color = __rb_tree_red;
        __rb_tree_rotate_right(parent, root);
        other = parent->left;
      }

      if ((other->left == nullptr || other->left->color == __rb_tree_black) &&
          (other->right == nullptr || other->right->color == __rb_tree_black)) {
        // both children of sibling are black
        other->color = __rb_tree_red;
        x = parent;
        parent = parent->parent;
      } else {
        // one of the child of the sibiling is red
        if (other->left == nullptr || other->left->color == __rb_tree_black) {
          // left is balck, and right must be red
          other->color = __rb_tree_red;
          other->right->color = __rb_tree_black;
          __rb_tree_rotate_left(other, root);
          other = parent->left;
        }
        // left is red
        other->left->color = __rb_tree_black;
        other->color = parent->color;
        parent->color = __rb_tree_black;
        __rb_tree_rotate_right(parent, root);
        x = root;
        break;
      }
    }
  }
  if (x != nullptr)
    x->color = __rb_tree_black;
}

template <class Key, class Value, class KeyOfValue, 
  class Compare, class Alloc=alloc>
class rb_tree {
protected:
  typedef void* void_pointer;
  typedef __rb_tree_node_base* base_ptr;
  typedef __rb_tree_node<Value> rb_tree_node; 
  typedef sup::simple_alloc<__rb_tree_node<Value>, Alloc> rb_tree_node_allocator;
  typedef __rb_tree_color_type color_type;

public:
  typedef Key key_type;
  typedef Value value_type;
  typedef Value* pointer;
  typedef const Value* const_pointer;
  typedef value_type& reference;
  typedef const value_type& const_reference; 
  typedef rb_tree_node* link_type;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;

  typedef __rb_tree_iterator<value_type, reference, pointer> iterator;
  typedef typename iterator::const_iterator const_iterator;
  typedef sup::reverse_iterator<iterator> reverse_iterator;
  typedef sup::reverse_iterator<const_iterator> const_reverse_iterator;

protected:
  // helper functions for node
  // create a node
  link_type get_node() {
    return rb_tree_node_allocator::allocate(1);
  }
  // deallocate the space of a node
  void put_node(link_type p) {
    rb_tree_node_allocator::deallocate(p, 1);
  }
  // create a node init with val
  link_type create_node(const value_type& val) {
    link_type tmp = get_node();
    try {
      sup::_construct(&(tmp->value_field), val);
    } catch(...) {
      put_node(tmp);
      throw;
    }
    return tmp;
  }
  // clone the value and color of a node
  link_type clone_node(link_type p) {
    link_type tmp = create_node(p->value_field);
    tmp->color = p->color;
    tmp->left = nullptr;
    tmp->right = nullptr;

    return tmp;
  }
  // destory a node by destoring the value and put the node
  void destroy_node (link_type p) {
    sup::_destroy(&(p->value_field));
    put_node(p);
  }

protected:
  // data members
  size_type node_count; 
  link_type header; // a node "above" root
  Compare key_compare; // a function for comparing keys  

  link_type& root() const { return (link_type&) header->parent;}
  link_type& left_most() const { return (link_type&) header->left;}
  link_type& right_most() const { return (link_type&) header->right;}
  // traversal methods:
  void pre_order(link_type cur) {
    pre_order_helper(cur);
  }
  void in_order(link_type cur) {
    in_order_helper(cur);
  }
  // traversal helper methods:
  void pre_order_helper(link_type cur) {
    if (cur) {
      std::cout << cur->color << "  " <<cur->value_field.second << std::endl;
      pre_order((link_type) cur->left);
      pre_order((link_type) cur->right);
    }
  }
  void in_order_helper(link_type cur) {
    if (cur) {
      in_order((link_type) cur->left);
      std::cout << cur->color << "  " <<cur->value_field.second << std::endl;
      in_order((link_type) cur->right);
    }
  }
  // extract data members from link_type
  // return left child
  static link_type& left(link_type p) {
    return (link_type&) (p->left);
  }
  // return right child
  static link_type& right(link_type cur) {
    return (link_type&) (cur->right);
  }
  // return parent
  static link_type& parent(link_type cur) {
    return (link_type&) (cur->parent);
  }
  // return value
  static reference value(link_type cur) {
    return cur->value_field;
  }
  // return key
  static const Key& key(link_type cur) {
    return KeyOfValue() (value(cur));
  }
  // return color
  static color_type& color(link_type cur) {
    return (color_type&) (cur->color);
  }

  // extract data members from base_ptr
  // return left child
  static link_type& left(base_ptr cur) {
    return (link_type&) (cur->left);
  }
  // return right child
  static link_type& right(base_ptr cur) {
    return (link_type&) (cur->right);
  }
  // return parent
  static link_type& parent(base_ptr cur) {
    return (link_type&) (cur->parent);
  }
  // return value
  static reference value(base_ptr cur) {
    return ((link_type)cur)->value_field;
  }
  // return key
  static const Key& key(base_ptr cur) {
    return KeyOfValue() (value(link_type(cur)));
  }
  // return color
  static color_type& color(base_ptr cur) {
    return (color_type&) (link_type(cur)->color);
  }
  // return the minimum node
  static link_type minimum(link_type cur) {
    return (link_type) __rb_tree_node_base::minimum(cur);
  }
  // return the maximum node
  static link_type maximum(link_type cur) {
    return (link_type) __rb_tree_node_base::maximum(cur);
  }

private:
  // insert aux; x is to be inserted; y is its parent
  // returns an interator pointing to the inserted node
  iterator __insert(base_ptr x_, base_ptr y_, const value_type& val) {
    link_type x = (link_type) x_;
    link_type y = (link_type) y_;
    link_type z;
    bool comp = key_compare(KeyOfValue()(val), key(y));
    z = create_node(val);

    // no x != 0
    if (y == header || x != nullptr|| key_compare(KeyOfValue()(val), key(y))) {
      // when y is header (tree is empty) 
      // or ??? 
      // or key of val is smaller than key of y. 
      left(y) = z; // insert into the left of y
      if (y == header) {
        root() = z;
        right_most() = z;
        // no need to set left_most();
      } else if (y == left_most()) {
        left_most() = z;
      }
    } else {
      right(y) = z;
      if (y == right_most()) {
        right_most() = z;
      }
    }

    parent(z) = y;
    left(z) = nullptr;
    right(z) = nullptr;

    __rb_tree_insert_rebalance(z, header->parent);

    ++node_count;
    return iterator(z);
  }
  // 
  link_type __move(link_type x, link_type cur) {
    if (x->parent != header) {
      if (cur->parent->left == cur) {
        cur->parent->left = cur->right;
      } else {
        cur->parent->right = cur->right;
      }

      cur->parent = x->parent;
      if (x->parent->left == x) {
        x->parent->left = cur;
      } else {
        x->parent->right = cur;
      }
    } else {
      if (cur->parent->left == cur) {
        cur->parent->left = cur->right;
      } else {
        cur->parent->right = cur->right;
      }

      cur->parent = x->parent;
      if (x == x->parent->left) {
        x->parent->left = cur;
      } else if (x == x->parent->right) {
        x->parent->right = cur;
      }
    }

    value_type* tmp = x->value_field;
    x->value_field = cur->value_field;
    cur->value_field = tmp;

    cur->left = x->left;
    cur->right = x->right;
  }
  // erase the x node without rebalancing
  link_type __erase(base_ptr x) {
    base_ptr child, parent;
    color_type color;

    if (x->left != nullptr && x->right != nullptr) {
      base_ptr successor = __rb_tree_node_base::minimum(x->right);
      if (x != root()) {
        if (x->parent->left == x) {
          x->parent->left = successor;
        } else {
          x->parent->right = successor;
        }
      } else {
        //x->parent->parent = successor;
        root() = (link_type) successor;
      }

      // save successor's right child, parent, and color
      child = successor->right;
      parent = successor->parent;
      color = successor->color;

      if (parent == x) {
        // successor is the right child of x
        parent = successor;
        // no need to set right child
      } else {
        if (child) 
          child->parent = parent;
        parent->left = child;

        successor->right = x->right;
        x->right->parent = successor;
      }

      successor->parent = x->parent;
      successor->left = x->left;
      successor->left->parent = successor;
      successor->color = x->color;

      // color is black => trigger rebalance
      if (color == __rb_tree_black) 
        __rb_tree_erase_rebalance(child, parent, (base_ptr&)root());
      
      destroy_node((link_type)x);

      return (link_type)successor;
    }

    color = x->color;
    parent = x->parent;
    // x has only one child
    if (x->left != nullptr) 
      child = x->left;
    else 
      child = x->right;

    if (child)
      child->parent = parent;

    if (x != root()) {
      if (parent->left == x) {
        parent->left = child;
      } else {
        parent->right = child;
      }
    } else {
      header->parent = child;
    }

    if (color == __rb_tree_black) 
      __rb_tree_erase_rebalance(child, parent, (base_ptr&)root());

    destroy_node((link_type) x);

    return (link_type) child;
  }
  // init the rb tree
  void init() {
    header = get_node();
    color(header) = __rb_tree_red;
    root() = nullptr;
    left_most() = header;
    right_most() = header;
  }

public:
  /********* De-constructor *********/
  rb_tree(const Compare& comp = Compare()): node_count(0), key_compare(comp) { 
    init(); 
  }
  ~rb_tree() {
    clear();
    put_node(header);
  }

  /********* Accessors *********/
  Compare key_comp() const { return key_compare;}
  iterator begin() const { return left_most(); }
  iterator end() const { return header; }
  bool empty() const { return node_count == 0; }
  size_type size() const { return node_count; }
  size_type max_size() const { return size_type(-1); }

  iterator find(const Key& val);

  /********* Modifiers *********/
  std::pair<iterator, bool> insert_unique(const value_type& val);
  iterator insert_equal(const value_type& val);
  template<class InputIterator>
  void insert_unique(InputIterator first, InputIterator last);
  template<class InputIterator>
  void insert_equal(InputIterator first, InputIterator last);
  iterator erase(iterator position);
  size_type erase(const Key& k);
  iterator erase(iterator first, iterator last);
  void clear();
};

/**
 * @brief Find the value mapped from the given key
 * 
 * @tparam Key - type
 * @tparam Value - type
 * @tparam KeyOfValue - construct key from value
 * @tparam Compare - comparator for the key
 * @tparam Alloc - allocator type
 * @param k - the given key
 * @return rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator 
 *  - to the mapped element (if not found, end() is returned)
 */
template <class Key, class Value, class KeyOfValue, 
  class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator 
  rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::find(const Key& k) {
  link_type y = header;
  link_type x = root();
  while (x != 0) {
    if (key_compare(k, key(x))) {
      x = left(x);
    } else if (key_compare(key(x), k)) {
      x = right(x);
    } else {
      return iterator(x);
    }
  }

  return end();
}

/**
 * @brief insert an element of value val into the rb tree (with equal keys allowed)
 * 
 * @tparam Key - type of the key
 * @tparam Value - type of the value
 * @tparam KeyOfValue - a functor returns key of the value
 * @tparam Compare - compare function type
 * @tparam Alloc - allocator type
 * @param val - value of the inserted element
 * @return rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator - to 
 *  the inserted element
 */
template <class Key, class Value, class KeyOfValue, 
  class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
  rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_equal(const value_type& val) {
  
  link_type y = header;
  link_type x = root();
  while(x != nullptr) {
    y = x;
    x = key_compare(KeyOfValue()(val), key(x)) ? left(x) : right(x);
  }

  return __insert(x, y, val);
}

/**
 * @brief insert unique elements in a given range [first, last)
 * 
 * @tparam Key - type
 * @tparam Value - type
 * @tparam KeyOfValue - construct key from value
 * @tparam Compare - comparator for the key
 * @tparam Alloc - allocator type
 * @tparam InputIterator - iterator type of the range
 * @param first - start of the range
 * @param last - end of the range
 */
template <class Key, class Value, class KeyOfValue, 
  class Compare, class Alloc>
template<class InputIterator>
void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_equal(InputIterator first, InputIterator last) {
  while (first != last) {
    insert_equal(*first);
    ++first;
  }
}

/**
 * @brief insert an element of value val into the rb tree (with equal keys 
 *  not allowed)
 * 
 * @tparam Key - type of the key
 * @tparam Value - type of the value
 * @tparam KeyOfValue - a functor returns key of the value
 * @tparam Compare - compare function type
 * @tparam Alloc - allocator type
 * @param val - value of the inserted element
 * @return std::<iterator, bool> - a pair of iterator and a bool var denoting 
 *  whether the inserted value is unique (or successfully inserted)
 */
template <class Key, class Value, class KeyOfValue, 
  class Compare, class Alloc>
std:: pair<typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator, bool>
  rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(const value_type& val) {
  link_type y = header;
  link_type x = root();
  bool comp = true;
  while(x != nullptr) {
    y = x;
    comp = key_compare(KeyOfValue()(val), key(x));
    x = comp ? left(x) : right(x);
  } // y would be the parent of x

  // the code below is necessary (because the comparison is < )!
  iterator j = iterator(y);
  if (comp) { // x is smaller than y
    if (j == begin()) {
      return std::pair<iterator, bool>(__insert(x, y, val), true);
    } else 
      --j; // there is the possiblity that this element has the same key as val
  }
  if (key_compare(key(j.node), KeyOfValue()(val))) // x is larger than y
    return std::pair<iterator, bool>(__insert(x, y, val), true);

  // x is y
  return std::pair<iterator, bool>(j, false);
}

/**
 * @brief insert all elements in a given range [first, last)
 * 
 * @tparam Key - type
 * @tparam Value - type
 * @tparam KeyOfValue - construct key from value
 * @tparam Compare - comparator for the key
 * @tparam Alloc - allocator type
 * @tparam InputIterator - iterator type of the range
 * @param first - start of the range
 * @param last - end of the range
 */
template <class Key, class Value, class KeyOfValue, 
  class Compare, class Alloc>
template<class InputIterator>
void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(InputIterator first, InputIterator last){
  while (first != last) {
    insert_unique(*first);
    ++first;
  }
}

/**
 * @brief erase the element pointed by the given iterator
 * 
 * @tparam Key - type of the key
 * @tparam Value - type of the value
 * @tparam KeyOfValue - a functor returns key of the value
 * @tparam Compare - compare function type
 * @tparam Alloc - allocator type
 * @param position - iterator to the element erased
 * @return rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator 
 *  - to the next element to the erased element
 */
template <class Key, class Value, class KeyOfValue, 
  class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator 
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::erase(
  rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator position) {
  link_type cur = (link_type) position.node;
  --node_count;
  iterator tmp;
  if (left_most() == position.node) {
    tmp = position;
    if (node_count == 0)
      left_most() = header;
    else {
      ++tmp;
      left_most() = (link_type) tmp.node;
    }
  }
  if (right_most() == position.node) {
    tmp = position;
    if (node_count == 0) 
      right_most() = header;
    else  {
      --tmp;
      right_most() = (link_type) tmp.node;
    }
  }

  return iterator(__erase(cur));
}

/**
 * @brief erase the element with the given key
 * 
 * @tparam Key - type of the key
 * @tparam Value - type of the value
 * @tparam KeyOfValue - a functor returns key of the value
 * @tparam Compare - compare function type
 * @tparam Alloc - allocator type
 * @param k - the given key
 * @return rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::size_type 
 *  - to the next element to the erased element
 */
template <class Key, class Value, class KeyOfValue, 
  class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::size_type 
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::erase(const Key& k) {
  iterator position = find(k);
  if (position == end())
    return 0;
  erase(position);
  return 1;
}

/**
 * @brief erase elements in a given range [first, last)
 * 
 * @tparam Key - type of the key
 * @tparam Value - type of the value
 * @tparam KeyOfValue - a functor returns key of the value
 * @tparam Compare - compare function type
 * @tparam Alloc - allocator type
 * @param first - the start of the range
 * @param last - the end of the range
 * @return rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
 *  - last
 */
template <class Key, class Value, class KeyOfValue, 
  class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator 
  rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::erase(
  rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator first, 
  rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator last) {
  if (first == begin() && last == end()) 
    clear();

  while (first != last) {
    erase(first);
    ++first;
  }

  return last;
}

/**
 * @brief clear the tree
 * 
 * @tparam Key - type of the key
 * @tparam Value - type of the value
 * @tparam KeyOfValue - a functor returns key of the value
 * @tparam Compare - compare function type
 * @tparam Alloc - allocator type
 */
template <class Key, class Value, class KeyOfValue, 
  class Compare, class Alloc>
void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::clear() {
  sup::stack<base_ptr> stk; 
  base_ptr cur = header->parent;
  base_ptr pre = nullptr;
  while(cur != nullptr || !stk.empty()) {
    while( cur != nullptr) {
      stk.push(cur);
      cur = cur->left;
    }

    cur = stk.top();
    if (cur->right == nullptr || cur->right == pre) {
      sup::_destroy(&((link_type)cur)->value_field);
      rb_tree_node_allocator::deallocate((link_type)cur, 1);
      stk.pop();
      pre = cur;
      cur = nullptr;
    } else {
      cur = cur->right;
    }
  }

  root() = nullptr;
  left_most() = header;
  right_most() = header;
}

}

#endif