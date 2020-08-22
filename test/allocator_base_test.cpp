#include <gtest/gtest.h>

#include "../src/sstl_allocator_base.hpp"

TEST(Allocator_base_test, small_try) {
    sup::allocator_base<int> a;

    EXPECT_TRUE(false);
}