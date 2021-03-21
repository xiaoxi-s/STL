#include <gtest/gtest.h>

#include "../src/sstl_deque.hpp"


namespace deque_test {

TEST(deque_int_test, init_empty) {
  sup::deque<int> dq; 
  EXPECT_TRUE(dq.empty());
}

TEST(deque_int_test, push_front_simple) {
  sup::deque<int> dq; 
  for (int i = 0; i < 10; ++i) {
    dq.push_front(i);
  }

  sup::deque<int>::iterator it = dq.begin();

  for (int i = 9; it != dq.end(); ++it, --i) {
    EXPECT_TRUE(*it == i );
  }
}

TEST(deque_int_test, push_back_simple) {
  sup::deque<int> dq; 
  for (int i = 0; i < 10; ++i) {
    dq.push_back(i);
  }
  EXPECT_TRUE(dq.size() == 10);

  sup::deque<int>::iterator it = dq.begin();

  for (int i = 0; it != dq.end(); ++it, ++i) {
    EXPECT_TRUE( *it == i );
  }
}

TEST(deque_int_test, push_front_overload) {
  // initial size: 8 nodes; 8 ints each buffer
  sup::deque<int> dq; 
  for (int i = 0; i < 400; ++i) {
    dq.push_front(i);
  }
  EXPECT_TRUE(dq.size() == 400);

  sup::deque<int>::iterator it = dq.begin();
  for (int i = 399; it != dq.end(); ++it, --i) {
    EXPECT_TRUE( *it == i );
  }
}

TEST(deque_int_test, push_back_overload) {
  sup::deque<int> dq; 
  EXPECT_TRUE(dq.size() == 0);
  for (int i = 0; i < 400; ++i) {
    dq.push_back(i);
  }
  EXPECT_TRUE(dq.size() == 400);

  sup::deque<int>::iterator it = dq.begin();
  for (int i = 0; it != dq.end(); ++it, ++i) {
    EXPECT_TRUE( *it == i );
  }
}

TEST(deque_int_test, pop_back) {
  sup::deque<int> dq(16, 1); 
  EXPECT_TRUE(dq.size() == 16);
  for (int i = 0; i < 8; ++i) {
    dq.pop_back();
  }
  EXPECT_TRUE(dq.size() == 8);
  dq.pop_back();
  EXPECT_TRUE(dq.size() == 7);
  for (int i = 0; i < 7; ++i) {
    dq.pop_back();
  }
  EXPECT_TRUE(dq.size() == 0);
}

TEST(deque_int_test, pop_front) {
  sup::deque<int> dq(16, 1); 
  EXPECT_TRUE(dq.size() == 16);
  for (int i = 0; i < 8; ++i) {
    dq.pop_front();
  }
  EXPECT_TRUE(dq.size() == 8);
  dq.pop_front();
  EXPECT_TRUE(dq.size() == 7);
  for (int i = 0; i < 7; ++i) {
    dq.pop_front();
  }
  EXPECT_TRUE(dq.size() == 0);
}

TEST(deque_int_test, clear) {
  sup::deque<int> dq(16, 1); 
  dq.clear();
  EXPECT_TRUE(dq.size() == 0);
  dq.push_back(1);
  EXPECT_TRUE(dq.size() == 1);
  dq.clear();
  EXPECT_TRUE(dq.size() == 0);
  dq.push_front(1);
  EXPECT_TRUE(dq.size() == 1);
  dq.clear();
  EXPECT_TRUE(dq.size() == 0);
}

TEST(deque_int_test, erase_one_element) {
  sup::deque<int> dq(16, 1); 
  sup::deque<int>::iterator it = dq.begin();
  it += 8;
  it = dq.erase(it);
  EXPECT_TRUE(dq.size() == 15);
  EXPECT_TRUE(it == dq.begin() + 8);
}

TEST(deque_int_test, erase_range) {
  sup::deque<int> dq(16, 1); 
  sup::deque<int>::iterator start = dq.begin() + 1;
  sup::deque<int>::iterator end = dq.end() - 1;
  end = dq.erase(start, end);
  EXPECT_TRUE(dq.size() == 2);
  EXPECT_TRUE(end == dq.begin() + 1);
}

TEST(deque_int_test, insert) {
  sup::deque<int> dq(16, 1); 

  sup::deque<int>::iterator it = dq.begin() + 8;
  it = dq.insert(it, 10);
  EXPECT_TRUE(*it == 10);
  EXPECT_TRUE(dq.size() == 17);
  it = dq.insert(dq.begin(), 555);
  EXPECT_TRUE(*it == 555);
  EXPECT_TRUE(dq.size() == 18);
  it = dq.insert(dq.end(), 8888);
  EXPECT_TRUE(*it == 8888);
  EXPECT_TRUE(dq.size() == 19);
}

}