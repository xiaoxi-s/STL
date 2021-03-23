#include <gtest/gtest.h>

#include "../../src/sstl_list.hpp"
#include "../../src/sstl_stack.hpp"

namespace stack_int_test {

TEST(stack_int_deque_test, empty) {
  sup::stack<int> stk;
  EXPECT_TRUE(stk.empty());
  stk.push(1);
  EXPECT_TRUE(!stk.empty());
}

TEST(stack_int_deque_test, size) {
  sup::stack<int> stk;
  EXPECT_TRUE(stk.size() == 0);
  stk.push(1);
  EXPECT_TRUE(stk.size() == 1);
}

TEST(stack_int_deque_test, push) {
  sup::stack<int> stk;
  EXPECT_TRUE(stk.size() == 0);
  stk.push(1);
  EXPECT_TRUE(stk.top() == 1);
}


TEST(stack_int_deque_test, pop) {
  sup::stack<int> stk;
  stk.push(1);
  stk.pop();
  EXPECT_TRUE(stk.empty());
  EXPECT_TRUE(stk.size() == 0);
}

TEST(stack_int_list_test, empty) {
  sup::stack<int, sup::list<int>> stk;
  EXPECT_TRUE(stk.empty());
  stk.push(1);
  EXPECT_TRUE(!stk.empty());
}

TEST(stack_int_list_test, size) {
  sup::stack<int, sup::list<int>> stk;
  EXPECT_TRUE(stk.size() == 0);
  stk.push(1);
  EXPECT_TRUE(stk.size() == 1);
}

TEST(stack_int_list_test, push) {
  sup::stack<int, sup::list<int>> stk;
  EXPECT_TRUE(stk.size() == 0);
  stk.push(1);
  EXPECT_TRUE(stk.top() == 1);
}

TEST(stack_int_list_test, pop) {
  sup::stack<int, sup::list<int>> stk;
  stk.push(1);
  stk.pop();
  EXPECT_TRUE(stk.empty());
  EXPECT_TRUE(stk.size() == 0);
}
}