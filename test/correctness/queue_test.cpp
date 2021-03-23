#include <gtest/gtest.h>

#include "../../src/sstl_list.hpp"
#include "../../src/sstl_queue.hpp"

namespace queue_int_test {

TEST(queue_int_deque_test, empty) {
  sup::queue<int> q;
  EXPECT_TRUE(q.empty());
  q.push(1);
  EXPECT_TRUE(!q.empty());
}

TEST(queue_int_deque_test, size) {
  sup::queue<int> q;
  EXPECT_TRUE(q.size() == 0);
  q.push(1);
  EXPECT_TRUE(q.size() == 1);
  q.push(2);
  EXPECT_TRUE(q.size() == 2);
}

TEST(queue_int_deque_test, push) {
  sup::queue<int> q;
  EXPECT_TRUE(q.size() == 0);
  q.push(1);
  EXPECT_TRUE(q.front() == 1);
}

TEST(queue_int_deque_test, pop) {
  sup::queue<int> q;
  q.push(1);
  q.pop();
  EXPECT_TRUE(q.empty());
  EXPECT_TRUE(q.size() == 0);
}

TEST(queue_int_list_test, empty) {
  sup::queue<int, sup::list<int>> q;
  EXPECT_TRUE(q.empty());
  q.push(1);
  EXPECT_TRUE(!q.empty());
}

TEST(queue_int_list_test, size) {
  sup::queue<int, sup::list<int>> q;
  EXPECT_TRUE(q.size() == 0);
  q.push(1);
  EXPECT_TRUE(q.size() == 1);
}

TEST(queue_int_list_test, push) {
  sup::queue<int, sup::list<int>> q;
  EXPECT_TRUE(q.size() == 0);
  q.push(1);
  EXPECT_TRUE(q.front() == 1);
}

TEST(queue_int_list_test, pop) {
  sup::queue<int, sup::list<int>> q;
  q.push(1);
  q.pop();
  EXPECT_TRUE(q.empty());
  EXPECT_TRUE(q.size() == 0);
}

}