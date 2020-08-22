#ifndef _SSTL_ALLOCATOR_BASE_H
#define _SSTL_ALLOCATOR_BASE_H

#include <new>

/**
 * Basic concepts in this part
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
 **/

/**
 * Question lists:
 *
 *   What rebind() is used for?
 *
 * Constructors:
 *   What are _GLIBCXX20_CONSTEXPR and _GLIBCXX_USE_NOEXCEPT for?
 *
 * allocate():
 *   std::__throw_bad_alloc()
 *
 * deallocate():
 *   __cpp_aligned_new Macro
 *
 * max_size():
 *   __PTRDIFF_MAX__ // it seems that the macro is defined in stdint.h
 *
 * Construct & Destory:
 *   Not sure about what the version control is doing
 *   (std::forward in construct & destory)
 *
 **/

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

namespace sup {

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

  // so far, not sure about the function of the struct below
  template <class T1>
  struct rebind {
    typedef allocator_base<T1> other;
  };

  /**
   * Several constructors.
   *
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
  pointer address(reference x) const { return std::__addressof(x); }

  const_pointer address(const_reference x) const { return std::__addressof(x); }

  /**
   * Allocation function. This is what you are looking for!
   *
   * @param n - number of bytes
   *
   * Macros used here:
   *  - __cpp_aligned_new
   **/
  pointer allocate(size_type n, const void* = static_cast<const void*>(0)) {
    if (n > this->max_size())
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
  void deallocate(pointer p, size_type) {
    // there should be some alignment issues

    ::operator delete(p);
  }

  /**
   * @return the maximum allocable size
   * __PTRDIFF_MAX__
   **/
  size_type max_size() const {
    // Might use __PTRDIFF_MAX
    return size_t(-1) / sizeof(T);
  }

  // For C++ 11 and later versions, STL uses std::forward.
  //

  // Here is the simmplified version:
  void construct(pointer p, T val) { ::new ((void*)p) T(val); }

  void destory(pointer p) { p->~T(); }

  /**
   * Comparator
   **/
  template <typename T1>
  friend bool operator==(const allocator_base&, const allocator_base<T1>&) {
    return true;
  }
  template <typename T1>
  friend bool operator!=(const allocator_base&, const allocator_base<T1>&) {
    return false;
  }
};

}  // namespace sup
#endif