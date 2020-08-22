#include <gtest/gtest.h>

#include "../src/hello.cpp"

TEST(ExampleTests, hi) {
    hello();
    EXPECT_TRUE(true);
}