#include "../src/hello.cpp"
#include <gtest/gtest.h>

TEST(ExampleTests, hi) {
    hello();
    EXPECT_TRUE(false);
}