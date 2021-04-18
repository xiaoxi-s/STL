#ifndef _SSTL_ALLOCATOR_H
#define _SSTL_ALLOCATOR_H

#include "sstl_allocator_base.hpp"
#include "naiveallocator.hpp"

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

class naive_allocator;

#ifdef __SSTL_USE_MALLOC
#  define __malloc_allocator alloc
#else
  typedef naive_allocator alloc;
#endif

template <class T, class Alloc>
class simple_alloc;

template <class T, class Alloc=naive_allocator>
class simple_alloc {
public:
  typedef T value_type;
  typedef size_t size_type;

  // rebind for other usage.
  template <class T2, class Alloc2>
  struct rebind {
    typedef simple_alloc<T2, Alloc2> other;
  };

  /**
   * Constructors
   *
   * BConstructors use _GLIBCXX20_CONSTEXPR and _GLIBCXX_NOTHROW
   **/
  simple_alloc() {}
  simple_alloc(const simple_alloc& a) {}

  // can be copy constructed by other types of allocators...
  // although there might not be one...
  template <class T2, class Alloc2>
  simple_alloc(const simple_alloc<T2, Alloc2> a) {}

  ~simple_alloc() {}

  static T* allocate(const size_t n) {
    return n == 0 ? nullptr : (T*) Alloc::allocate(n * sizeof(T));
  }

  static T* allocate() {
    return (T*) Alloc::allocate(sizeof(T));
  }

  static void deallocate(const T* p, const size_type n) {
    Alloc::deallocate((char*) p, n*sizeof(T));
  }

  static void deallocate(const T* p) {
    Alloc::deallocate((char*) p, sizeof(T));
  }
  // Operator = overload here since C++11:
  // allocator interface requirements
  friend bool operator==(const simple_alloc&, const simple_alloc&) { return true; }
  friend bool operator!=(const simple_alloc&, const simple_alloc&) { return false; }

  // inherit everthing else (! from GNU sources code)
};

// operator== interfaces
template <class T1, class Alloc1, class T2, class Alloc2>
bool operator==(const simple_alloc<T1, Alloc1>&, const simple_alloc<T2, Alloc2>&) {
  return true;
}
template <class T1, class Alloc1, class T2, class Alloc2>
bool operator!=(const simple_alloc<T1, Alloc1>&, const simple_alloc<T2, Alloc2>&) {
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