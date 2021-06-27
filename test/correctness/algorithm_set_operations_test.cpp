#include <gtest/gtest.h>
#include <stdlib.h>

#include "../../src/algorithms/sstl_set_operations.hpp"

namespace algorithm_set_operations_test {
TEST(set_union_test, return_value_test) {
  int n1 = 10;
  int n2 = 6;
  int array1[] = {0, 1, 2, 3, 4, 15, 17, 20, 24, 30};
  int array2[] = {2, 3, 4, 5, 6, 7};
  int expected_output[] = {0, 1, 2, 3, 4, 5, 6, 7, 15, 17, 20, 24, 30};
  int output[20];
  int* end = 
    sup::set_union(array1, array1 + n1, array2, array2 + n2, output);
  EXPECT_TRUE(end == output + 13);
  
  for (int i = 0; i < 13; ++i) {
    EXPECT_TRUE(output[i] == expected_output[i]);
  }
}

TEST(set_intersection_test, return_value_test) {
  int n1 = 10;
  int n2 = 6;
  int array1[] = {0, 1, 2, 3, 4, 15, 17, 20, 24, 30};
  int array2[] = {2, 3, 4, 5, 6, 7};
  int expected_output[] = {2, 3, 4};
  int output[20];
  int* end = 
    sup::set_intersection(array1, array1 + n1, array2, array2 + n2, output);
  EXPECT_TRUE(end == output + 3);
  
  for (int i = 0; i < 3; ++i) {
    EXPECT_TRUE(output[i] == expected_output[i]);
  }
}

TEST(set_difference_test, return_value_test) {
  int n1 = 10;
  int n2 = 6;
  int array1[] = {0, 1, 2, 3, 4, 15, 17, 20, 24, 30};
  int array2[] = {2, 3, 4, 5, 6, 7};
  int expected_output[] = {0, 1, 15, 17, 20, 24, 30};
  int output[20];
  int* end = 
    sup::set_difference(array1, array1 + n1, array2, array2 + n2, output);
  EXPECT_TRUE(end == output + 7);
  
  for (int i = 0; i < 7; ++i) {
    EXPECT_TRUE(output[i] == expected_output[i]);
  }
}

}