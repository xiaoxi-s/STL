#include <gtest/gtest.h>

#include <cstdlib> 
#include <ctime>

#include "../../src/algorithms/sstl_sort.hpp"

namespace algorithm_sort_test {
TEST(sort_test, sort_test) {
  int n = 10;
  int array[] = {6, 7, 1, 10, 3, 4, 8, 2, 5, 9};

  sup::sort(array, array+n);
  for (int i = 0; i < n - 1; ++i)
    EXPECT_TRUE(array[i] < array[i+1]);
}

TEST(sort_test, sort_long_random_numbers_test) {
  int array[100000];
  int n = 100000;
  srand(int(time(0)));
  for (int i = 0; i < n; ++i) {
    array[i] = rand();
  }
  sup::sort(array, array + n);
  for (int i = 0; i < n - 1; ++i) {
    EXPECT_TRUE(array[i] <= array[i+1]);
  }
}

} // namespace algorithm_sort_test 

