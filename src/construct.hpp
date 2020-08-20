#ifdef _CONSTRUCT_H
#define _CONSTRUCT_H
#include <new>

namespace sup {
template <typename T1, typename T2>
inline void construct(T1* p, const T2& value) {}
}  // namespace sup

#endif