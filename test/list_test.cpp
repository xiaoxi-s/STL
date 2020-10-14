#include <gtest/gtest.h>

#include "../src/sstl_list.hpp"

namespace list_iterator_int_test {
TEST(list_iterator_int_test, test_trait) {
  sup::list<int> l;
  sup::list<int>::iterator l_iterator=l.begin();
  sup::iterator_traits<sup::list<int>::iterator> traits; 
  
}

TEST(list_iterator_int_test, test_increment_decrement) {
  
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

TEST(list_iterator_test, test_increment_decrement) {
  
}


}