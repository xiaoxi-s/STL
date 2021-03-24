#ifndef _SSTL_HEAP_H
#define _SSTL_HEAP_H

#include "../sstl_iterator.hpp"

/**
 * @author Xiaoxi Sun
 **/

/**
 * Concepts
 *  - std::swap used
**/

namespace sup {

// helper functions: get parent and get left child
template <class Difference>
inline Difference __get_parent(Difference cur) {
  Difference parent_ind = (cur - 1) / 2;
  return parent_ind;
}
template <class Difference>
inline Difference __get_left_child(Difference cur) {
  Difference left_child_ind = 2*cur + 1;
  return left_child_ind;
}

// percoalte down and up
template <class RandomAccessIterator>
inline void __percolate_down(RandomAccessIterator first, RandomAccessIterator cur, RandomAccessIterator last) {
  typename sup::iterator_traits<RandomAccessIterator>::value_type tmp; 
  typedef typename sup::iterator_traits<RandomAccessIterator>::difference_type difference_type;
  difference_type n = last - first;
  difference_type i = cur - first; 
  difference_type child_i = __get_left_child(i);

  while (child_i < n) {
    
    if (child_i + 1< n && first[child_i] < first[child_i+1]) {
      child_i = child_i + 1;
    }

    if (first[child_i] > first[i]) {
      tmp = first[i];
      first[i] = first[child_i];
      first[child_i] = tmp;
    } else 
      break;
    
    i = child_i;
    child_i = __get_left_child(i);
  }
}
template <class RandomAccessIterator>
inline void __percolate_up(RandomAccessIterator first, RandomAccessIterator cur, RandomAccessIterator last) {
  typename sup::iterator_traits<RandomAccessIterator>::value_type tmp; 
  typedef typename sup::iterator_traits<RandomAccessIterator>::difference_type difference_type;
  difference_type i = cur - first; 
  difference_type parent = __get_parent(i);

  while (parent >= 0) {
    if (first[parent] < first[i]) {
      tmp = first[parent];
      first[parent] = first[i];
      first[i] = tmp;
    } else 
      break;
    i = cur - first; 
    parent = __get_parent(i);
  }
}

// percoalte down and up with comparator
template <class RandomAccessIterator, class Comp>
inline void __percolate_down(RandomAccessIterator first, RandomAccessIterator cur, RandomAccessIterator last, Comp comparator) {
  typedef typename sup::iterator_traits<RandomAccessIterator>::difference_type difference_type;
  typename sup::iterator_traits<RandomAccessIterator>::value_type tmp; 

  difference_type n = last - first;
  difference_type i = cur - first; 
  difference_type child_i = __get_left_child(i);

  while (child_i < n) {
    if (child_i + 1 < n && comparator(first[child_i], first[child_i + 1]) ) {
      child_i = child_i + 1;
    }

    if (comparator(first[i], first[child_i])) {
      tmp = first[i];
      first[i] = first[child_i];
      first[child_i] = tmp;
    } else 
      break;
    
    i = child_i;
    child_i = __get_left_child(i);
  }
}
template <class RandomAccessIterator, class Comp>
inline void __percolate_up(RandomAccessIterator first, RandomAccessIterator cur, RandomAccessIterator last, Comp comparator) {
  typename sup::iterator_traits<RandomAccessIterator>::value_type tmp; 
  typedef typename sup::iterator_traits<RandomAccessIterator>::difference_type difference_type;

  difference_type i = cur - first;
  difference_type parent = __get_parent(i);

  while (cur >= first) {
    if ( comparator(first[parent], first[i]) ) {
      tmp = first[parent];
      first[parent] = first[i];
      first[i] = tmp;
    } else 
      break;
    i = cur - first;
    parent = __get_parent(i);
  }
}

/**
 * @brief make a max heap given first and last iterator of the array ranging [start, last) 
 * 
 * @tparam RandomAccessIterator - iterator type
 * @param first - start of the range
 * @param last - end of the range
 */
template <class RandomAccessIterator>
inline void make_heap(RandomAccessIterator first, RandomAccessIterator last) {
  typedef typename sup::iterator_traits<RandomAccessIterator>::difference_type difference_type;
  difference_type n = last - first;
  for (difference_type i = n/2 - 1; i >= 0; --i) {
    __percolate_down(first, first + i, last);
  }
}

/**
 * @brief make a heap given first and last iterator of the array ranging [start, last), 
 *  and a comparator: "<" gives a max heap; ">" gives a min heap.
 * 
 * @tparam RandomAccessIterator - iterator type
 * @param first - start of the range
 * @param last - end of the range
 * @param comparator - the given comparator
 */
template <class RandomAccessIterator, class Comp>
inline void make_heap(RandomAccessIterator first, RandomAccessIterator last, Comp comparator) {
  typedef typename sup::iterator_traits<RandomAccessIterator>::difference_type difference_type;
  difference_type n = last - first;
  for (difference_type i = n/2 - 1; i >= 0; --i) {
    __percolate_down(first, first + i, last, comparator);
  }
}

/**
 * @brief push heap algorithm for max heap assuming the last element 
 *  is at last - 1
 * 
 * @tparam RandomAccessIterator - specified iterator type
 * @param first - start of the heap (the value that would be popped out)
 * @param last - end of the heap
 */
template <class RandomAccessIterator>
inline void push_heap(RandomAccessIterator first, RandomAccessIterator last) {
  typedef typename sup::iterator_traits<RandomAccessIterator>::difference_type difference_type;

  last = last - 1;// use last as cur
  difference_type i = last - first;
  RandomAccessIterator parent = first + __get_parent(i);
  typename sup::iterator_traits<RandomAccessIterator>::value_type tmp; 

  while (i >= 0) {
    if (*parent < *last) {
      tmp = *parent; 
      *parent = *last;
      *last = tmp;
    } else 
      break;
    last = parent;
    i = last - first;
    parent = first + __get_parent(i);
  }
}

/**
 * @brief push heap algorithm given a comparator assuming the last element 
 *  is at last - 1
 * 
 * @tparam RandomAccessIterator - specified iterator type
 * @tparam Comp - a function type which is used to compare
 * @param first - start of the heap (the value that would be popped out)
 * @param last - end of the heap
 * @param comparator - the comparator function
 */
template <class RandomAccessIterator, class Comp>
inline void push_heap(RandomAccessIterator first, RandomAccessIterator last, Comp comparator) {
  typedef typename sup::iterator_traits<RandomAccessIterator>::difference_type difference_type;

  last = last - 1;// use last as cur
  difference_type i = last - first;
  RandomAccessIterator parent = first + __get_parent(i);
  typename sup::iterator_traits<RandomAccessIterator>::value_type tmp; 

  while (i >= 0) {
    if (comparator(*parent, *last)) {
      tmp = *parent; 
      *parent = *last;
      *last = tmp;
    } else 
      break;
    
    last = parent;
    i = last - first;
    parent = first + __get_parent(i);;
  }
}

/**
 * @brief Determine whether a container ranging [first, last) is a max heap
 * 
 * @tparam RandomAccessIterator - iterator type
 * @param first - start of the range
 * @param last - end of the range
 */
template <class RandomAccessIterator>
inline bool is_heap(RandomAccessIterator first, RandomAccessIterator last) {
  typedef typename sup::iterator_traits<RandomAccessIterator>::difference_type difference_type;
  difference_type n = last - first; 
  
  for (difference_type i = 0; i < n/2; ++i) {
    RandomAccessIterator cur = first + i;
    RandomAccessIterator left_child = __get_left_child(i) + first;

    if (left_child < last && *cur < *left_child) {
      return false;
    }
    if (left_child + 1 < last && *cur < *(left_child + 1)) {
      return false;
    }
  }

  return true;
}

/**
 * @brief Determine whether a container ranging [first, last) is a heap given 
 *  a comparator
 * 
 * @tparam RandomAccessIterator - iterator type
 * @param first - start of the range
 * @param last - end of the range
 */
template <class RandomAccessIterator, class Comp>
inline bool is_heap(RandomAccessIterator first, RandomAccessIterator last, Comp comparator) {
  typedef typename sup::iterator_traits<RandomAccessIterator>::difference_type difference_type;
  difference_type n = last - first; 
  
  for (difference_type i = 0; i < n/2; ++i) {
    RandomAccessIterator cur = first + i;
    RandomAccessIterator left_child = __get_left_child(i) + first;

    if (left_child < last && comparator(*cur, *left_child) ) {
      return false;
    }
    if (left_child + 1 < last && comparator(*cur, *(left_child + 1)) ) {
      return false;
    }
  }

  return true;
}

/**
 * @brief pop the maximum element from the heap ranging [first, last)
 * 
 * @tparam RandomAccessIterator - iterator type
 * @param first - start of the heap
 * @param last - end of the heap
 */
template <class RandomAccessIterator>
inline void pop_heap(RandomAccessIterator first, RandomAccessIterator last) {
  last = last - 1;
  first[0] = last[0];
  __percolate_down(first, first, last);
}

/**
 * @brief pop the head element out of the heap ranging [first, last) 
 *  based on the comparator
 * 
 * @tparam RandomAccessIterator - iterator type
 * @tparam Comp - function type for the comparator
 * @param first - start of the heap
 * @param last - end of the heap
 * @param comparator - the comparator function
 */
template <class RandomAccessIterator, class Comp>
inline void pop_heap(RandomAccessIterator first, RandomAccessIterator last, Comp comparator) {
    last = last - 1;
  first[0] = last[0];
  __percolate_down(first, first, last, comparator);
}

/**
 * @brief sort a heap in a ascending order
 * 
 * @tparam RandomAccessIterator - iterator
 * @tparam Comp - function type for the comparator
 * @param first - start of the heap
 * @param last - end of the heap
 */
template <class RandomAccessIterator>
inline void sort_heap(RandomAccessIterator first, RandomAccessIterator last) {
  typedef typename sup::iterator_traits<RandomAccessIterator>::difference_type difference_type;
  typedef typename sup::iterator_traits<RandomAccessIterator>::value_type value_type;

  while (last != first) {
    value_type tmp_max = first[0];
    pop_heap(first, last);
    last = last - 1;
    *last = tmp_max;
  }
}

/**
 * @brief sort a heap in a ? order (depends on the comparator)
 * 
 * @tparam RandomAccessIterator - iterator type
 * @tparam Comp - function type for the comparator
 * @param first - start of the heap
 * @param last - end of the heap
 * @param comparartor - the given comparator
 */
template <class RandomAccessIterator, class Comp>
inline void sort_heap(RandomAccessIterator first, RandomAccessIterator last, Comp comparartor) {
  typedef typename sup::iterator_traits<RandomAccessIterator>::difference_type difference_type;
  typedef typename sup::iterator_traits<RandomAccessIterator>::value_type value_type;

  while (last != first) {
    value_type tmp_head = first[0];
    pop_heap(first, last, comparartor);
    last = last - 1;
    *last = tmp_head;
  }
}

}
#endif