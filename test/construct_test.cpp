#include <gtest/gtest.h>
#include <stdlib.h>

#include "../src/sstl_construct.hpp"

TEST(Construct_test, small_try) {
  int * p = (int*) malloc (sizeof(int));
  sup::construct<int>(p, 1);

  EXPECT_TRUE(false);
}