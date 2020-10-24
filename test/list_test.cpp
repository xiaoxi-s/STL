#include <gtest/gtest.h>

#include "../src/sstl_list.hpp"

namespace list_iterator_int_test {
TEST(list_iterator_int_test, test_trait) {
  sup::list<int> l;
  sup::list<int>::iterator l_iterator=l.begin();
  sup::iterator_traits<sup::list<int>::iterator> traits; 
  
}

TEST(list_iterator_int_test, test_increment_decrement) {
  sup::list<int> l;
  sup::list<int> ::iterator l_it = l.begin();
  EXPECT_TRUE(++l_it == l.end());
}
}

namespace list_int_test {
TEST(list_int_test, insert_erase_one_element_test) {
  sup::list<int> l;

  l.insert(l.begin(), 1);
  l.insert(l.begin(), 2);
  sup::list<int>::iterator l_i = l.begin();
  EXPECT_TRUE(*l_i == 2);
  ++l_i;
  EXPECT_TRUE(*l_i == 1);
  EXPECT_TRUE(l.size() == 2);
  l.erase(l_i);
  l.erase(l.begin());
  EXPECT_TRUE(l.size() == 0);
}

TEST(list_int_test, pop_push_back) {
  sup::list<int> l;
  for (int i = 0; i < 10; ++i) {
    l.push_back(i);
  }

  sup::list<int>::iterator l_it = l.begin();
  EXPECT_TRUE(l.size() == 10);
  for (int i =0; i < 10; ++i) {
    EXPECT_TRUE(*l_it == i);
    ++l_it;
  }

  EXPECT_TRUE(*l.begin() == 0);
  EXPECT_TRUE(*--l.end() == 9);

  EXPECT_TRUE(l.front() == 0);
  EXPECT_TRUE(l.back() == 9);

  for (int i = 0; i < 10; ++i) {
    EXPECT_TRUE(l.back() == 10 - i - 1);
    l.pop_back();
  }
  EXPECT_TRUE(l.size() == 0);
}

TEST(list_int_test, remove_test) {
  sup::list<int> l;
  for (int i = 0; i < 10; ++i) {
    l.push_back(127);
  }
  EXPECT_TRUE(l.size() == 10);
  l.remove(127);
  EXPECT_TRUE(l.size() == 0);
}

TEST(list_int_test, remove_if_test) {
  sup::list<int> l;
  for (int i = 0; i < 10; ++i) {
    l.push_back(i);
  }
  EXPECT_TRUE(l.size() == 10);
  for (int i = 0; i < 10; ++i) {
    l.remove_if([] (int n) {return n%2 == 0;});
  }
  EXPECT_TRUE(l.size() == 5);
}

TEST(list_int_test, test_clear) {
  sup::list<int> l;
  for (int i = 0; i < 10; ++i) {
    l.push_back(i);
  }
  EXPECT_TRUE(l.size() == 10);
  l.clear();
  EXPECT_TRUE(l.size() == 0);

  for (int i = 0; i < 10; ++i) {
    l.push_back(i);
  }
  EXPECT_TRUE(l.size() == 10);
  sup::list<int>::iterator it = l.begin();
    for (int i = 0; i < 10; ++i) {
    EXPECT_TRUE(*it == i) ;
    ++it;
  }
}

TEST(list_int_test, swap_test) {
  sup::list<int> l1;
  sup::list<int> l2;
  for (int i = 0; i < 10; ++i) {
    l1.push_back(i);
  }

  for (int i = 20; i < 25; ++i){
    l2.push_back(i);
  }

  EXPECT_TRUE(l1.size() == 10);
  EXPECT_TRUE(l2.size() == 5);

  l1.swap(l2);

  EXPECT_TRUE(l1.size() == 5);
  EXPECT_TRUE(l2.size() == 10);

  sup::list<int>::iterator it = l2.begin();
  for (int i = 0; i < 10; ++i) {
    EXPECT_TRUE(*it == i);
    ++it;
  }
  it = l1.begin();
  for (int i = 20; i < 25; ++i){
    EXPECT_TRUE(*it == i);
    ++it;
  }
}

}