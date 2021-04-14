#include <gtest/gtest.h>

#include "../../src/sstl_set.hpp"

// As most of the methods are wrapper methods for 
// the RB tree, the test is mainly to ensure successfuly 
// compiling and provide basic sanity checks for the 
// implementation

namespace set_int_test {

TEST(set_int_test, construct_set) {
  int n = 5;
  int a[] = {0, 1, 2, 3, 4};
  sup::set<int> s1;
  EXPECT_TRUE(s1.empty());
  sup::set<int> s2(a, a + n); 
  EXPECT_TRUE(s2.size() == 5);
}

TEST(set_int_test, iterator_and_reverse_iterator) {
  int n = 5;
  int i = 0;
  int a[] = {0, 1, 2, 3, 4};
  sup::set<int> s(a, a + n);

  // test iterator
  sup::set<int>::iterator it = s.begin();
  while(it != s.end()) {
    EXPECT_TRUE(*it == a[i]);
    ++it;
    ++i;
  }

  // test reverse iterator
  i = 0;
  sup::set<int>::reverse_iterator rit = s.rbegin();
  while(rit != s.rend()) {
    EXPECT_TRUE(*rit == a[n-i-1]);
    ++rit;
    ++i;
  }
}

TEST(set_int_test, insert) {
  int n = 10;
  sup::set<int> s;
  for (int i = 0; i < n; ++i) {
    s.insert(i);
  }
  EXPECT_TRUE(s.size() == n);
}
}