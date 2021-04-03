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