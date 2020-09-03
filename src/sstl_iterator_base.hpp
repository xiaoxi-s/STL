#ifndef _SSTL_ITERATOR_BASE_H
#define _SSTL_ITERATOR_BASE_H

#include <cstddef>

/**
 * @author Xiaoxi Sun
 **/

/**
 * Concepts:
 *  Iterator Traits & Extraction
 *
 *  Iterator Types
 **/

/**
 * Questions:
 *
 *  Specialized __iterator_traits (for C++11 and newer versions) of type
 *  __void_t is not written here
 *
 *  Functions for returning base type for iterators
 **/

namespace sup {

///  Marking differnt types of iterators.
struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

// Mark the signatures of a iterator
// this is used for user-defined iterators
template <class Category, class T, class Distance = ptrdiff_t,
          class Pointer = T*, class Reference = T&>
class iterator {
 public:
  typedef Category iterator_category;
  typedef T value_type;
  typedef Distance difference_type;
  typedef Pointer pointer;
  typedef Reference referece;
};

// for C++11, there is the definition related with __void_t.
// Here just define iterator_tratis.
//
// This is for extraction. When use the traits in an iterator,
// use iterator_traits<Iterator>::xx to extract.
template <class Iterator>
class iterator_traits {
 public:
  typedef typename Iterator::iterator_category iterator_category;
  typedef typename Iterator::value_type value_type;
  typedef typename Iterator::difference_type difference_type;
  typedef typename Iterator::pointer pointer;
  typedef typename Iterator::reference referece;
};

// Partial specialization of pointer types
template <class T>
class iterator_traits<T*> {
 public:
  typedef random_access_iterator_tag iterator_category;
  typedef T value_type;
  typedef ptrdiff_t difference_type;
  typedef T* pointer;
  typedef T& referece;
};
template <class T>
class iterator_traits<const T*> {
 public:
  typedef random_access_iterator_tag iterator_category;
  typedef T value_type;
  typedef ptrdiff_t difference_type;
  typedef const T* pointer;
  typedef const T& referece;
};

/******** some helper functions for easier extraction ********/

template <class Iterator>
inline typename iterator_traits<Iterator>::iterator_category iterator_category(
    const Iterator&) {
  typedef typename iterator_traits<Iterator>::iterator_category category;
  return category();  // return a struct; so we need to construct an object
}

template <class Iterator>
inline typename iterator_traits<Iterator>::difference_type* distance_type(
    const Iterator&) {
  return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
}

template <class Iterator>
inline typename iterator_traits<Iterator>::value_types* value_type(
    const Iterator&) {
  return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
}

/******** distance() & advance() function ********/
// These functions are listsed here for convenience.
// STL source file separates the types and functions for iterators

// distance function
template <class InputIterator>
inline typename iterator_traits<InputIterator>::difference_type __distance(
    InputIterator first, InputIterator last, input_iterator_tag) {
  typename iterator_traits<InputIterator>::difference_type n = 0;
  while (first != last) {
    ++first, ++n;
  }
  return n;
}

template <class RandomAccessIterator>
inline typename iterator_traits<RandomAccessIterator>::difference_type
__distance(RandomAccessIterator first, RandomAccessIterator last,
           random_access_iterator_tag) {
  return last - first;
}

// The name should be the name at the lowest level
template <class InputIterator>
inline typename iterator_traits<InputIterator>::difference_type distance(
    InputIterator first, InputIterator last) {
  typedef typename iterator_traits<InputIterator>::iterator_category category;
  return __distance(first, last, category());
}

// advance function
template <class InputIterator, class Distance>
inline void __advance(InputIterator& i, Distance n, input_iterator_tag) {
  while (--n) ++i;
}

template <class ForwardIterator, class Distance>
inline void __advance(ForwardIterator& i, Distance n, forward_iterator_tag) {
  advance(i, n, input_iterator_tag());
}

template <class BidirectionalIterator, class Distance>
inline void __advance(BidirectionalIterator& i, Distance n, bidirectional_iterator_tag) {
  if (n >= 0) 
    while (n--) ++i;
  else 
    while (n++) --i;
}

template <class RandomAccessIterator, class Distance>
inline void __advance(RandomAccessIterator& i, Distance n, random_access_iterator_tag) {
  i += n;
}

template <class InputIterator, class Distance>
inline void advance(
    InputIterator &i, Distance n) {
  __advance(i, n, iterator_category(i));
}

// there are functions for returning base type for iterators
// for now, the meaning is unclear.
// will be added if the functions are used.

}  // namespace sup

#endif