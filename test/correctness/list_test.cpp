#include <gtest/gtest.h>

#include "../../src/sstl_list.hpp"

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


TEST(list_int_test, erase_range) {
  sup::list<int> l;
  for (int i = 0; i < 10; ++i) {
    l.push_back(i);
  }

  sup::list<int>::iterator first = l.begin();
  sup::list<int>::iterator last = l.begin();

  for (int i = 0; i < 3; ++i) ++first;
  for (int i = 0; i < 6; ++i)  ++last;

  last = l.erase(first, last);

  EXPECT_TRUE(l.size() == 7);
  first = l.begin();

  for (int i = 0; first != last; ++first, ++i) {
    EXPECT_TRUE(i == *first);
  }

  for (int i = 6; first != l.end(); ++first, ++i) {
    EXPECT_TRUE(i == *first);
  }
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

TEST(list_int_test, splice_list_test) {
  sup::list<int> l1;
  sup::list<int> l2;
  for (int i = 0; i < 10; ++i) 
    l1.push_back(i);

  for (int i = 20; i < 25; ++i)
    l2.push_back(i);

  l1.splice(l1.begin(), l2);
  EXPECT_TRUE(l2.empty());

  sup::list<int>::iterator it = l1.begin();
  
  // list "l2"
  for (int i = 20; i < 25; ++i) {
    EXPECT_TRUE(*it == i);
    ++it; 
  }
  
  // list "l1"
  for (int i = 0; i < 10; ++i) {
    EXPECT_TRUE(*it == i);
    ++it;
  }
}

TEST(list_int_test, splice_single_element_test) {
  sup::list<int> l1;
  sup::list<int> l2;
  for (int i = 0; i < 10; ++i) 
    l1.push_back(i);

  for (int i = 20; i < 25; ++i)
    l2.push_back(i);

  sup::list<int>::iterator it2 = l2.begin();
  l1.splice(l1.begin(), l2, it2);
  EXPECT_TRUE(l2.size() == 4);

  sup::list<int>::iterator it1 = l1.begin();
  EXPECT_TRUE(*it1 == 20);
  ++it1;
  for (int i = 0;it1 != l1.end(); ++it1, ++i) {
    EXPECT_TRUE(*it1 == i);
  }
}

TEST(list_int_test, splice_range_test) {
  sup::list<int> l1;
  sup::list<int> l2;
  for (int i = 0; i < 10; ++i) 
    l1.push_back(i);

  for (int i = 20; i < 25; ++i)
    l2.push_back(i);

  sup::list<int>::iterator it2 = l2.begin();
  ++it2;
  ++it2; 

  l1.splice(l1.begin(), l2, l2.begin(), it2);
  EXPECT_TRUE(l2.size() == 3);
  sup::list<int>::iterator it1 = l1.begin();
  
  for (int i = 20; i < 22; ++it1, ++i) {
    EXPECT_TRUE(*it1 == i);
  }
  for (int i = 0; i < 10; ++it1, ++i) {
    EXPECT_TRUE(*it1 == i);
  }
}

TEST(list_int_test, merge) {
  sup::list<int> l1;
  sup::list<int> l2;
  for (int i = 0; i < 10; i += 2) 
    l1.push_back(i);
  for (int i = 1; i < 10; i += 2)
    l2.push_back(i);

  l1.merge(l2);
  EXPECT_TRUE(l2.empty());

  sup::list<int>::iterator it = l1.begin();
  for (int i = 0; i < 10; ++it, ++i) {
    EXPECT_TRUE(*it == i);
  }
}

TEST(list_int_test, merge_with_comparator) {
  sup::list<int> l1;
  sup::list<int> l2;

  // create two decreasing array
  for (int i = 9; i >= 0; --i) 
    l1.push_back(i);
  for (int i = 24; i >= 20; --i)
    l2.push_back(i);

  // merge two lists in an decreasing order
  l1.merge(l2, [](int a, int b) {
            return (a > b);});
  EXPECT_TRUE(l2.empty());

  sup::list<int>::iterator it = l1.begin();
  for(int i = 24; i >= 20; ++it, --i) {
    EXPECT_TRUE(*it == i);
  }

  for(int i = 9; i >= 0; ++it, --i) {
    EXPECT_TRUE(*it == i);
  }
}

TEST(list_int_test, reverse) {
  sup::list<int> l1;

  for (int i = 0; i < 10; ++i) 
    l1.push_back(i);

  l1.reverse();

  sup::list<int>::iterator it = l1.begin();
  for(int i = 9; it != l1.end(); ++it, --i) {
    EXPECT_TRUE(*it == i);
  }
}


TEST(list_int_test, assign_with_range) {
  sup::list<int> l1;
  sup::list<int> l2;

  for (int i = 0; i < 10; ++i) 
    l1.push_back(i);
  for (int i = 20; i < 25; ++i)
    l2.push_back(i);

  l1.assign(l2.begin(), l2.end());
  
  sup::list<int>::iterator it2 = l2.begin();
  sup::list<int>::iterator it1 = l1.begin();
  
  // l2 does not change
  // and l1 contains the same sequence (not same nodes) as l2
  for (int i = 20; it2 != l2.end(); ++it2, ++it1, ++i) {
    EXPECT_TRUE(*it2 == i);
    EXPECT_TRUE(*it1 == i);
  }
}

TEST(list_int_test, assign_with_values) {
  sup::list<int> l1;

  for (int i = 0; i < 10; ++i) 
    l1.push_back(i);

  l1.assign(5, 999);

  sup::list<int>::iterator it1 = l1.begin();

  EXPECT_TRUE(l1.size() == 5);
  for (;it1 != l1.end(); ++it1) {
    EXPECT_TRUE(*it1 == 999);
  }
}

TEST(list_int_test, unique_simple) {
  sup::list<int> l1;

  l1.assign(5, 999);
  l1.unique();
  EXPECT_TRUE(l1.size() == 1);
  EXPECT_TRUE(*l1.begin() == 999);
}

TEST(list_int_test, unique_predicate) {
  sup::list<int> l1;
  for (int i = 0; i < 10; ++i) 
    l1.push_back(i);


  // false unique!!!
  l1.unique([](int a, int b) {
            return (a != b);});
  EXPECT_TRUE(l1.size() == 1);
}

TEST(list_int_test, sort_simple) {
  sup::list<int> l1;
  for (int i = 9; i >= 0; --i) 
    l1.push_back(i);

  l1.sort();

  sup::list<int>::iterator it1 = l1.begin();
  for (int i = 0; i < 10; ++it1, ++i) {
    EXPECT_TRUE(*it1 == i);
  }
}

TEST(list_int_test, sort_predicate) {
  sup::list<int> l1;
  for (int i = 0; i<10; ++i) 
    l1.push_back(i);

  // sort in an decreasing order
  l1.sort([](int a, int b) {
            return (a > b);});

  sup::list<int>::iterator it1 = l1.begin();
  for (int i = 9; i >= 0; ++it1, --i)  {
    EXPECT_TRUE(*it1 == i);
  }
}

TEST(list_int_test, resize) {
  sup::list<int> l1;
  for (int i = 0; i<10; ++i) 
    l1.push_back(i);

  l1.resize(3, 999);

  EXPECT_TRUE(l1.size() == 3);

  sup::list<int>::iterator it1 = l1.begin();
  for (int i = 0; it1 != l1.end(); ++it1, ++i) {
    EXPECT_TRUE(*it1 == 999);
  }
}
}
