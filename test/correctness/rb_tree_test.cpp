#include <gtest/gtest.h>

#include <stdlib.h>

#include "../../src/sstl_rb_tree.hpp"
#include <utility>
#include <string>


namespace rb_tree_int_str_test {

template<class T1, class T2>
struct get_first {
  const T1& operator()(const std::pair<T1, T2>& p) {
    return p.first;
  }
};

TEST(rb_tree_int_str_test, construct) {
  sup::rb_tree<int, std::pair<int, std::string>, get_first<int, std::string>, std::less<int>> t;
}

TEST(rb_tree_int_str_test, insert_unique_simple_and_size) {
  int a[] = {6, 4, 8, 5, 7, 2, 9, 1, 0, 3};
  int n = 10;
  std::string str = "123";
  sup::rb_tree<int, std::pair<int, std::string>, get_first<int, std::string>, std::less<int>> t;
  for (int i = 0 ; i<n; ++i)
    t.insert_unique(std::make_pair(a[i], str));
  
  // does the same insersion again
  for (int i = 0 ; i<n; ++i)
    t.insert_unique(std::make_pair(a[i], str));
  printf("%ld\n", t.size());
  EXPECT_TRUE(t.size() == 10); // the size should be only 10
}

TEST(rb_tree_int_str_test, insert_equal_simple_and_size) {
  int a[] = {6, 4, 8, 5, 7, 2, 9, 1, 0, 3};
  int n = 10;
  std::string str = "123";
  sup::rb_tree<int, std::pair<int, std::string>, 
    get_first<int, std::string>, std::less<int>> t;

  for (int i = 0 ; i<n; ++i)
    t.insert_equal(std::make_pair(a[i], str));

  // does the same insersion again
  for (int i = 0 ; i<n; ++i)
    t.insert_equal(std::make_pair(a[i], str));

  EXPECT_TRUE(t.size() == 20); // the size should be 20 this time
}

TEST(rb_tree_int_str_test, copy_construct) {
  sup::rb_tree<int, std::pair<int, std::string>, 
    get_first<int, std::string>, std::less<int>> t1;
  
  int a[] = {6, 4, 8, 5, 7, 2, 9, 1, 0, 3};
  int n = 10;
  std::string str = "123";
  for (int i = 0 ; i<n; ++i)
    t1.insert_unique(std::make_pair(a[i], str));
  
  sup::rb_tree<int, std::pair<int, std::string>, 
    get_first<int, std::string>, std::less<int>> t2(t1);
  
  auto it1 = t1.begin();
  auto it2 = t2.begin();
  EXPECT_TRUE(t1.size() == t2.size());

  // same values
  while (it1 != t1.end()) {
    EXPECT_TRUE(*it1 == *it2);
    ++it1;
    ++it2;
  }

  // erase all
  for (int i = 0 ; i<n; ++i)
    t2.erase(a[i]);

  EXPECT_TRUE(t2.size() == 0);

  // check t2 could be inserted again
  for (int i = 0 ; i<n; ++i)
    t2.insert_unique(std::make_pair(a[i], str));
  
  it1 = t1.begin();
  it2 = t2.begin();
  while (it1 != t1.end()) {
    EXPECT_TRUE(*it1 == *it2);
    ++it1;
    ++it2;
  }
}

TEST(rb_tree_int_str_test, equal_operator_overloading) {
  sup::rb_tree<int, std::pair<int, std::string>, 
    get_first<int, std::string>, std::less<int>> t1;
  
  int a[] = {6, 4, 8, 5, 7, 2, 9, 1, 0, 3};
  int n = 10;
  std::string str = "123";
  for (int i = 0 ; i<n; ++i)
    t1.insert_unique(std::make_pair(a[i], str));
  
  sup::rb_tree<int, std::pair<int, std::string>, 
    get_first<int, std::string>, std::less<int>> t2;

  // use = operator
  t2 = t1;
  
  auto it1 = t1.begin();
  auto it2 = t2.begin();
  EXPECT_TRUE(t1.size() == t2.size());

  // same values
  while (it1 != t1.end()) {
    EXPECT_TRUE(*it1 == *it2);
    ++it1;
    ++it2;
  }

  // erase all
  for (int i = 0 ; i<n; ++i)
    t2.erase(a[i]);

  EXPECT_TRUE(t2.size() == 0);

  // check t2 could be inserted again
  for (int i = 0 ; i<n; ++i)
    t2.insert_unique(std::make_pair(a[i], str));
  
  it1 = t1.begin();
  it2 = t2.begin();
  while (it1 != t1.end()) {
    EXPECT_TRUE(*it1 == *it2);
    ++it1;
    ++it2;
  }
}

TEST(rb_tree_int_str_test, insert_unique_range_and_size) {
  int a[] = {6, 4, 8, 5, 7, 2, 9, 1, 0, 3};
  int n = 10;
  std::string str = "123";
  std::pair<int, std::string> pairs[10];

  // create value type
  for (int i = 0; i < n; ++i) {
    pairs[i] = std::make_pair(a[i], str);
  }

  sup::rb_tree<int, std::pair<int, std::string>, 
    get_first<int, std::string>, std::less<int>> t;

  t.insert_unique(pairs, pairs + n);

  sup::rb_tree<int, std::pair<int, std::string>, 
    get_first<int, std::string>, std::less<int>>::iterator 
    it = t.begin();
  
  // pre order traversal
  for (int i = 0; i < n; ++i, ++it) {
    EXPECT_TRUE((*it).first == i);
  }
  printf("%ld\n", t.size());
  EXPECT_TRUE(t.size() == 10); // the size should be only 10
}

TEST(rb_tree_int_str_test, insert_equal_range_and_size) {
  int a[] = {6, 4, 8, 5, 7, 2, 9, 1, 0, 3};
  int n = 10;
  std::string str = "123";
  std::pair<int, std::string> pairs[10];
  
  // create value type
  for (int i = 0; i < n; ++i) {
    pairs[i] = std::make_pair(a[i], str);
  }

  sup::rb_tree<int, std::pair<int, std::string>, 
    get_first<int, std::string>, std::less<int>> t;

  t.insert_equal(pairs, pairs+n);

  // does the same insersion again
  t.insert_equal(pairs, pairs+n);
  sup::rb_tree<int, std::pair<int, std::string>, 
    get_first<int, std::string>, std::less<int>>::iterator 
    it = t.begin();
  
  for (int i = 0; i < n; ++i) {
    EXPECT_TRUE((*it).first == i);
    ++it;
    EXPECT_TRUE((*it).first == i);
    ++it;
  }

  EXPECT_TRUE(t.size() == 20); // the size should be 20 this time
}

TEST(rb_tree_int_str_test, insert_unique_with_hint) {
  int n = 10;
  std::string str = "123";
  sup::rb_tree<int, std::pair<int, std::string>, 
    get_first<int, std::string>, std::less<int>> t;
  std::pair<int, std::string> pairs[10];
  
  // create value type
  for (int i = 0; i < n; ++i) {
    pairs[i] = std::make_pair(i, str);
  }
  t.insert_unique(pairs[0]);
  // waiting for implementation
  for (int i = 1; i < n; ++i) {
    t.insert_unique(t.lower_bound(i-1), pairs[i]);
  }
  sup::rb_tree<int, std::pair<int, std::string>, 
    get_first<int, std::string>, std::less<int>>::iterator it=t.begin();

  // make sure this is a binary search tree
  for(int i = 0; it!=t.end(); ++it, ++i) {
    EXPECT_TRUE(it == t.lower_bound(i));
  }
}

TEST(rb_tree_int_str_test, insert_equal_with_hint) {
  int a[] = {6, 4, 8, 5, 7, 2, 9, 1, 0, 3};
  int n = 10;
  std::string str = "123";
  sup::rb_tree<int, std::pair<int, std::string>, 
    get_first<int, std::string>, std::less<int>> t;
  sup::rb_tree<int, std::pair<int, std::string>, 
    get_first<int, std::string>, std::less<int>>::iterator it=t.begin(), tmp;
  
  std::pair<int, std::string> pairs[10];
  
  // create value type
  for (int i = 0; i < n; ++i) {
    pairs[i] = std::make_pair(i, str);
  }
  t.insert_equal(pairs[0]);
  // waiting for implementation
  for (int i = 1; i < n; ++i) {
    t.insert_equal(t.lower_bound(i-1), pairs[i]);
  }
  t.insert_equal(pairs[0]);
  for (int i = 1; i < n; ++i) {
    t.insert_equal(t.lower_bound(i-1), pairs[i]);
  }

  // make sure this is a binary search tree
  for(int i = 0; it!=t.end(); ++i) {
    EXPECT_TRUE(it == t.lower_bound(i));
    ++it;
    ++it;
  }
  // waiting for implementation
}

TEST(rb_tree_int_str_test, find) {
  int a[] = {6, 4, 8, 5, 7, 2, 9, 1, 0, 3};
  int n = 10;
  sup::rb_tree<int, std::pair<int, std::string>, 
    get_first<int, std::string>, std::less<int>> t;

  // the value is simply the key converted into string
  for (int i = 0 ; i<n; ++i) {
    std::string str = std::to_string(a[i]);
    t.insert_unique(std::make_pair(a[i], str));
  }
  
  for (int i = 0; i < n; ++i) {
    auto it = t.find(a[i]);
    EXPECT_TRUE(*it == std::make_pair(a[i], std::to_string(a[i])));
  }
}

TEST(rb_tree_int_str_test, count) {
  int a[] = {6, 4, 8, 5, 7, 2, 9, 1, 0, 3};
  int n = 10;
  sup::rb_tree<int, std::pair<int, std::string>, 
    get_first<int, std::string>, std::less<int>> t;

  // insert once
  for (int i = 0 ; i<n; ++i) {
    std::string str = std::to_string(a[i]);
    t.insert_unique(std::make_pair(a[i], str));
  }
  
  for (int i = 0; i < n; ++i) {
    EXPECT_TRUE(t.count(a[i]) == 1);
  }
  
  // insert twice
  for (int i = 0 ; i<n; ++i) {
    std::string str = std::to_string(a[i]);
    t.insert_equal(std::make_pair(a[i], str));
  }

  for (int i = 0; i < n; ++i) {
    EXPECT_TRUE(t.count(a[i]) == 2);
  }
}

TEST(rb_tree_int_str_test, lower_upper_bound) {
  int a[] = {6, 4, 8, 5, 7, 2, 9, 1, 0, 3};
  int n = 10;
  sup::rb_tree<int, std::pair<int, std::string>, 
    get_first<int, std::string>, std::less<int>> t;

  // insert once
  for (int i = 0 ; i<n; ++i) {
    std::string str = std::to_string(a[i]);
    t.insert_unique(std::make_pair(a[i], str));
  }
  
  EXPECT_TRUE(t.lower_bound(0) == t.find(0));
  EXPECT_TRUE(t.lower_bound(8) == t.find(8));
  EXPECT_TRUE(t.lower_bound(11) == t.end());
  EXPECT_TRUE(t.lower_bound(-1) == t.begin());

  EXPECT_TRUE(t.upper_bound(0) == t.find(1));
  EXPECT_TRUE(t.upper_bound(8) == t.find(9));
  EXPECT_TRUE(t.upper_bound(11) == t.end());
  EXPECT_TRUE(t.upper_bound(-1) == t.begin());

  // insert twice
  for (int i = 0 ; i<n; ++i) {
    std::string str = std::to_string(a[i]);
    t.insert_equal(std::make_pair(a[i], str));
  }
  sup::rb_tree<int, std::pair<int, std::string>, 
    get_first<int, std::string>, std::less<int>>::iterator it;

  for (int i = 0; i<n; ++i) {
    // find the first occurance:
    for (it=t.begin(); it!=t.end();++it) {
      if ((*it).first == i)
        break;
    }
    // test whether lower bound returns the first occurance
    EXPECT_TRUE(it == t.lower_bound(i));
    // test whether upper bound returns the next occurance
    int j = i-1;
    EXPECT_TRUE(it == t.upper_bound(j));

  }
}

TEST(rb_tree_int_str_test, clear) {
  int a[] = {6, 4, 8, 5, 7, 2, 9, 1, 0, 3};
  int n = 10;
  sup::rb_tree<int, std::pair<int, std::string>, 
    get_first<int, std::string>, std::less<int>> t;

  for (int i = 0 ; i<n; ++i) {
    std::string str = std::to_string(a[i]);
    t.insert_unique(std::make_pair(a[i], str));
  }
}

TEST(rb_tree_int_str_test, erase) {
  int a[] = {6, 4, 8, 5, 7, 2, 9, 1, 0, 3};
  int n = 10;
  sup::rb_tree<int, std::pair<int, std::string>, 
    get_first<int, std::string>, std::less<int>> t;

  for (int i = 0 ; i<n; ++i) {
    std::string str = std::to_string(a[i]);
    t.insert_unique(std::make_pair(a[i], str));
  }

  for(int i = 0; i < n; ++i) {
    t.erase(a[i]);
    EXPECT_TRUE(t.size() == n - i - 1);
  }
  EXPECT_TRUE(t.empty());

  for (int i = 0 ; i<n; ++i) {
    std::string str = std::to_string(a[i]);
    t.insert_unique(std::make_pair(a[i], str));
  }

  for(int i = 0; i < n; ++i) {
    t.erase(a[i]);
    EXPECT_TRUE(t.size() == n - i - 1);
  }
  EXPECT_TRUE(t.empty());
}
}