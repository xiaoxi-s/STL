#ifndef _SSTL_CONSTRUCT_H
#define _SSTL_CONSTRUCT_H
#include <new>  // to use placement new

/**
 * All credits to STL Annotated Sources by Jie Hou.
 *
 * @author Xiaoxi Sun
 **/

namespace sup {

/**
 * Constructor bsed on type and value
 *
 * @param T1
 * @param T2
 **/
template <class T1, class T2>
inline void construct(T1* p, const T2& value) {
  new (p) T1(value);  // placement new
}

// two destory()
/**
 * Destory the object pointed by pointer
 *
 * @param pointer the pointer to the object being destoryed
 **/
template <class T>
inline void destory(T* pointer) {
  pointer->~T();
}

// specialized destory for char and wchar_t
inline void destory(char*, char*) {}
// wide character for unicode encoding
inline void destory(wchar_t*, wchar_t*) {}

}  // namespace sup
#endif