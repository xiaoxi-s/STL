#ifndef _SSTL_BINARY_OPERATIONS_H
#define _SSTL_BINARY_OPERATIONS_H

// include for std::pair
#include <utility>

/**
 * @author Xiaoxi
 **/

namespace sup {
template <class ForwardIterator, class T>
bool binary_search (ForwardIterator first, ForwardIterator last,
                    const T& val) {
  while (first < last) {
    ForwardIterator mid = first + (last - first)/2;
    if (*mid < val) {
      first = mid + 1;
    } else if (val < *mid) {
      last = mid;
    } else {
      return true;
    }
  }
  return false;
}

template <class ForwardIterator, class T, class Comparator>
bool binary_search (ForwardIterator first, ForwardIterator last,
                    const T& val, Comparator cmp) {
  while (first < last) {
    ForwardIterator mid = first + (last - first)/2;
    if (cmp(*mid, val)) {
      first = mid + 1;
    } else if (cmp(val, *mid)) {
      last = mid;
    } else {
      return true;
    }
  }
  return false;
}

template <class ForwardIterator, class T>
ForwardIterator upper_bound (ForwardIterator first, ForwardIterator last,
                    const T& val) {
  while (first < last) {
    ForwardIterator mid = first + (last - first)/2;
    if (val < *mid) {
      last = mid;
    } else 
      first = mid + 1;
  }
  
  return first;
}

template <class ForwardIterator, class T, class Comparator>
ForwardIterator upper_bound (ForwardIterator first, ForwardIterator last,
                    const T& val, Comparator cmp) {
  while (first < last) {
    ForwardIterator mid = first + (last - first)/2;
    if (cmp(val, *mid)) {
      last = mid;
    } else 
      first = mid + 1;
  }
  
  return first;
}

template <class ForwardIterator, class T>
ForwardIterator lower_bound (ForwardIterator first, ForwardIterator last,
                    const T& val) {
  while (first < last) {
    ForwardIterator mid = first + (last - first)/2;
    if (*mid < val) {
      first = mid + 1;
    } else 
      last = mid;
  }
  
  return first;
}

template <class ForwardIterator, class T, class Comparator>
ForwardIterator lower_bound (ForwardIterator first, ForwardIterator last,
                    const T& val, Comparator cmp) {
  while (first < last) {
    ForwardIterator mid = first + (last - first)/2;
    if (cmp(*mid, val)) {
      first = mid + 1;
    } else 
      last = mid;
  }
  
  return first;
}

template <class ForwardIterator, class T>
std::pair<ForwardIterator, ForwardIterator>
equal_range (ForwardIterator first, ForwardIterator last, const T& val) {
  ForwardIterator it = lower_bound(first, last, val);
  return std::make_pair(it, upper_bound(it, last, val));
}

template <class ForwardIterator, class T, class Comparator>
std::pair<ForwardIterator, ForwardIterator>
equal_range (ForwardIterator first, ForwardIterator last, const T& val, 
             Comparator cmp) {
  ForwardIterator it = lower_bound(first, last, val, cmp);
  return std::make_pair(it, upper_bound(it, last, val, cmp));
}
}

#endif