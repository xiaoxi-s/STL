#include <gtest/gtest.h>
#include <vector>
#include <string>

#include "../../src/sstl_uninitialized.hpp"

namespace uninitialized_test {

TEST(uninitialized_fill, native_pointer) {
  int array[10];
  int *p = array;
  for(int i= 0; i < 10; ++i) {
    p[i] = 1;
  }
  sup::uninitialized_fill(p, p + 10, 0);
  for(int i = 0; i < 10; ++i) {
    EXPECT_TRUE(p[i] == 0);
  }
}

TEST(uninitialized_fill, write_to_string_array){
  std::string ss[10];
  sup::uninitialized_fill(ss, ss+10, "abc");

  // test for all values are 0
  for(int i = 0; i < 10; ++i) {
    EXPECT_TRUE(ss[i] == "abc");
  }
}

TEST(uninitialized_copy, write_to_native_pointer) {
  int array[10];
  std::vector<int> vec(10, 1); // the vector contains 10 1s
  std::vector<int>::iterator start = vec.begin();

  int *p = array;
  for(int i= 0; i < 10; ++i) {
    p[i] = 0;
  }

  // test write to the elements pointed by native pointer & return value
  EXPECT_TRUE(sup::uninitialized_copy(start, vec.end(), p) == array + 10);
  for(int i = 0; i < 10; ++i) {
    EXPECT_TRUE(p[i] == 1);
  }
}

TEST(uninitialized_copy, write_to_string_array) {
  std::vector<std::string> vec(10, "abc"); // the vector contains 10 1s
  std::vector<std::string>::iterator start = vec.begin();

  std::string ss[10];

  // test write to the elements pointed by iterator & return value
  EXPECT_TRUE(sup::uninitialized_copy(start, start + 10, ss) == ss+10);
  for (int i = 0; i < 10; ++i) {
    EXPECT_TRUE(ss[i] == "abc");
  }
}

TEST (uninitialized_fill_n, write_to_native_pointer) {
  int array[10];
  int *p = array;

  // set all elements to be 0
  for(int i= 0; i < 10; ++i) {
    p[i] = 1;
  }

  // test write to the elements pointed by native pointer & return value
  EXPECT_TRUE(sup::uninitialized_fill_n(p, 10, 0) == p + 10);
  for(int i = 0; i < 10; ++i) {
    EXPECT_TRUE(p[i] == 0);
  }
}

TEST(uninitialized_fill_n, write_to_string_array) {

  std::string s[10];
  // test write to the elements pointed by iterator & return value
  EXPECT_TRUE(sup::uninitialized_fill_n(s, 10 , "abc") == s+10);
  for(int i = 0; i < 10; ++i) {
    EXPECT_TRUE(s[i] == "abc");
  }
}
}