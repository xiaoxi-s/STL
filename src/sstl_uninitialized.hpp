#ifndef _SSTL_UNINITIALIZED_H
#define _SSTL_UNINITIALIZED_H

// after c++11 (inclusive) will have this header

#include "sstl_type_tratis.hpp"
#include "sstl_construct.hpp"
#include "sstl_iterator_base.hpp"

/**
 * Credits to STL source code from GNU
 *
 * @author Xiaoxi Sun
 **/

/**
 * Concepts:
 *
 *  Behaviors of "rollback or commit".
 *
 *  try-catch that would not be optimized: __try __catch
 **/

/**
 * Questions:
 *  type_traits is included after c++11
 *
 *  value_type(), __type_traits, __true_type, __false_type,
 *  is_pod would be introduced later
 *
 *  std::__addressof is used here for brevity.
 *  std::copy is used in __uninitialized_copy.
 *  std::fill
 *  std::fill_n
 *
 *  uninitialized_copy() is simplified in that
 *  is_assignable is used in uninitialized_copy() after c++11 (inclusive)
 *  std::iterator_traits<> is used here.
 *  is_trivial()
 *
 *  __throw_exception_again ????
 *
 *  why (void) ++curr ????
 **/

namespace sup {

/******** Initialize Based on Range ********/

/**
 * Notice the rollback or commit behavior.
 **/
template <bool TrivialValueTypes>
struct __uninitialized_copy {
  template <class InputIterator, class ForwardIterator>
  static ForwardIterator _uninit_copy(InputIterator first, InputIterator last,
                                      ForwardIterator result) {
    ForwardIterator curr = result;
    // try-catch that would not be optimized
    __try {
      while (first != last) {
        sup::_construct(std::__addressof(result), *first);
        ++first;
        ++result;
      }
      return curr;
    }
    __catch(...) {
      sup::_destroy(curr, result);
      __throw_exception_again;
    }
  }
};
// Is trivial value types: We do not need to call constructors
template <>
struct __uninitialized_copy<true> {
  template <class InputIterator, class ForwardIterator>
  static ForwardIterator _uninit_copy(InputIterator first, InputIterator last,
                                      ForwardIterator result) {
    // where <algorithm> is used.
    return std::copy(first, last, result);
  }
};

/**
 * Initialize (construct) n elements T starting from first.
 *
 * @tparam InputIterator - source iterator
 * @tparam ForwardIterator - destination iterator
 * @param first - starting position of initialization
 * @param last - end position
 * @param result - the starting place to be initialized
 *
 * @return the iterator pointing to the last element if success; otherwise the
 *  iterator pointing to the first element that is not successfully unitialized.
 **/
template <class InputIterator, class ForwardIterator>
inline ForwardIterator uninitialized_copy(InputIterator first,
                                          InputIterator last,
                                          ForwardIterator result) {
  typedef typename sup::iterator_traits<InputIterator>::value_type _value_type1;
  typedef
      typename sup::iterator_traits<ForwardIterator>::value_type _value_type2;

  // similified here. There would be more code after c++11 (inclusive)

  return sup::__uninitialized_copy<(
      __is_trivial(_value_type1) &&
      __is_trivial(_value_type2))>::_uninit_copy(first, last, result);
}

/******** Initialize Based on Range and a Value ********/

// non-trivial value type
template <bool TrivialValueType>
struct __uninitialized_fill {
  template <class ForwardIterator, class T>
  static void __uninit_fill(ForwardIterator first, ForwardIterator last,
                            const T& value) {
    ForwardIterator curr = first;
    __try {
      for (; curr != last; ++curr) {
        sup::_construct(std::__addressof(*curr), value);
      }
    }
    __catch(...) {
      sup::_destroy(first, curr);
      __throw_exception_again;
    }
  }
};
// trivial value type
template <>
struct __uninitialized_fill<true> {
  template <class ForwardIterator, class T>
  static void __uninit_fill(ForwardIterator first, ForwardIterator last,
                            const T& value) {
    std::fill(first, last, value);
  }
};

/**
 * Initialize based on range [first, last) and a single value.
 *
 * @tparam ForwardIterator - for iteration
 * @tparam T - value type
 * @param first - start position
 * @param last - end position
 * @param value - the const value
 *
 * @return the iterator pointing to the last element if success; otherwise the
 *  iterator pointing to the first element that is not successfully unitialized.
 **/
template <class ForwardIterator, class T>
inline void uninitialized_fill(ForwardIterator first, ForwardIterator last,
                               const T& value) {
  typedef
      typename sup::iterator_traits<ForwardIterator>::value_type _value_type;

  // simiplified here There would be more code after c++11 (inclusive)

  sup::__uninitialized_fill<__is_trivial(_value_type)>::uninit_fill(first, last,
                                                                    value);
}

/******** Initialize Based on Number of Elements ********/

// nont-trivial value type
template <bool TrivialValueType>
struct __uninitialized_fill_n {
  template <class ForwardIterator, class Size, class T>
  static ForwardIterator __uninit_fill_n(ForwardIterator first, Size n,
                                         const T& value) {
    ForwardIterator curr = first;
    __try {
      // why (void) ++curr
      for (; n > 0; --n, ++curr) {
        sup::_construct(std::__addressof(*first, value));
      }
    }
    __catch(...) {
      sup::_destroy_n(first, n);
      __throw_exception_again;
    }
  }
};
// trivial value type
template <>
struct __uninitialized_fill_n<true> {
  template <class ForwardIterator, class Size, class T>
  static ForwardIterator __uninit_fill_n(ForwardIterator first, Size n,
                                         const T& value) {
    return std::fill_n(first, n, value);
  }
};

/**
 * Initialize (construct) n elements T starting from first.
 *
 * @tparam ForwardIterator - iterator type
 * @tparam Size - size type
 * @tparam T - value tye
 * @param first - starting position of initialization
 * @param n - number of elements
 * @param value - the value
 *
 * @return the last element
 **/
template <class ForwardIterator, class Size, class T>
inline ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n,
                                            const T& value) {
  typedef
      typename sup::iterator_traits<ForwardIterator>::value_type _value_type;

  // simplified here

  return sup::__uninitialized_fill_n<__is_trivial(
      _value_type)>::__uninit_fill_n(first, n, value);
}

}  // namespace sup

#endif