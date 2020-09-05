#include <gtest/gtest.h>

#include "../src/sstl_iterator_base.hpp"

namespace iterator_base_test {

TEST(iterator_base_test, native_pointer_advance_distance) {
  int a[10];
  int* p = a;

  for (int i = 0; i < 10; ++i) a[i] = i;
  for (int i = 0; i < 10; ++i) {
    // test distance
    EXPECT_TRUE(sup::distance(a, a + i) == i);
  }
  for (int i = 0; i < 10; ++i) {
    EXPECT_TRUE(p == a + i);
    // test advance
    sup::advance(p, 1);
  }
}

// Create a integer generator as a iterator
class MyForwardIteratorForTest : public sup::iterator<sup::forward_iterator_tag, int> {
 private:
  int start;
  int end;
  int current;

  typedef sup::iterator_traits<MyForwardIteratorForTest> traits_type;

  typedef MyForwardIteratorForTest iterator_type;
  typedef typename traits_type::difference_type difference_type;
  typedef typename traits_type::pointer pointer;
  typedef typename traits_type::reference reference;

 public:
  MyForwardIteratorForTest() : start(0), current(0), end(10) {}
  MyForwardIteratorForTest(int s, int e) : start(s), current(s), end(e) {}

  reference operator*() { return current; }

  MyForwardIteratorForTest& operator++() {
    if (current < end) current += 1;
    return *this;
  }

  // only for test
  MyForwardIteratorForTest operator+(difference_type n) {
    int temp = current + n;
    if (temp > end)
      return MyForwardIteratorForTest(end, end);
    else
      return MyForwardIteratorForTest(temp, end);
  }

  // only for test
  MyForwardIteratorForTest operator-(difference_type n) {
    int temp = current - n;
    if (temp > end)
      return MyForwardIteratorForTest(end, end);
    else
      return MyForwardIteratorForTest(temp, end);
  }

  bool operator==(MyForwardIteratorForTest it) {
    if (this->current == it.current) return true;
    return false;
  }

  bool operator!=(MyForwardIteratorForTest it) {
    if (this->current != it.current) return true;
    return false;
  }
};

TEST(iterator_base_test, customized_forward_iterator) {
  MyForwardIteratorForTest myit;
  EXPECT_TRUE(*myit == 0);
  ++myit;
  // test ++
  EXPECT_TRUE(*myit == 1);
}

TEST(iterator_base_test, customized_forward_iterator_arithmatic) {
  MyForwardIteratorForTest myit(-1, 10);
  // test +, -
  EXPECT_TRUE(*myit == -1);
  EXPECT_TRUE(*(myit + 1) == 0);
  EXPECT_TRUE(*(myit - 10) == -11);
}

TEST(iterator_base_test, customized_forward_iterator_advance_distance) {
  MyForwardIteratorForTest myit1(-1, 10);
  MyForwardIteratorForTest myit2(1, 10);
  // test distance function
  EXPECT_TRUE(sup::distance(myit1, myit2) == 2);
}

// Create a random accessor
class MyRandomAccessForTest : public sup::iterator<sup::input_iterator_tag, int> {
 private:
  int start;
  int end;
  int current;

  typedef sup::iterator_traits<MyRandomAccessForTest> traits_type;

  typedef MyRandomAccessForTest iterator_type;
  typedef typename traits_type::difference_type difference_type;
  typedef typename traits_type::pointer pointer;
  typedef typename traits_type::reference reference;

 public:
  MyRandomAccessForTest() : start(0), current(0), end(10) {}
  MyRandomAccessForTest(int s, int e) : start(s), current(s), end(e) {}

  reference operator*() { return current; }

  MyRandomAccessForTest& operator++() {
    if (current < end) current += 1;
    return *this;
  }

  // only for test
  MyRandomAccessForTest operator+(difference_type n) {
    int temp = current + n;
    if (temp > end)
      return MyRandomAccessForTest(end, end);
    else
      return MyRandomAccessForTest(temp, end);
  }

  // only for test
  MyRandomAccessForTest operator-(difference_type n) {
    int temp = current - n;
    if (temp > end)
      return MyRandomAccessForTest(end, end);
    else
      return MyRandomAccessForTest(temp, end);
  }

  bool operator==(MyRandomAccessForTest it) {
    if (this->current == it.current) return true;
    return false;
  }

  bool operator!=(MyRandomAccessForTest it) {
    if (this->current != it.current) return true;
    return false;
  }
};

TEST(iterator_base_test, customized_random_iterator) {
  MyRandomAccessForTest myit;
  EXPECT_TRUE(*myit == 0);
  ++myit;
  // test plus
  EXPECT_TRUE(*myit == 1);
}

TEST(iterator_base_test, customized_random_iterator_arithmatic) {
  MyRandomAccessForTest myit(-1, 10);
  // test other +, -
  EXPECT_TRUE(*myit == -1);
  EXPECT_TRUE(*(myit + 1) == 0);
  EXPECT_TRUE(*(myit - 10) == -11);
}

TEST(iterator_base_test, customized_random_iterator_advance_distance) {
  MyRandomAccessForTest myit1(-1, 10);
  MyRandomAccessForTest myit2(1, 10);
  // test distance function
  EXPECT_TRUE(sup::distance(myit1, myit2) == 2);
}



}  // namespace iterator_base_test