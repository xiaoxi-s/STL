## STL

Implement basic algorithm, adaptor, container, iterator, functors in STL that are easier to understand. There would be concepts & questions related with each file based on the concepts learned from implementation. 

### Environment

- System: Ubuntu 20.04 LTS
- Build: gcc 9.3.0 + CMake 3.16.3
- Editor: VSCode


## Takeaways
 - traits methods
 - division of trivial & non-trivial objects for performance
 - quick sort for list
 - the range specification [first, last) has many advantages


## Core Sources Implementation
### Utily Functions
 - Allocators finished
 - Memory initialization library finished

### Data Structures
 - `vector`: finished
 - `list`: finished
 - `deque`: finished
 - `stack`: finished
 - `queue`: finished
 - `priority_queue`: finished
 - `forward_list`: finished
 - `rb_tree`: on-going

## Next Step

- Study associative containers.

## Test

### Utilities
- `allocator`: allocator unfinished; base finished

### Data Structures
- `vector`: finished
- `list`: finished
 - `deque`: finished
 - `stack`: finished
 - `queue`: finished
 - `priority_queue`: finished
 - `forward_list`: finished
 - `rb_tree`: on-going

### Algorithms
- `uninitialized`: finished
- `iterator`: finished
- `construct`: finished
- `heap`: finished

## Considerations

 - Multi-threading is not considered. 
 - Alignment issues are not considered.
 - The source uses some standard libraries (listed below) for brevity. 

### Standard Libraries used

This part records the standard libraries that are not currently implemented, while have to be used. 

- `sstl_allocator_base.hpp` and `sstl_allocator.hpp`
  - Source: 
    - `<new>`
  - Functions: 
    - `static_cast<T>()`

- `sstl_construct.hpp`

  - Source: 
    - `<type_traits>`
    - `<ext/alloc_traits.h>` (for iterator_traits)

  - Functions & Types: 
    - `__has_trivial_destructor()`
    - `std::__addressof()`
    - `__gnu_cxx::__alloc_traits<Allocator>`

- `sstl_iterator_base.hpp`
  - Source: `<cstddef>`
  - Functions:

- `sstl_iterator.hpp`
  - Source: None
  - Functions: None
  - Keyword: `decltype`

- `sstl_uninitialized.hpp`
  - Sources: None
  - Functions: 
    - `std::__addressof()`
    - `std::iterator_traits()`
    - `is_trivial()`
    - `std::copy()`
    - `std::fill()`
    - `std::fill_n()`

- `sstl_vector.hpp`
  - Sources: None
  - Functions: 
    - `std::copy`
    - `std::copy_backward`
    - `std::is_integral<>`
    - `std::fill`

- `sstl_list.hpp`
  - Sources: 
    - `Type Traits`
  - Functions: 
    - `std::__is_integer`
    - `std::allocator_traits`

- `sstl_deque.hpp`
  - Sources: None
  - Functions: 
    - `std::copy`
    - `std::copy_backward`

- `sstl_queue.hpp`
  - Contains deque by default 

- `sstl_stack.hpp`
  - Contains deque by default

- `sstl_forward_list.hpp`
  - Sources: None
  - Functions: None

- `sstl_rb_tree.hpp`
  - Source: 
  - Functions: 