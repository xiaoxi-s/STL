#ifndef _SSTL_ALLOCATOR_BASE_H
#define _SSTL_ALLOCATOR_BASE_H

#include <new>

/**
 * @author Xiaoxi Sun
 **/

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
 * @brief a wrapper for new and delete allocator 
 * 
 * @tparam threads 
 * @tparam inst 
 */
template <bool threads, int inst>
class __simple_default_allocator_template {
 public:
  // The container might have the need of allcoating
  // space for different types
  template <bool threads2, int inst2>
  struct rebind {
    typedef __simple_default_allocator_template<threads2, inst2> other;
  };

  /**
   * Several constructors.
   *
   * Notice that the source code of SGI does not specify some of the
   * type parameter (<T>). But I specify all of them for clarity.
   **/
  __simple_default_allocator_template<threads, inst>() {}
  __simple_default_allocator_template<threads, inst>(
    const __simple_default_allocator_template<threads, inst>&) {}

  template <bool threads2, int inst2>
  __simple_default_allocator_template(
    const __simple_default_allocator_template<threads2, inst2>&) {}

  ~__simple_default_allocator_template<threads, inst>() {}

  /**
   * Return the address of x
   *
   * @param x - the address being asked
   * @return &x
   *
   * Macros used here:
   *  - _GLIBCXX_NOEXCEPT
   **/

  /**
   * Allocation function. This is what you are looking for!
   *
   * @param n - number of bytes
   *
   * Macros used here:
   *  - __cpp_aligned_new
   **/
  static void* allocate(size_t n) {
    // C++ standard does not specify this. Return nullptr for completeness
    if (n == 0) return nullptr;

    // There should be a branch dealing with alignment issues

    return new char[n];
  }

  /**
   * p should not be a null pointer
   *
   * @param p - pointing to the space that would be deallocated
   * @param size_type - used for considering using first alloc or sub alloc
   **/
  static void deallocate(char* p, size_t) {
    // there should be some alignment issues
    delete [] p;
  }

  /**
   * @return the maximum allocable size
   * __PTRDIFF_MAX__
   **/
  static size_t max_size() {
    // Might use __PTRDIFF_MAX
    return size_t(-1);
  }

  // For C++ 11 and later versions, STL uses std::forward.
  //

  // Here is the simmplified version:
  /**
   * Comparator
   **/
  template <bool threads1, int inst1, bool threads2, int inst2>
  friend bool operator==(
    const __simple_default_allocator_template<threads1, inst1>&, 
    const __simple_default_allocator_template<threads2, inst2>&) {
    return true;
  }
  template <bool threads1, int inst1, bool threads2, int inst2>
  friend bool operator!=(
    const __simple_default_allocator_template<threads1, inst1>&, 
    const __simple_default_allocator_template<threads2, inst2>&) {
    return false;
  }
};

}  // namespace sup
#endif