#ifndef _SSTL_ALLOCATOR_BASE_H
#define _SSTL_ALLOCATOR_BASE_H

/**
 * @author Xiaoxi Sun
 **/

#include <new>

/**
 * Concepts
 *
 * operator new and new; overload of those two "things"
 *
 * alignment issues; the alignof keyword
 *
 * allocator interface
 *
 * allocator stucture in STL of SGI.
 *
 * std::forward
 *
 * noexcept: The noexcept-specification is a part of the function type and may
 * appear as part of any function declarator.
 *
 * max_size():
 *   __PTRDIFF_MAX__ to ptrdiff_t is similar to INT_MAX to int
 *
 * std::__throw_bad_alloc():
 *   Defined in <functexcept.h>. Throw an bad_alloc exception. This exception
 *   demonstrates failure of using new to allocate space.
 *
 * __cpp_aligned_new:
 *   This is a feature test Macro introduced since C++17.
 *
 * rebind():
 *   The container might have the need of allcoating space for different types
 **/

/**
 * Question:
 * 
 * Constructors:
 *   What are _GLIBCXX20_CONSTEXPR and _GLIBCXX_USE_NOEXCEPT for?
 *
 * Construct & destroy:
 *   Not sure about what the version control is doing
 *   (std::forward in construct & destroy)
 **/

namespace sup {

/**
 * This is a C++ standard allocator.
 *  - allocation calls operator new
 *  - release calls operator delete
 *
 * The base class for allocators in STL. Situations that are not
 * considered are:
 *  - multi-threading
 *  - alignment
 *
 * @tparam T type of the allocated object.
 **/
template <class T>
class allocator_base {
 public:
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef T* pointer;
  typedef const T* const_pointer;
  typedef T& reference;
  typedef const T& const_reference;
  typedef T value_type;

  // The container might have the need of allcoating
  // space for different types
  template <class T2>
  struct rebind {
    typedef allocator_base<T2> other;
  };

  /**
   * Several constructors.
   *
   * Notice that the source code of SGI does not specify some of the
   * type parameter (<T>). But I specify all of them for clarity.
   **/
  allocator_base<T>() {}
  allocator_base<T>(const allocator_base<T>&) {}

  template <class T1>
  allocator_base(const allocator_base<T1>&) {}

  ~allocator_base<T>() {}

  /**
   * Return the address of x
   *
   * @param x - the address being asked
   * @return &x
   *
   * Macros used here:
   *  - _GLIBCXX_NOEXCEPT
   **/
  static pointer address(reference x) { return std::__addressof(x); }

  static const_pointer address(const_reference x) { return std::__addressof(x); }

  /**
   * Allocation function. This is what you are looking for!
   *
   * @param n - number of elements
   *
   * Macros used here:
   *  - __cpp_aligned_new
   **/
  static pointer allocate(size_type n, const void* = static_cast<const void*>(0)) {
    if (n > allocator_base<T>::max_size())
      std::__throw_bad_alloc();  // this is an interesting function. study later

    // C++ standard does not specify this. Return nullptr for completeness
    if (n == 0) return nullptr;

    // There should be a branch dealing with alignment issues

    return static_cast<T*>(::operator new(n * sizeof(T)));
  }

  /**
   * p should not be a null pointer
   *
   * @param p - pointing to the space that would be deallocated
   * @param size_type - I do not know why
   **/
  static void deallocate(pointer p, size_type) {
    // there should be some alignment issues

    ::operator delete(p);
  }

  /**
   * @return the maximum allocable size
   * __PTRDIFF_MAX__
   **/
  static size_type max_size() {
    // Might use __PTRDIFF_MAX
    return size_t(-1) / sizeof(T);
  }

  // For C++ 11 and later versions, STL uses std::forward.
  //

  // Here is the simmplified version:
  static void construct(pointer p, T val) { ::new ((void*)p) T(val); }

  static void destroy(pointer p) { p->~T(); }

  /**
   * Comparator
   **/
  template <class T1>
  friend bool operator==(const allocator_base&, const allocator_base<T1>&) {
    return true;
  }
  template <class T1>
  friend bool operator!=(const allocator_base&, const allocator_base<T1>&) {
    return false;
  }
};

}  // namespace sup
#endif