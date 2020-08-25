# STL

Implement basic algorithm, adaptor, container, iterator, functors in STL that are easier to understand. There would be concepts & questions related with each file based on the concepts learned from implementation. 

# Current Situations

 - Allocators finished
 - Memory initialization library finished

# Considerations

 - Multi-threading is not considered. 
 - Alignment issues are not considered.
 - The source uses some standard libraries (listed below) for brevity. 

## Standard Libraries used

This part records the standard libraries that are not currently implemented, while have to be used. 

1. sstl_allocator_base.hpp
  - Source: 
    <new>
  - Functions: 
  Source: new (for placement new)

2. sstl_construct.hpp

  - Source: 
    - <type_traits>
    - ext/alloc_traits.h (for iterator_traits)

  - Functions: 
    - __has_trivial_destructor()
    - std::__addressof
    - std::advance

3. sstl_uninitialized.hpp 
  - Sources: 
    - <type_traits>

  - Functions: 
    - stl::__addressof
    - std::iterator_traits
    - is_trivial
    - std::copy
    - std::fill
    - std::fill_n

# Environment

## System

Ubuntu 20.04 LTS

## Build

gcc 9.3.0

CMake 3.16.3

## Editor

VSCode

# Next Step

- Construct file with specified allocators
- Study iterators
