#include <gtest/gtest.h>
#include <functional>

#include "../../src/sstl_vector.hpp"
#include "../../src/sstl_priority_queue.hpp"

namespace priority_queue_int_test {

TEST(priority_queue_int_test, constructors) {
  sup::priority_queue<int> pq1; 
  EXPECT_TRUE(pq1.empty());

  // based on min heap
  sup::priority_queue<int, sup::vector<int>, std::greater<int>> pq2; 
  EXPECT_TRUE(pq2.empty());

  int n = 10;
  int a[10];
  for (int i = 0 ; i < n; ++i) {
    a[i] = 10 - i;
  }
  
  // based on max heap
  sup::priority_queue<int> pq3 (a, a + 10); 
  EXPECT_TRUE(pq3.top() == 10);
  EXPECT_TRUE(pq3.size() == 10);

  sup::priority_queue<int, sup::vector<int>, std::greater<int>> pq4 (a, a + 10); 
  EXPECT_TRUE(pq4.top() == 1);
  EXPECT_TRUE(pq4.size() == 10);
}

TEST(priority_queue_int_test, empty) {
  sup::priority_queue<int> pq1; 
  EXPECT_TRUE(pq1.empty());

  // based on min heap
  sup::priority_queue<int, sup::vector<int>, std::greater<int>> pq2; 
  EXPECT_TRUE(pq2.empty());

  int n = 10;
  int a[10];
  for (int i = 0 ; i < n; ++i) {
    a[i] = 10 - i;
  }
  
  // based on max heap
  sup::priority_queue<int> pq3 (a, a + 10); 
  for (int i = 0; i < 10; ++i) {
    pq3.pop();
  }
  EXPECT_TRUE(pq3.empty());
}

TEST(priority_queue_int_test, size) {
  int n = 10;
  int a[10];
  for (int i = 0 ; i < n; ++i) {
    a[i] = 10 - i;
  }
  
  // based on max heap
  sup::priority_queue<int> pq3 (a, a + 10); 
  EXPECT_TRUE(pq3.size() == 10);

  // based on min heap
  sup::priority_queue<int, sup::vector<int>, std::greater<int>> pq4 (a, a + 10); 
  EXPECT_TRUE(pq4.size() == 10);

  for (int i = 0; i < 10; ++i) {
    pq3.pop();
    pq4.pop();

    EXPECT_TRUE(pq3.size() == n - i - 1);
    EXPECT_TRUE(pq4.size() == n - i - 1);
  }

  EXPECT_TRUE(pq3.empty());
  EXPECT_TRUE(pq4.empty());
}

TEST(priority_queue_int_test, top_simple_and_push) {
  int n = 10;
  int a[10];
  for (int i = 0 ; i < n; ++i) {
    a[i] = 10 - i;
  }
  
  // based on max heap
  sup::priority_queue<int> pq (a, a + 10); 
  for (int i = 0; i < 10; ++i) {
    EXPECT_TRUE(pq.top() == 10 - i);
    pq.pop();
  }
}

TEST(priority_queue_int_test, top_comparator_and_push) {
  int n = 10;
  int a[10];
  for (int i = 0 ; i < n; ++i) {
    a[i] = 10 - i;
  }

  // based on min heap
  sup::priority_queue<int, sup::vector<int>, std::greater<int>> pq (a, a + 10); 
  for (int i = 0; i < 10; ++i) {
    EXPECT_TRUE(pq.top() == i + 1);
    pq.pop();
  }
}

TEST(priority_queue_int_test, push_simple) {
  int n = 10;

  // based on max heap
  sup::priority_queue<int> pq; 
  for (int i = 0; i < n; ++i) {
    pq.push(i);
    EXPECT_TRUE(pq.top() == i);
  }
}

TEST(priority_queue_int_test, push_comparator) {
  int n = 10;

  // based on max heap
  sup::priority_queue<int, sup::vector<int>, std::greater<int>> pq; 
  for (int i = n - 1; i >= 0; --i) {
    pq.push(i);
    EXPECT_TRUE(pq.top() == i);
  }
}

}
