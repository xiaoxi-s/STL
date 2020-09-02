#ifndef _SSTL_ITERATOR_H
#define _SSTL_ITERATOR_H

#include "sstl_iterator_base.hpp"

/**
 * @author Xiaoxi Sun
 **/

/**
 * Concepts:
 *  decltype
 *
 **/

/**
 * Questions:
 *  - operator == overload for reverse_iterator after c++11
 *  - my implementation of reverse_iterator has an offset of 1 with the
 *  source reverse_iterator
 *  - Use of decltype for calculating difference of two reverse_iterators.
 *  - Iterator arithmatics for different reverse_iterators not implemented
 **/

// implementation of some fancy iterators e.g. reverse_iterators.
namespace sup {

/**
 * @tparam Iterator must be a random access iterator
 **/
template <typename Iterator>
class reverse_iterator
    : public iterator<typename iterator_traits<Iterator>::iterator_category,
                      typename iterator_traits<Iterator>::value_type,
                      typename iterator_traits<Iterator>::difference_type,
                      typename iterator_traits<Iterator>::pointer,
                      typename iterator_traits<Iterator>::reference> {
 protected:
  Iterator current;
  typedef iterator_traits<Iterator> traits_type;

 public:
  typedef Iterator iterator_type;
  typedef typename traits_type::difference_type difference_type;
  typedef typename traits_type::pointer pointer;
  typedef typename traits_type::reference reference;

  // Constructors and deconstructors
  reverse_iterator() : current() {}

  reverse_iterator(iterator_type x) : current(x) {}
  // if another template argument is able to be converted to Iterator,
  // copy construction is available.
  template<class Iterator2>
  reverse_iterator(reverse_iterator<Iterator2> it) : current(it.base()) {}
  // operator overload for versions >= c++11

  // for interal use
  iterator_type base() const { return current; }

  // need (--current) to be a valid iterator
  reference operator*() const {
    // do not know why source code use --.
    return *current;
  }

  pointer operator->() const {
    // do not know why source code uses --.
    return _to_pointer(current);
  }

  // for reverse_iterator, this is essentially pre --
  reverse_iterator& operator++() {
    --current;
    return *this;
  }

  // for reverse_iterator, this is essentially post --
  reverse_iterator operator++(int) {
    reverse_iterator temp(current);
    --current;
    return *temp;
  }

  // just do this in the reverse way
  reverse_iterator& operator--() {
    ++current;
    return *this;
  }

  // just do this in the reverse way
  reverse_iterator operator--(int) {
    reverse_iterator temp(current);
    ++current;
    return *temp;
  }

  // notice that this returns a copy
  // again, do it in the reverse way
  reverse_iterator operator+(difference_type n) {
    return reverse_iterator(current - n);
  }

  // again, do it in the reverse way
  reverse_iterator& operator+=(difference_type n) {
    current -= n;
    return *this;
  }

  // notice that this returns a copy
  // again, do it in the reverse way
  reverse_iterator operator-(difference_type n) {
    return reverse_iterator(current + n);
  }

  // again, do it in the reverse way
  reverse_iterator& operator-=(difference_type n) {
    current += n;
    return *this;
  }

  // do this in the reverse way
  reference operator[](difference_type n) {
    // source code use *(*this + n), which is the same
    return *(current - n);
  }

  // in order to be compatible with native pointers
  template <class T>
  static T* _to_pointer(T* p) {
    return p;
  }
  // Or, the parameter is also a iterator
  template<class T>
  static T* _to_pointer(T it) { return it.operator->(); }
};

// Comparison
template <class Iterator>
inline bool operator==(const reverse_iterator<Iterator>& __x,
                       const reverse_iterator<Iterator>& __y) {
  return __x.base() == __y.base();
}

template <class Iterator>
inline bool operator<(const reverse_iterator<Iterator>& __x,
                      const reverse_iterator<Iterator>& __y) {
  return __y.base() < __x.base();
}

template <class Iterator>
inline bool operator!=(const reverse_iterator<Iterator>& __x,
                       const reverse_iterator<Iterator>& __y) {
  return !(__x == __y);
}

template <class Iterator>
inline bool operator>(const reverse_iterator<Iterator>& __x,
                      const reverse_iterator<Iterator>& __y) {
  return __y < __x;
}

template <class Iterator>
inline bool operator<=(const reverse_iterator<Iterator>& __x,
                       const reverse_iterator<Iterator>& __y) {
  return !(__y < __x);
}

template <class Iterator>
inline bool operator>=(const reverse_iterator<Iterator>& __x,
                       const reverse_iterator<Iterator>& __y) {
  return !(__x < __y);
}

template <class IteratorL, class IteratorR>
inline bool operator==(const reverse_iterator<IteratorL>& __x,
                       const reverse_iterator<IteratorR>& __y) {
  return __x.base() == __y.base();
}

template <class IteratorL, class IteratorR>
inline bool operator<(const reverse_iterator<IteratorL>& __x,
                      const reverse_iterator<IteratorR>& __y) {
  return __y.base() < __x.base();
}

template <class IteratorL, class IteratorR>
inline bool operator!=(const reverse_iterator<IteratorL>& __x,
                       const reverse_iterator<IteratorR>& __y) {
  return !(__x == __y);
}

template <class IteratorL, class IteratorR>
inline bool operator>(const reverse_iterator<IteratorL>& __x,
                      const reverse_iterator<IteratorR>& __y) {
  return __y < __x;
}

template <class IteratorL, class IteratorR>
inline bool operator<=(const reverse_iterator<IteratorL>& __x,
                       const reverse_iterator<IteratorR>& __y) {
  return !(__y < __x);
}

template <class IteratorL, class IteratorR>
inline bool operator>=(const reverse_iterator<IteratorL>& __x,
                       const reverse_iterator<IteratorR>& __y) {
  return !(__x < __y);
}

// some arithmetics of reverse_iterator of different types. 
// write later

}  // namespace sup
#endif