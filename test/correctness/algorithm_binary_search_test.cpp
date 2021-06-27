#include <gtest/gtest.h>
#include <stdlib.h>

#include "../../src/algorithms/sstl_binary_operations.hpp"

namespace algorithm_binary_search_test {

TEST(binary_search_int_test, return_value_test) {
  int n = 10;
  int array[] = {0, 1, 2, 3, 4, 15, 17, 20, 24, 30};
  int query_val[] = {-5, -4, -2, -1, 10, 16, 23, 27, 40, 50};
  for (int i = 0; i < n; ++i) {
    EXPECT_TRUE(sup::binary_search(array, array + 10, array[i]));
  }

  for (int i = 0; i < n; ++i) {
    EXPECT_FALSE(sup::binary_search(array, array + 10, query_val[i]));
  }
}

TEST(lower_bound_int_test, return_value_test) {
  int n = 10;
  int array[] = {0, 1, 2, 3, 4, 15, 17, 20, 24, 30};
  // values in array
  for (int i = 0; i < n; ++i) {
    EXPECT_TRUE(sup::lower_bound(array, array + 10, array[i]) == array+i);
  }

  EXPECT_TRUE(sup::lower_bound(array, array+10, -1) == array);
  EXPECT_TRUE(sup::lower_bound(array, array+10, 10) == array + 5);
  EXPECT_TRUE(sup::lower_bound(array, array+10, 15) == array + 5);
  EXPECT_TRUE(sup::lower_bound(array, array+10, 60) == array + 10);
}

TEST(upper_bound_int_test, return_value_test) {
  int n = 10;
  int array[] = {0, 1, 2, 3, 4, 15, 17, 20, 24, 30};
  // values in array
  for (int i = 0; i < n; ++i) {
    EXPECT_TRUE(sup::upper_bound(array, array + 10, array[i]) == array+i+1);
  }

  EXPECT_TRUE(sup::upper_bound(array, array+10, -1) == array);
  EXPECT_TRUE(sup::upper_bound(array, array+10, 10) == array + 5);
  EXPECT_TRUE(sup::upper_bound(array, array+10, 15) == array + 6);
  EXPECT_TRUE(sup::upper_bound(array, array+10, 60) == array + 10);
}

TEST(equal_range_int_test, return_value_test) {
  int n = 10;
  int array[] = {0, 1, 2, 3, 4, 15, 17, 20, 24, 30};
  int query_val[] = {-5, -4, -2, -1, 10, 16, 23, 27, 40, 50};
  std::pair<int*, int*> pp;
  
  // values in array
  for (int i = 0; i < n; ++i) {
    pp = sup::equal_range(array, array+10, array[i]);
    EXPECT_TRUE(pp.first == array + i);
    EXPECT_TRUE(pp.second == array + i + 1);
  }
  pp = sup::equal_range(array, array + 10, -1);
  EXPECT_TRUE(pp.first == array);
  EXPECT_TRUE(pp.second == array);
  pp = sup::equal_range(array, array+10, 10);
  EXPECT_TRUE(pp.first == array + 5);
  EXPECT_TRUE(pp.second == array + 5);

  pp = sup::equal_range(array, array+10, 15);
  EXPECT_TRUE(pp.first == array + 5);
  EXPECT_TRUE(pp.second == array + 6);
  
  pp = sup::equal_range(array, array+10, 60);
  EXPECT_TRUE(pp.first == array + 10);
  EXPECT_TRUE(pp.second == array + 10);
}

}  // namespace algorithm_binary_search_test