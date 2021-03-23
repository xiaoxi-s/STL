#ifndef _SSTL_QUEUE_H
#define _SSTL_QUEUE_H

#include "sstl_deque.hpp"

namespace sup {

template <class T, class Sequence = sup::deque<T>>
class queue {

template <class Type, class Seq>
friend bool operator==(const queue<Type, Seq>& q1, const queue<Type, Seq>& q2);
template <class Type, class Seq>
friend bool operator<(const queue<Type, Seq>& q1, const queue<Type, Seq>& q2);

public:
  typedef typename Sequence::value_type value_type;
  typedef typename Sequence::size_type size_type;
  typedef typename Sequence::reference reference;

protected:
  Sequence c;

public: 
  bool empty() const { return c.empty(); }
  size_type size() const { return c.size(); }
  reference front() {return c.front();}
  void push(const value_type& x) { c.push_back(x); }
  void pop() { c.pop_front(); }
};


// below functions are not tested as deque == is not implemented
template <class Type, class Seq>
bool operator==(const queue<Type, Seq>& q1, const queue<Type, Seq>& q2) {
  return q1.c == q2.c;
}

template <class Type, class Seq>
bool operator<(const queue<Type, Seq>& q1, const queue<Type, Seq>& q2) {
  return q1.c < q2.c;
}

}
#endif
