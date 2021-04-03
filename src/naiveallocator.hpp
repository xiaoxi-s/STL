#ifndef _NAIVEALLOC_H
#define _NAIVEALLOC_H

#include <climits>
#include <cstddef>  // ptrdiff_t size_t
#include <cstdlib>
#include <iostream>
#include <new>  // for placement new

/**
 * @author Xiaoxi Sun
 **/
namespace sup {

class naive_allocator {
 public:

  static void* allocate(size_t n, const void* hint = 0) {
    return (void*) new char[n];
  }

  static void deallocate(char* p, size_t n) { delete [] p; }

  static void destory(void* p) { ; }

  static size_t max_size() {
      return size_t(-1);
  }
};

}  // namespace sup

#endif