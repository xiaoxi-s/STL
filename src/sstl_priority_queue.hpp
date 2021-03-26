#ifndef _SSTL_PRIORITY_QUEUE
#define _SSTL_PRIORITY_QUEUE

#include <functional>

#include "sstl_vector.hpp"
#include "algorithms/sstl_heap.hpp"

/**
 * @author Xiaoxi Sun
 **/

/**
 * Concepts
 *  - std::swap used
 *  - use std::less for convience for now
 *  - __STL_TRY was used for push and pop in the source code
**/

namespace sup {

template <class T, class Sequence=sup::vector<T>,
          class Comp=std::less<typename Sequence::value_type>>
class priority_queue {
public:
  typedef typename Sequence::value_type value_type;
  typedef typename Sequence::size_type size_type;
  typedef typename Sequence::reference reference;
  typedef typename Sequence::const_reference const_reference;

protected:
  Sequence c;
  Comp comparator;

public:
  /**
   * @brief Construct a new priority queue object
   * 
   */
  priority_queue(): c() {}

  /**
   * @brief Construct a new priority queue object given a comparator
   * 
   * @param comp - a reference to the given comparator
   */
  explicit priority_queue(const Comp& comp): c(), comparator(comp) {}

  /**
   * @brief Construct a new priority queue object based on a 
   *  range of elements [first, last) and a specified comparator
   * 
   * @tparam InputIterator - type of the input iterator
   * @param first - the start of the range
   * @param last - the end of the range
   */
  template <class InputIterator>
  priority_queue(InputIterator first, InputIterator last, 
    const Comp& comp): c(first, last), comparator(comp) {
      sup::make_heap(c.begin(), c.end(), comparator);
  }

  /**
   * @brief Construct a new priority queue object based on a 
   *  range of elements [first, last) and with the default 
   *  comparator
   * 
   * @tparam InputIterator - type of the input iterator
   * @param first - the start of the range
   * @param last - the end of the range
   */
  template <class InputIterator>
  priority_queue(InputIterator first, InputIterator last)
    : c(first, last) {
      sup::make_heap(c.begin(), c.end(), comparator);
  }

  /**
   * @brief returns a boolean variable denoting whether the 
   *  priority queue is empty
   * 
   * @return true - the priority queue is empty
   * @return false - the priority queue is not empty
   */
  bool empty() const { return c.empty(); }

  /**
   * @brief return the size of the priority queue
   * 
   * @return size_type - the size of the priority queue
   */
  size_type size() const { return c.size(); }

  /**
   * @brief return a constant reference to the first element
   * 
   * @return const_reference - to the first element
   */
  const_reference top() const { return c.front(); }

  /**
   * @brief push an element of value into the pq
   * 
   * @param val - of the element
   */
  void push(value_type& val) {
    try {
      c.push_back(val);
      sup::push_heap(c.begin(), c.end(), comparator);
    } catch (...) {
      c.clear();
      throw;
    }
  }

  /**
   * @brief pop an element out of the priority queue
   */
  void pop() {
    try {
      sup::pop_heap(c.begin(), c.end(), comparator);
      c.pop_back();
    } catch(...) {
      c.clear();
      throw;
    }
  }
};

}
#endif