#ifdef _SSTL_CONSTRUCT_H
#define _SSTL_CONSTRUCT_H
#include <new>  // to use placement new

/**
 * All credits for STL Annotated Sources by Jie Hou.
 *
 * @author Xiaoxi Sun
 **/

namespace sup {

/**
 * Constructor bsed on type and value
 * 
 * @param T1
 * @param T2
 **/
template <class T1, class T2>
inline void construct(T1* p, const T2& value) {
  new (p) T1(value);  // placement new
}

// two destory()
/**
 * Destory the object pointed by pointer
 *
 * @param pointer the pointer to the object being destoryed
 **/
template <class T>
inline void destory(T* pointer) {
  pointer->~T();
}
/**
 * Destory objects based on the range [first, last)
 *
 *
 * @param first the first object being destoryed
 * @param last the first object that will not be destoryed after first
 **/
template <class ForwardIterator, class T>
inline void destory(ForwardIterator first, ForwardIterator last) {
  // use __type_traits<T> to determine the existence of the trivial destructor
  // of such type.
  typedef typename __type_traits<T>::has_trivial_destructor
      trivial_destructor;  // this would be explained later

  __destory(first, last, value_type(first));
}
// destory based on the existence of trivial destructor
/**
 * If there is a very important (non-trivial) destructor
 *
 * @param first the first object being destoryed
 * @param last the first object that will not be destoryed after first
 * @param __false_type - denoting only the type rather than value because value
 *is not used
 **/
template <class ForwardIterator>
inline void __destory_aux(ForwardIterator first, ForwardIterator last,
                          __false_type) {
  for (; first < last; ++first) destory(&*first);
}
/**
 * If there is a trivial destructor, nothing is needed to do before release
 *space
 *
 * @param first the first object being destoryed
 * @param last the first object that will not be destoryed after first
 * @param __true_type - denoting only the type rather than value because value
 *is not used
 **/
template <class ForwardIterator>
inline void __destory_aux(ForwardIterator first, ForwardIterator last,
                          __true_type) {}

// specialized destory for char and wchar_t
inline void destory(char*, char*) {}
// wide character for unicode encoding
inline void destory(wchar_t*, wchar_t*) {}

}  // namespace sup
#endif