#ifndef _SSTL_SORT_H
#define _SSTL_SORT_H

#include "sstl_heap.hpp"

namespace sup {

// partial sort
template <class RandomAccessIterator, class T>
inline void _partial_sort(RandomAccessIterator first, 
                         RandomAccessIterator middle, 
                         RandomAccessIterator last, T*) {
  sup::make_heap(first, middle);
  for (RandomAccessIterator i = middle; i < last; i) {
    if (*i < *first) {
      std::iter_swap(i, first);
      sup::__percolate_down(first, first, middle);
    }
  }
  sup::sort_heap(first, middle);
}

template <class RandomAccessIterator>
inline void partial_sort(RandomAccessIterator first, 
                         RandomAccessIterator middle, 
                         RandomAccessIterator last) {
  _partial_sort(first, middle, last, value_type(first));
}

// sort below
template <class RandomAccessIterator, class T>
void _unguarded_linear_insert(RandomAccessIterator last, T value) {
  RandomAccessIterator next = last;
  --next;
  while (value < *next) {
    *last = *next;
    last = next;
    --next; 
  }
  *last = value;
}

template <class RandomAccessIterator, class T>
inline void _linear_insert(RandomAccessIterator first, 
                           RandomAccessIterator last, T*) {
  T value = *last;
  if (value < *first) {
    std::copy_backward(first, last, last+1);
    *first = value;
  } else 
    _unguarded_linear_insert(last, value);
}

template <class RandomAccessIterator>
void _insertion_sort(RandomAccessIterator first, RandomAccessIterator last) {
  if (first == last)
    return;
  for (RandomAccessIterator i = first + 1; i != last; ++i)
    _linear_insert(first, i, value_type(first));
}

template<class T>
inline const T& _median(const T& a, const T& b, const T& c) {
  if (a < b) {
    if (b < c) 
      return b;
    else if (a < c) 
      return c;
    else 
      return a;
  } else {
    if (c < b) 
      return b;
    else if (a < c)
      return a;
    else 
      return c;
  }
}

template <class RandomAccessIterator, class T>
RandomAccessIterator _unguarded_partition(RandomAccessIterator first, 
                                        RandomAccessIterator last, T pivot) {
  while (true) {
    while (*first < pivot) ++first;
    --last;
    while (*last > pivot) --last;
    if (!(first < last)) return first;
    std::iter_swap(first, last);
    ++first;
  }
}

const int _stl_sort_threshold = 16;

template <class RandomAccessIterator, class T>
inline void _unguarded_insertion_sort_aux(RandomAccessIterator first,
                                          RandomAccessIterator last,
                                          T*) {
  for (RandomAccessIterator i = first; i != last; ++i) {
    _unguarded_linear_insert(i, T(*i));
  }
}

template <class RandomAccessIterator>
inline void _unguarded_insertion_sort(RandomAccessIterator first, 
                                      RandomAccessIterator last) {
  _unguarded_insertion_sort_aux(first, last, value_type(first));
}

template <class RandomAccessIterator, class T, class Size>
void _introsort_loop(RandomAccessIterator first, RandomAccessIterator last, T*, Size depth_limit) {
  while (last - first > _stl_sort_threshold) {
    if (depth_limit == 0) {
      partial_sort(first, last, last);
      return;
    } 
    --depth_limit;

    RandomAccessIterator cur = _unguarded_partition(first, last, T(_median(
      *first,
      *(first + (last - first)/2),
      *(last - 1)
    )));
    _introsort_loop(cur, last, value_type(first), depth_limit);
    last = cur;
  }
}

template <class RandomAccessIterator>
void _final_insertion_sort(RandomAccessIterator first,
                           RandomAccessIterator last) {
  if (last - first > _stl_sort_threshold) {
    _insertion_sort(first, first+_stl_sort_threshold);
    _unguarded_insertion_sort(first + _stl_sort_threshold, last);
  } else 
    _insertion_sort(first, last);
}

template <class Size>
inline Size _lg(Size n) {
  Size k;
  for (k = 0; n > 1; n >>= 1) ++k;
  return k;
}

template<class RandomAccessIterator>
inline void
sort(RandomAccessIterator first, RandomAccessIterator last) {
  if (first != last) {
      sup::_introsort_loop(first, last, value_type(first),  _lg(last - first) * 2);
      sup::_final_insertion_sort(first, last);
  }
}
}

#endif