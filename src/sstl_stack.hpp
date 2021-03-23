#ifndef _SSTL_STACK_H
#define _SSTL_STACK_H

#include "sstl_deque.hpp"

namespace sup {

template <class T, class Sequence = deque<T>>
class stack {

template <class Type, class Seq>
friend bool operator==(const stack<Type, Seq>& stk1, const stack<Type, Seq>& stk2);
template <class Type, class Seq>
friend bool operator<(const stack<Type, Seq>& stk1, const stack<Type, Seq>& stk2);

public:
  typedef typename Sequence::value_type value_type;
  typedef typename Sequence::size_type size_type;
  typedef typename Sequence::reference reference;

protected:
  Sequence c;

public:
  bool empty() const { return c.empty(); }
  size_type size() const { return c.size(); }
  reference top() {return c.back();}
  void push(const value_type& x) { c.push_back(x); }
  void pop() { c.pop_back(); }
};

// the below two operator overloads are not tested because deque == is not implemented
template <class Type, class Seq>
bool operator==(const stack<Type, Seq>& stk1, const stack<Type, Seq>& stk2) {
  return stk1.c == stk2.c;
}

template <class Type, class Seq>
bool operator<(const stack<Type, Seq>& stk1, const stack<Type, Seq>& stk2) {
  return stk1.c < stk2.c;
}

}

#endif
