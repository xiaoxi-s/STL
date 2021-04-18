#ifndef _SSTL_VECTOR_H
#define _SSTL_VECTOR_H

#include "sstl_allocator.hpp"
#include "sstl_iterator.hpp"
#include "sstl_uninitialized.hpp"

/**
 * @author Xiaoxi Sun
 **/

/**
 * Concepts:
 *  - vector
 *  - range [first, last)
 *  - separation of construct & allocation
 **/

/**
 * Questions:
 *  - No vector_base is implemented for space allocation
 *  - destroy() & construct() functions are introduced after c++17. Therefore,
 *    they are not used here.
 *  - std::copy used
 *  - std::copy_backward used
 *  - std::is_integral<> used
 *  - std::fill used
 *  - const overload not implemented
 *  - std::__is_integer, std::__true_type, and std::__false_type used
 **/

namespace sup {

template <class T, class Alloc=alloc>
class vector {
  /******** Public types and methods ********/
 public:
  typedef T value_type;
  typedef T* pointer;
  typedef const T* const_pointer;

  typedef T* iterator;  // use pointers as iterators
  typedef const T* const_iterator;
  typedef sup::reverse_iterator<iterator> reverse_iterator;
  typedef sup::reverse_iterator<const_iterator> const_reverse_iterator;

  typedef T& reference;
  typedef const T& const_reference;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;

  typedef sup::simple_alloc<T, Alloc> allocator_type;

  /******** Constructors ********/
  vector();
  explicit vector(size_type n);
  template<class InputIterator1, class InputIterator2>
  vector(InputIterator1 first, InputIterator2 last);
  vector(vector& x);
  // Destructors
  ~vector();

  /******** Sizes ********/
  size_type size() const;
  size_type capacity() const;
  bool empty() const;
  void resize(size_type new_size, const_reference value);
  void resize(size_type new_size);

  /******** Iterators ********/
  iterator begin();
  iterator end();
  reverse_iterator rbegin();
  reverse_iterator rend();

  /******** Element access ********/
  reference operator[](size_type n);
  reference front();
  const_reference front() const;
  reference back();

  void push_back(const_reference value);
  void pop_back();

  iterator erase(iterator position);
  iterator erase(iterator first, iterator last);

  void reserve(size_type n);
  void clear();

  template<class InputIterator1, class InputIterator2>
  void assign(InputIterator1 first, InputIterator2 last);

  iterator insert(iterator position, const_reference value);
  void insert(iterator position, size_type n, const_reference value);
  template <class InputIterator>
  void insert(iterator position, InputIterator first, InputIterator last);

  void swap(vector<T>& x);

  allocator_type get_allocator() const;

 protected:
  typedef simple_alloc<T, Alloc> data_allocator;
  iterator start;
  iterator finish;
  iterator end_of_storage;

  // helper function when inserting into a full vector
  void insert_aux(iterator position, const_reference value);
  template <class InputIterator>
  void insert_aux(iterator position, InputIterator first, InputIterator last,
                  std::false_type);
  template <class Integer>
  void insert_aux(iterator position, Integer n, const_reference value,
                  std::true_type);
  void deallocate();

  template<class InputIterator1, class InputIterator2>
  void assign_aux(InputIterator1 first, InputIterator2 last, std::__false_type);
  template<class Size>
  void assign_aux(Size n, const T& value, std::__true_type);

  iterator allocate_and_fill(size_type n, const T& value);

  void fill_initialize(size_type n, const T& value);
};

/************** Constructors & Destructor **************/

/**
 * @brief Construct a new vector<T, Alloc>::vector object
 *
 * @tparam T - element type  parameter
 * @tparam Alloc - allocator type
 */
template <class T, class Alloc>
vector<T, Alloc>::vector() : start(0), finish(0), end_of_storage(0) {}

/**
 * @brief Construct a new vector<T, Alloc>::vector object
 *
 * @tparam T - element type  parameter
 * @tparam Alloc - allocator type
 * @param n - number of elements
 */
template <class T, class Alloc>
vector<T, Alloc>::vector(size_type n) {
  fill_initialize(n, T());
}

/**
 * @brief The entrance of constructor based on two-paramters. The types of 
 *  the parameters depend on the exact specialization of this function.
 * 
 * @tparam T 
 * @tparam Alloc 
 * @tparam InputIterator1 
 * @tparam InputIterator2 
 * @param first 
 * @param last 
 */
template <class T, class Alloc>
template<class InputIterator1, class InputIterator2>
vector<T, Alloc>::vector(InputIterator1 first, InputIterator2 last) {
  start = nullptr;
  finish = nullptr;
  end_of_storage = nullptr;

    assign(first, last);
}

/**
 * @brief Construct a new vector<T, Alloc>::vector object
 *
 * @tparam T - element type  parameter
 * @tparam Alloc - allocator type
 * @param x - another vector
 */
template <class T, class Alloc>
vector<T, Alloc>::vector(vector& x) {
  start = data_allocator::allocate(x.size());
  end_of_storage = start + x.size();
  finish = uninitialized_copy(x.begin(), x.end(), start);
}

/**
 * @brief Destroy the vector<T, Alloc>::vector object
 *
 * @tparam T - element type parameter
 * @tparam Alloc - allocator type
 */
template <class T, class Alloc>
vector<T, Alloc>::~vector() {
  sup::_destroy(start, finish);
  deallocate();
}

/**************   Iterators   **************/

/**
 * @brief return the start of the vector
 *
 * @tparam T - element type  parameter
 * @tparam Alloc - allocator type
 * @return vector<T, Alloc>::iterator - iterator at the start
 */
template <class T, class Alloc>
typename vector<T, Alloc>::iterator vector<T, Alloc>::begin() {
  return start;
}

/**
 * @brief return the end of the vector
 *
 * @tparam T - element type  parameter
 * @tparam Alloc - allocator type
 * @return vector<T, Alloc>::iterator - iterator at the end
 */
template <class T, class Alloc>
typename vector<T, Alloc>::iterator vector<T, Alloc>::end() {
  return finish;
}

/**
 * @brief return the reverse begin
 *
 * @tparam T - element type  parameter
 * @tparam Alloc - allocator type
 * @return vector<T, Alloc>::iterator - iterator at the reverse start
 */
template <class T, class Alloc>
typename vector<T, Alloc>::reverse_iterator vector<T, Alloc>::rbegin() {
  return sup::reverse_iterator<iterator>(finish);
}

/**
 * @brief return the reverse end
 *
 * @tparam T - element type  parameter
 * @tparam Alloc - allocator type
 * @return vector<T, Alloc>::iterator - iterator at the reverse end
 */
template <class T, class Alloc>
typename vector<T, Alloc>::reverse_iterator vector<T, Alloc>::rend() {
  return sup::reverse_iterator<iterator>(start);
}

/**************   Size   **************/

/**
 * @brief return the number of elements in the vector
 *
 * @tparam T - element type paramter
 * @tparam Alloc - allocator type
 * @return vector<T, Alloc>::size_type - size
 */
template <class T, class Alloc>
typename vector<T, Alloc>::size_type vector<T, Alloc>::size() const {
  return size_type(finish - start);
}

/**
 * @brief return the capacity of the vector
 *
 * @tparam T - element type  parameter
 * @tparam Alloc - alocator type
 * @return vector<T, Alloc>::size_type - capacity
 */
template <class T, class Alloc>
typename vector<T, Alloc>::size_type vector<T, Alloc>::capacity() const {
  return size_type(end_of_storage - start);
}

/**
 * @brief determine whether the vector is constant
 *
 * @tparam T - element type  parameter
 * @tparam Alloc - alocator type
 * @return true - is empty
 * @return false - is not empty
 */
template <class T, class Alloc>
bool vector<T, Alloc>::empty() const {
  return start == finish;
}

/**
 * @brief resize the vector
 *
 * @tparam T - element type parameter
 * @tparam Alloc - alocator type
 * @param new_size - new size
 * @param value - values to be filled in if the size is larger
 */
template <class T, class Alloc>
void vector<T, Alloc>::resize(size_type new_size, const_reference value) {
  if (new_size < size())
    erase(start + new_size, finish);
  else
    insert(finish, new_size - size(), value);
}

/**
 * @brief resize with 0 filled in
 *
 * @tparam T - element type parameter
 * @tparam Alloc - allocator type
 * @param new_size - new size
 */
template <class T, class Alloc>
void vector<T, Alloc>::resize(size_type new_size) {
  resize(new_size, T());
}

/************** Element Access **************/

/**
 * @brief index operator
 *
 * @tparam T - element type parameter
 * @tparam Alloc - allocator type
 * @param n - index
 * @return vector<T, Alloc>::reference - the element reference at n + start
 */
template <class T, class Alloc>
typename vector<T, Alloc>::reference vector<T, Alloc>::operator[](size_type n) {
  return *(start + n);
}

/**
 * @brief return the front element
 *
 * @tparam T - element type parameter
 * @tparam Alloc - allocator type
 * @return vector<T, Alloc>::reference - the element reference at the front
 */
template <class T, class Alloc>
typename vector<T, Alloc>::reference vector<T, Alloc>::front() {
  return *start;
}

/**
 * @brief return the const reference to the front element
 *
 * @tparam T - element type parameter
 * @tparam Alloc - allocator type
 * @return vector<T, Alloc>::reference - the element reference at the front
 */
template <class T, class Alloc>
typename vector<T, Alloc>::const_reference vector<T, Alloc>::front() const {
  return *start;
}

/**
 * @brief return the last element
 *
 * @tparam T - element type parameter
 * @tparam Alloc - allocator type
 * @return vector<T, Alloc>::reference - the element reference at the end
 */
template <class T, class Alloc>
typename vector<T, Alloc>::reference vector<T, Alloc>::back() {
  return *(finish - 1);
}

/************** Vector Modification **************/

/**
 * @brief push the parameter into the vector from the back.
 *
 * @tparam T - element type parameter
 * @tparam Alloc - allocator type
 * @param value - to be pushed
 */
template <class T, class Alloc>
void vector<T, Alloc>::push_back(const_reference value) {
  if (finish != end_of_storage) {
    _construct(finish, value);
    ++finish;
  } else {
    insert_aux(finish, value);
  }
}

/**
 * @brief pop the last value from the end;
 *
 * @tparam T - element type parameter
 * @tparam Alloc - allocator type
 */
template <class T, class Alloc>
void vector<T, Alloc>::pop_back() {
  --finish;
  sup::_destroy(finish);
}

// erase
/**
 * @brief erase the element at position
 *
 * @tparam T - element type parameter
 * @tparam Alloc - allocator type
 * @param position - the position of the element to be erased
 * @return vector<T, Alloc>::iterator - iterator pointing to position
 */
template <class T, class Alloc>
typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(iterator position) {
  if (position + 1 != finish) {
    std::copy(position + 1, finish, position);
  }
  --finish;
  sup::_destroy(finish);

  return position;
}

/**
 * @brief erase the element in the given range [first, last)
 *
 * @tparam T - element type parameter
 * @tparam Alloc - allocator type
 * @param first - start the range (inclusive)
 * @param last - end of the range (exclusive)
 * @return vector<T, Alloc>::iterator - start iterator
 */
template <class T, class Alloc>
typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(iterator first,
                                                            iterator last) {
  iterator temp_end = std::copy(last, finish, first);
  sup::_destroy(temp_end, finish);
  finish = temp_end;
  return first;
}

/**
 * @brief reserve capacity of at least n
 * 
 * @tparam T - element type parameter
 * @tparam Alloc - allocator type
 * @param n - the capacity must be greater than this after reserve
 */
template <class T, class Alloc>
void vector<T, Alloc>::reserve(size_type n) {
  if (capacity() < n) {
    iterator new_start = data_allocator::allocate(n);
    iterator new_end_of_storage = new_start + n;
    iterator new_finish = sup::uninitialized_copy(begin(), end(), new_start);

    clear();
    deallocate();

    start = new_start;
    end_of_storage = new_end_of_storage;
    finish = new_finish;
  }
}

/**
 * @brief clear the vector. size would be 0.
 *
 * @tparam T - element type parameter
 * @tparam Alloc - allocator type
 */
template <class T, class Alloc>
void vector<T, Alloc>::clear() {
  erase(start, finish);
}

// assign vector to the current vector based on input types
template <class T, class Alloc>
template<class InputIterator1, class InputIterator2>
void vector<T, Alloc>::assign(InputIterator1 first, InputIterator2 last) {
  typedef typename std::__is_integer<InputIterator1>::__type _Integral;

  assign_aux(first, last, _Integral());
}

/**
 * @brief assign new value to the vector
 * 
 * @tparam T - element type parameter
 * @tparam Alloc - allocator type
 * @tparam InputIterator - input iterator
 * @param first - start of input iterator
 * @param last - end of input iterator
 */
template <class T, class Alloc>
template <class InputIterator1, class InputIterator2>
void vector<T, Alloc>::assign_aux(InputIterator1 first, InputIterator2 last, std::__false_type) {
  if (!empty()) {
    data_allocator::deallocate(start, this->size());
  }
  size_type n = last - first;
  start = data_allocator::allocate(n);
  finish = uninitialized_copy(first, last, start);
  end_of_storage = start + n;
}

/**
 * @brief assign new value to the vector
 * 
 * @tparam T - element type parameter
 * @tparam Alloc - allocator type
 * @param n - number of elements
 * @param val - value
 */
template <class T, class Alloc>
template<class Size>
void vector<T, Alloc>::assign_aux(Size n, const T& value, std::__true_type) {
  if (!empty()) {
    data_allocator::deallocate(start, this->size());
  }
  start = data_allocator::allocate(n);
  finish = uninitialized_fill_n(start, n, value);
  end_of_storage = start + n;
}

// insert
/**
 * @brief insert an element into position
 *
 * @tparam T - element type parameter
 * @tparam Alloc - allocator type
 * @param position - where value to be inserted
 * @param value - to be inserted
 * @return vector<T, Alloc>::iterator - position
 */
template <class T, class Alloc>
typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(
    iterator position, const_reference value) {
  insert(position, 1, value);
  return position;
}

/**
 * @brief insert n elements with value, starting from position.
 *
 * @tparam T - element type parameter
 * @tparam Alloc - allocator type
 * @param position - start position
 * @param n - number of elements
 * @param value - value to be inserted
 */
template <class T, class Alloc>
void vector<T, Alloc>::insert(iterator position, size_type n,
                              const_reference value) {
  insert_aux(position, n, value, typename std::is_integral<size_type>::type());
}

/**
 * @brief insert the elements in range [first, last)
 *
 * @tparam T - element type parameter
 * @tparam Alloc - allocator type
 * @tparam InputIterator - iterators to values to be copied
 * @param position - start point of insertion
 * @param first - start of the iterator
 * @param last - end of the iterator
 */
template <class T, class Alloc>
template <class InputIterator>
void vector<T, Alloc>::insert(iterator position, InputIterator first,
                              InputIterator last) {
  insert_aux(position, first, last,
             typename std::is_integral<InputIterator>::type());
}

/**
 * @brief swap this with x
 *
 * @tparam T - element type parameter
 * @tparam Alloc - allocator type
 * @param x - the vector to be swapped
 */
template <class T, class Alloc>
void vector<T, Alloc>::swap(vector<T>& x) {
  iterator temp_start = x.start;
  iterator temp_finish = x.finish;
  iterator temp_end_of_storage = x.end_of_storage;

  x.start = this->start;
  x.finish = this->finish;
  x.end_of_storage = this->end_of_storage;

  this->start = temp_start;
  this->finish = temp_finish;
  this->end_of_storage = temp_end_of_storage;
}
/**
 * @brief return a allocator of the vector
 * 
 * @tparam T - element type parameter
 * @tparam Alloc - allocator type
 * @return allocator_type 
 */
template <class T, class Alloc>
typename vector<T, Alloc>::allocator_type vector<T, Alloc>::get_allocator() const {
  return data_allocator();
}

/************** Private (helper) methods **************/

template <class T, class Alloc>
void vector<T, Alloc>::deallocate() {
  if (start)
    data_allocator::deallocate(start, size_type(end_of_storage - start));
}

template <class T, class Alloc>
typename vector<T, Alloc>::iterator vector<T, Alloc>::allocate_and_fill(
    size_type n, const T& value) {
  iterator result = data_allocator::allocate(n);
  sup::uninitialized_fill_n(result, n, value);
  return result;
}

template <class T, class Alloc>
void vector<T, Alloc>::fill_initialize(size_type n, const T& value) {
  start = data_allocator::allocate(n);
  sup::uninitialized_fill_n(start, n, value);
  finish = start + n;
  end_of_storage = finish;
}

template <class T, class Alloc>
void vector<T, Alloc>::insert_aux(iterator position, const_reference& value) {
  if (finish != end_of_storage) {  // enough capacity
    // construct a value at the expended place with value value
    sup::_construct(finish, *(finish - 1));  // this is for destroy

    ++finish;
    std::copy_backward(position, finish - 2, finish - 1);
    *position = value;
  } else {  // not enough capacity
    const size_type old_size = size();
    const size_type new_size = (old_size == 0 ? 1 : 2 * old_size);

    iterator new_start = data_allocator::allocate(new_size);
    iterator new_finish = new_start;

    try {  // copy everything in the original vector
      new_finish = sup::uninitialized_copy(start, position, new_start);
      sup::_construct(new_finish, value);
      new_finish = sup::uninitialized_copy(position, finish, new_finish + 1);
    } catch (...) {  // roll back or commit
      sup::_destroy(new_start, new_finish);
      data_allocator::deallocate(new_start, new_size);
      throw;
    }

    sup::_destroy(start, finish);
    deallocate();

    start = new_start;
    finish = new_finish;
    end_of_storage = start + new_size;
  }
}

template <class T, class Alloc>
template <class InputIterator>
void vector<T, Alloc>::insert_aux(iterator position, InputIterator first,
                                  InputIterator last, std::false_type) {
  typename sup::iterator_traits<InputIterator>::difference_type n =
      last - first;
  size_type old_size = size();
  difference_type ele_after = finish - position;

  if (n + old_size <= end_of_storage - start) {
    difference_type num_of_elem_after = finish - position;

    if (num_of_elem_after >= n) {
      // perhaps call (non-trivial) destructors for
      // num_of_elem_after times
      sup::uninitialized_copy(finish - n, finish, finish);
      std::copy_backward(position, finish - n, finish);
      std::copy(first, last, position);

      finish += n;
    } else {
      // would call (non-trivial) destructors for num_of_elem_after
      // times;
      // a better writing would be: finish + n - num_of_elem_after; but this
      // could cause overflow
      sup::uninitialized_copy(position, finish, finish - num_of_elem_after + n);
      std::copy(first, first + num_of_elem_after, position);
      sup::uninitialized_copy(first + num_of_elem_after, last,
                              position + num_of_elem_after);

      finish += n;
    }
  } else {
    size_type new_capacity =
        2 * old_size >= (old_size + n) ? 2 * old_size : (old_size + n);
    iterator new_start = data_allocator::allocate(new_capacity);
    iterator new_finish = new_start;
    try {
      new_finish = sup::uninitialized_copy(start, position, new_start);
      new_finish = sup::uninitialized_copy(first, last, new_finish);
      new_finish = sup::uninitialized_copy(position, finish, new_finish);
    } catch (...) {
      sup::_destroy(new_start, new_finish);
      data_allocator::deallocate(new_start, new_capacity);
      throw;
    }
    this->deallocate();

    start = new_start;
    finish = new_finish;
    end_of_storage = new_start + new_capacity;
  }
}

template <class T, class Alloc>
template <class Integer>
void vector<T, Alloc>::insert_aux(iterator position, Integer n,
                                  const_reference value, std::true_type) {
  if (n == 0) return;

  if (end_of_storage - finish >= n) {  // enough capacity
    difference_type num_of_elem_after = finish - position;

    if (num_of_elem_after >= n) {
      // perhaps call (non-trivial) destructors for
      // num_of_elem_after times
      sup::uninitialized_copy(finish - n, finish, finish);
      std::copy_backward(position, finish - n, finish);
      std::fill(position, position + n, value);

      finish += n;
    } else {
      // would call (non-trivial) destructors for num_of_elem_after
      // times
      sup::uninitialized_copy(position, finish, finish - num_of_elem_after + n);
      std::fill(position, position + num_of_elem_after, value);
      sup::uninitialized_fill(finish, finish - num_of_elem_after + n, value);

      finish += n;
    }
  } else {  // not enough capacity
    size_type old_size = this->size();

    size_type new_capacity =
        2 * old_size >= (old_size + n) ? 2 * old_size : (old_size + n);
    iterator new_start = data_allocator::allocate(new_capacity);
    iterator new_finish = new_start;

    try {
      // use uninitialized_copy at the new space
      new_finish = sup::uninitialized_copy(start, position, new_finish);
      new_finish = sup::uninitialized_fill_n(new_finish, n, value);
      new_finish = sup::uninitialized_copy(position, finish, new_finish);
    } catch (...) {
      sup::_destroy(new_start, new_finish);
      data_allocator::deallocate(new_start, new_capacity);
      throw;
    }
    this->deallocate();

    start = new_start;
    finish = new_finish;
    end_of_storage = new_start + new_capacity;
  }
}

}  // namespace sup
#endif