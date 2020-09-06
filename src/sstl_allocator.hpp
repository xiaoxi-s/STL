#ifndef _SSTL_ALLOCATOR_H
#define _SSTL_ALLOCATOR_H

#include "sstl_allocator_base.hpp"

/**
 * @author Xiaoxi Sun
 **/

/**
 * Concepts:
 *
 * Constructors:
 *  _GLIBCXX20_CONSTEXPR & constexpr - The constexpr specifier declares that it
 *   is possible to evaluate the value of the function or variable at compile
 *  time. A constexpr implies inline.
 *  from https://en.cppreference.com/w/cpp/language/constexpr
 *
 *  _GLIBCXX_NOTHROW & nothrow - This constant value is used as an argument for
 *   operator new and operator new[] to indicate that these functions shall not
 *   throw an exception on failure, but return a null pointer instead.
 *   from http://www.cplusplus.com/reference/new/nothrow/
 *
 *  Operator = overload:
 *   default keyword: explicitly-defaulted function definition: as an explicit
 *   instruction to the compiler to generate special member function or a
 *   comparison operator (since C++20) for a class.
 *   from https://en.cppreference.com/w/cpp/keyword/default
 *
 *  Template specification:
 *   volatile explained here:
 *   https://www.tutorialspoint.com/What-does-the-volatile-keyword-mean-in-Cplusplus
 *
 *  Swap:
 *   Use __is_empty() to determine whether a allocator is an empty type. This
 *   would save swapping time.
 **/

/**
 * Questions & Issues:
 *   allocator<void> is not complete
 * 
 *   shrink_to_fit is not implemented here, which was introduced since C++11. 
 **/

namespace sup {

template <class T>
class allocator;

// allocator specialize
template <>
class allocator<void> {
 public:
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef void* pointer;
  typedef const void* const_pointer;
  typedef void value_type;

  // rebind for allocating other types
  template <typename T>
  struct rebind {
    typedef allocator<T> other;
  };
  
  // construct & destory for allocator<void> need std::forward() to 
  // pass on type parameter? 
};

template <class T>
class allocator : public allocator_base<T> {
 public:
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef T* pointer;
  typedef const T* const_pointer;
  typedef T& reference;
  typedef const T& const_reference;
  typedef T value_type;

  // rebind for other usage.
  template <class T2>
  struct rebind {
    typedef allocator<T2> other;
  };

  /**
   * Constructors
   *
   * BConstructors use _GLIBCXX20_CONSTEXPR and _GLIBCXX_NOTHROW
   **/
  allocator() {}
  allocator(const allocator& a) : allocator_base<T>(a) {}

  // can be copy constructed by other types of allocators...
  // although there might not be one...
  template <class T2>
  allocator(const allocator<T2> a) {}

  ~allocator() {}

  // Operator = overload here since C++11:

  // allocator interface requirements
  friend bool operator==(const allocator&, const allocator&) { return true; }
  friend bool operator!=(const allocator&, const allocator&) { return false; }

  // inherit everthing else (! from GNU sources code)
};

// operator== interfaces
template <class T1, class T2>
bool operator==(const allocator<T1>&, const allocator<T2>&) {
  return true;
}
template <class T1, class T2>
bool operator!=(const allocator<T1>&, const allocator<T2>&) {
  return false;
}

// there are specialization of allocators
// even with volatile specifier of a template paramter.
// check
// https://www.tutorialspoint.com/What-does-the-volatile-keyword-mean-in-Cplusplus
// for explanation of the volatile keyword.

// Swap interface of allocators, where __is_empty() is used
// Note that the naming standard here is contradictary to it in
// sstl_construct in STL sources.
template <class Alloc, bool = __is_empty(Alloc)>
struct _alloc_swap {
  static void _swap(Alloc&, Alloc&) {}  // just the empty one
};
// specialize the non-empty one
template <class Alloc>
struct _alloc_swap<Alloc, false> {
  static void _swap(Alloc& first, Alloc& second) {
    if (first != second) swap(first, second);
  }
};

// Compare (not equal) interface of allocators.
template <class Alloc, bool = __is_empty(Alloc)>
struct _alloc_neq {
  // this is for improving efficiency related to stateless
  // allocators
  static bool _neq(Alloc&, Alloc&) { return false; }
};
// partial specialization for non-empty allocators
template <class Alloc>
struct _alloc_neq<Alloc, false> {
  static bool _neq(Alloc& first, Alloc& second) { return first != second; }
};

// There is a shrink_to_fit interface provided since C++11, which is 
// not shown here. 

}  // namespace sup

#endif