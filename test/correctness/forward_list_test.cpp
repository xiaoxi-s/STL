#include <gtest/gtest.h>

#include "../../src/sstl_forward_list.hpp"

namespace forward_list_int_test { 

TEST(forward_list_int_test, empty) {
  sup::forward_list<int> l;
  EXPECT_TRUE(l.empty());
}

TEST(forward_list_int_test, push_front_front_and_iterator) {
  sup::forward_list<int> l;
  for (int i = 0 ; i < 10; ++i) {
    l.push_front(i);
    EXPECT_TRUE(l.front() == i);
  }

  sup::forward_list<int>::iterator it = l.begin();

  for (int i = 9; it != l.end(); ++it, --i) {
    EXPECT_TRUE(*it == i);
  }
}

TEST(forward_list_int_test, pop_front) {
  sup::forward_list<int> l;
  for (int i = 0 ; i < 10; ++i) {
    l.push_front(i);
  }

  sup::forward_list<int>::iterator it = l.begin();;

  for (int i = 9; it != l.end(); --i) {
    ++it;
    l.pop_front();
  }

  EXPECT_TRUE(l.empty());
}

TEST(forward_list_int_test, size) {
  sup::forward_list<int> l;
  for (int i = 0 ; i < 10; ++i) {
    l.push_front(i);
    EXPECT_TRUE(l.size() == i + 1);
  }
}

TEST(forward_list_int_test, swap) {
  sup::forward_list<int> l1;
  for (int i = 0 ; i < 10; ++i) {
    l1.push_front(i);
  }

  sup::forward_list<int> l2;
  l1.swap(l2);

  EXPECT_TRUE(l1.size() == 0);
  EXPECT_TRUE(l2.size() == 10);
}

TEST(forward_list_int_test, clear) {
  sup::forward_list<int> l;
  
  for (int i = 0 ; i < 10; ++i) 
    l.push_front(i);
  
  EXPECT_TRUE(l.size() == 10);

  l.clear();
  EXPECT_TRUE(l.size() == 0 && l.empty());
}

}