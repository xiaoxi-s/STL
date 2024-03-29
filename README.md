## STL

Implement basic algorithm, adaptor, container, iterator, functors in STL that are easier to understand. There would be concepts & questions related with each file based on the concepts learned from implementation. 

### Environment

- System: Ubuntu 20.04 LTS
- Build: gcc 9.3.0 + CMake 3.16.3
- Editor: VSCode

### Considerations
 - Multi-threading is not considered. 
 - Alignment issues are not considered as the allocator is just a wrapper for new and delete.
 - The source uses some standard libraries (listed below) for brevity. 

### Takeaways
 - traits methods
 - division of trivial & non-trivial objects for performance
 - quick sort for list (although merge sort appears to be simpler)
 - the range specification [first, last) has many advantages
 - intro sort: best O(n), worst O(nlogn)

## Core Sources Implementation

### Utility Functions
 - Allocators finished (now using `new` and `delete`)
 - Memory initialization library finished

### Data Structures
 - `vector`: finished
 - `list`: finished
 - `deque`: finished
 - `stack`: finished
 - `queue`: finished
 - `priority_queue`: finished
 - `forward_list`: finished
 - `rb_tree`: finished
 - `set`: finished
 - `map`: finished
 - `unordered_set`: finished
 - `unordered_map`: finished

### Algorithms
- `set_operations`: finished
- `binary_operations`: finished
- `heap`: finished

## Test

### Valgrind
Valgrind memcheck test Passed. The detailed process is as follows: First, build the project, 

```shell
mkdir build
cd build
cmake ..
```
Then, find the executable in `build/test/correctness/STL_CORRECTNESS_TESTS`. With valgrind installed (using `sudo apt install valgrind`), run the following command in shell,

```shell
valgrind --leak-check=full ./test/correctness/STL_CORRECTNESS_TESTS
```

### Utilities Tests
 - Allocators finished
 - Memory initialization library finished

### Data Structures Tests
- `vector`: finished
- `list`: finished
 - `deque`: finished
 - `stack`: finished
 - `queue`: finished
 - `priority_queue`: finished
 - `forward_list`: finished
 - `rb_tree`: finished
 - `set`: finished
 - `map`: finished
 - `unordered_set`: finished
 - `unordered_map`: finished

### Algorithms Tests
- `set_operations`: finished
- `binary_operations`: finished
- `heap`: finished

## Next Step

No intention of implementing performance test...

### Standard Libraries used

This part records the standard libraries that are necessary. 

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
    - `utility` (use std::pair)
  - Functions: 

- `sstl_set.hpp`
  - Source: None
  - Functions: 
    - `std::less` is used as the default template parameter for set
    - `std::_Identity` is used as the template parameter for RB tree
- `sstl_map.hpp`
  - Source: 
    - `utility` library for std::pair
  - Functions:
    - `std::make_pair`
    - `std::less`
    - `std::binary_function`
- `sstl_hashtable.hpp`
  - Source:
    - `algorithm` use std::upper_bound
    - `utility` use std::make_pair
  - Functions:
    - None
- `sstl_unordered_set.hpp`
  - Source: 
    - `functional`
  - Functions:
    - `std::make_pair`
    - `std::hash<T>`
    - `std::equal_to<T>`
    - `std::_Identity<Key>`
- `sstl_unordered_map.hpp`
  - Source: 
    - `functional`
  - Functions:
    - `std::make_pair`
    - `std::hash<T>`
    - `std::equal_to<T>`
    - `std::_Select1st<std::pair<T1, T2>>`
- `sstl_binary_operations.hpp`
  - Source: None
  - Functions: None
- `sstl_heap.hpp`
  - Source: None
  - Functions:
    - `std::swap`
- `sstl_set_operations.hpp`
  - Source: None
  - Functions: `std::copy`
- `sstl_sort.hpp`
  - Source: None
  - Functions: 
    - `std::copy_backward`
    - `std::iter_swap`