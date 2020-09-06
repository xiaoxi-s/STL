#include <vector>
#include <gtest/gtest.h>

#include "../src/sstl_uninitialized.hpp"

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

TEST(uninitialized_fill, vector_iterator){
  std::vector<int> vec(10, 1); // the vector contains 10 1s
  std::vector<int>::iterator start = vec.begin();
  std::vector<int>::iterator end = vec.end();

  sup::uninitialized_fill(start, end, 0);

  // test for all values are 0
  for(int i = 0; i < 10; ++i) {
    EXPECT_TRUE(vec[i] == 0);
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
  EXPECT_TRUE(sup::uninitialized_copy(start, vec.end(), p) == p + 10);
  for(int i = 0; i < 10; ++i) {
    EXPECT_TRUE(p[i] == 1);
  }
}

TEST(uninitialized_copy, write_to_vector_iterator) {
  int array[10];
  std::vector<int> vec(10, 1); // the vector contains 10 1s
  std::vector<int>::iterator start = vec.begin();

  int *p = array;
  for(int i= 0; i < 10; ++i) {
    p[i] = 0;
  }

  // test write to the elements pointed by iterator & return value
  EXPECT_TRUE(sup::uninitialized_copy(p, p+ 10 , start) == vec.end());
  for(int i = 0; i < 10; ++i) {
    EXPECT_TRUE(vec[i] == 0);
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

TEST(uninitialized_fill_n, write_to_vector_iterator) {
  std::vector<int> vec(10, 1); // the vector contains 10 1s

  // test write to the elements pointed by iterator & return value
  EXPECT_TRUE(sup::uninitialized_fill_n(vec.begin(), 10 , 0) == vec.end());
  for(int i = 0; i < 10; ++i) {
    EXPECT_TRUE(vec[i] == 0);
  }
}
}