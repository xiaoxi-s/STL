#ifndef _SSTL_CONSTRUCT_H
#define _SSTL_CONSTRUCT_H
/**
 * Partial credits to STL Annotated Sources by Jie Hou.
 * Partial credits to STL sources.
 *
 * @author Xiaoxi Sun
 **/

#include <new>  // to use placement new
#include <ext/alloc_traits.h>

/**
 * Concepts
 *
 * template with specialized type: bool & specialize with true or false
 *
 * use struct like a namespace
 *
 * destroy: __cplusplus >= 201103L has an assertion about "is_destructible"
 * 
 * iterator_traits - defined in std namespace. Remember this!  
 **/

/**
 * Questions:
 * 
 * None
 **/

namespace sup {

/**
 * Constructor bsed on type and value
 *
 * @param T1
 * @param T2
 **/
template <class T1, class T2>
inline void _Construct(T1* p, const T2& value) {
  new (p) T1(value);  // placement new
}

/******** destroy() ********/
// Three types: single type, single type + size, range
// _Destory(T*); _Destory(ForwardIterator, ForwardIterator);
// _Destroy(ForwardIterator, SizeType)

/******** 1. Destroy based on only a pointer. ********/
/**
 * Destroy the object pointed by pointer
 * 
 * @param pointer the pointer to the object being destroyed
 **/
template <class T>
inline void _Destroy(T* pointer) {
  pointer->~T();
}

/******** 2. Destroy based on range ********/

/**
 * Start point of the destroy based on range
 *
 * __cplusplus >= 201103L has an assertion
 **/
template <class ForwardIterator>
inline void _Destroy(ForwardIterator first, ForwardIterator last) {
  // Get the value type
  typedef typename std::iterator_traits<ForwardIterator>::value_type _value_type;

  // c++11 would assert that whether the type is destructible;
  // if so, and assert error is raised.

  // destroy accordingly.
  sup::_DestroyAux<__has_trivial_destructor(_value_type)>::__destory(first,
                                                                     last);
}

/**
 * Auxiliary function for destroy based on range [first, last)
 * with non-trivial destructor
 **/
template <bool>
struct _DestroyAux {
  template <class ForwardIterator>
  static void __destroy(ForwardIterator first, ForwardIterator last) {
    for (; first != last; ++first) {
      sup::_Destroy(std::addressof(*first));
    }
  }
};
// with trivial destructor
template <>
struct _DestroyAux<true> {
  template <class ForwardIterator>
  static void __destroy(ForwardIterator first, ForwardIterator last) {}
};

/******** 3. Destory based on a pointer and number of elements ********/

/**
 * Start point of the destroy based on pointer and size
 *
 * c++ 11 static assert
 **/
template <class ForwardIterator, class SizeType>
inline ForwardIterator _Destroy_n(ForwardIterator first, SizeType n) {
  // get the value type
  typedef typename std::iterator_traits<ForwardIterator>::value_type _value_type;

  // static assert in c++ 11
  return sup::_DestroyNAux<__has_trivial_destructor(_value_type)>::__destory_n(first, n);
}

// with non-trivial destructor
template <bool>
struct _DestroyNAux {
  template <class ForwardIterator, class SizeType>
  static ForwardIterator __destroy_n(ForwardIterator first, SizeType n) {
    for (; n > 0; ++first, --n) {
      sup::_Destroy(std::addressof(*first));
    }
    return first;
  }
};

// (specialized template) with trivial destructor
template <>
struct _DestroyNAux<true> {
  template <class ForwardIterator, class SizeType>
  static ForwardIterator __destroy_n(ForwardIterator first, SizeType n) {
    std::advance(first, n);
    return first;
  }
};

// specialized destroy for char and wchar_t
inline void _Destroy(char*, char*) {}
// wide character for unicode encoding
inline void _Destroy(wchar_t*, wchar_t*) {}

}  // namespace sup
#endif