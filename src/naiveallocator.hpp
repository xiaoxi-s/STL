#ifndef _NAIVEALLOC_H
#define _NAIVEALLOC_H

#include <climits>
#include <cstddef>  // ptrdiff_t size_t
#include <cstdlib>
#include <iostream>
#include <new>  // for placement new

/**
 * All credits for STL Annotated Sources by Jie Hou.
 * 
 * @author Xiaoxi Sun
 **/
namespace sup {

template <class T>
inline T* _allocate(ptrdiff_t size, T*) {
  set_new_handler(0);
  T* tmp = (T*)(new (size_t)(size * sizeof(T)));

  if (T == nullptr) {
    cerr << "out of memory" << endl;
    exit(1);
  }

  return tmp;
}

template <class T>
inline void _deallocate(T* buffer) {
  delete buffer;
}

template <class T1, class T2>
inline void _construct(T1* p, const T2& value) {
  new (p) T1(value);
}

template <class T>
inline void _destory(T* ptr) {
  ptr->~T();
}

template <class T>
class naive_allocator {
 public:
  typedef T value_type;
  typedef T* pointer;
  typedef const T* const_pointer;
  typedef T& reference;
  typedef const T& const_reference;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;

  // rebind allocator of type U
  template <class U>
  struct rebind {
    typedef allocator<U> other;
  }

  pointer
  allocate(size_type n, const void* hint = 0) {
    return _allocate((difference_type)n, (pointer) nullptr);
  }

  void deallocate(pointer p, size_type n) { _deallocate(p); }

  void destory(pointer p) { _destory(p); }

  pointer address(reference x) { return (pointer)&x; }

  const_pointer const_address(const_reference x) {
    return (const_pointer)&x;
  }

  size_type max_size() const {
      return size_type(UINT_MAX/sizeof(T));
  }
};

}  // namespace sup

#endif