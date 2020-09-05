#include <gtest/gtest.h>

#include "../src/sstl_iterator.hpp"

namespace iterator_test {

TEST(reverse_iterator_test, native_pointer_plus) {
  int array[10];
  int *it = array;
  for (int i = 0; i < 10; ++i) {
    array[i] = i;
  }
  sup::reverse_iterator<int *> r_it_end(it);
  sup::reverse_iterator<int *> r_it1(it + 10);
  sup::reverse_iterator<int *> r_it2(it + 10);
  sup::reverse_iterator<int *> r_it3(it + 10);
  sup::reverse_iterator<int *> r_it4(it + 10);
  int temp = 9;
  while (r_it1 != r_it_end && r_it2 != r_it_end && r_it3 != r_it_end &&
         r_it4 != r_it_end) {

    // result in the expected position
    EXPECT_TRUE(*r_it1 == temp);
    EXPECT_TRUE(*r_it2 == temp);
    EXPECT_TRUE(*r_it3 == temp);

    // test for plus related operators
    r_it1 += 1;
    r_it2++;
    ++r_it3;
    r_it4 = r_it4 + 1;
    --temp;
  }
  EXPECT_TRUE(temp == -1);
}

TEST(reverse_iterator_test, native_pointer_minus) {
  int array[10];
  int *it = array;
  for (int i = 0; i < 10; ++i) {
    array[i] = i;
  }
  sup::reverse_iterator<int *> r_it_end(it + 11);
  sup::reverse_iterator<int *> r_it1(it + 1);
  sup::reverse_iterator<int *> r_it2(it + 1);
  sup::reverse_iterator<int *> r_it3(it + 1);
  sup::reverse_iterator<int *> r_it4(it + 1);
  int temp = 0;
  while (r_it1 != r_it_end && r_it2 != r_it_end && r_it3 != r_it_end &&
         r_it4 != r_it_end) {

    // result in the expected position
    EXPECT_TRUE(*r_it1 == temp);
    EXPECT_TRUE(*r_it2 == temp);
    EXPECT_TRUE(*r_it3 == temp);

    // test for minus related operators
    r_it1 -= 1;
    r_it2--;
    --r_it3;
    r_it4 = r_it4 - 1;
    ++temp;
  }
  // same as testing operator !=
  EXPECT_TRUE(temp == 10);
}

TEST(reverse_iterator_test, native_pointer_comparator) {
  int array[10];
  int *it = array;
  for (int i = 0; i < 10; ++i) {
    array[i] = i;
  }
  sup::reverse_iterator<int *> r_it_end(it);
  sup::reverse_iterator<int *> r_it1(it + 10);
  sup::reverse_iterator<int *> r_it2(it + 10);
  sup::reverse_iterator<int *> r_it3(it + 10);
  sup::reverse_iterator<int *> r_it4(it + 10);
  int temp = 9;
  while (r_it1 != r_it_end && r_it2 != r_it_end && r_it3 != r_it_end &&
         r_it4 != r_it_end) {
    // comparators
    EXPECT_TRUE(r_it1 == r_it2);
    EXPECT_TRUE(r_it2 == r_it3);
    EXPECT_TRUE(r_it3 == r_it4);

    EXPECT_TRUE(r_it1 < r_it_end);
    EXPECT_TRUE(r_it_end > r_it2);
    EXPECT_TRUE(r_it3 <= r_it_end);
    EXPECT_TRUE(r_it_end >= r_it4);

    r_it1 += 1;
    r_it2++;
    ++r_it3;
    r_it4 = r_it4 + 1;
    --temp;
  }
  EXPECT_TRUE(temp == -1);
}

TEST(reverse_iterator_test, native_pointer_dereference) {
  int array[10];
  int *it = array;
  for (int i = 0; i < 10; ++i) {
    array[i] = i;
  }
  sup::reverse_iterator<int *> r_it_end(it);
  sup::reverse_iterator<int *> r_it(it + 10);

  int temp = 9;
  while (r_it != r_it_end) {
    // test for dereference
    EXPECT_TRUE(*r_it == temp);
    r_it += 1;
    --temp;
  }
  EXPECT_TRUE(temp == -1);
}

TEST(reverse_iterator_test, native_pointer_index) {
  int array[10];
  int *it = array;
  for (int i = 0; i < 10; ++i) {
    array[i] = i;
  }
  sup::reverse_iterator<int *> r_it_end(it);
  sup::reverse_iterator<int *> r_it(it + 10);

  int temp = 9;
  for (int i = 0; i < 10; ++i) {
    // test for index operator
    EXPECT_TRUE(r_it[i] == temp);
    --temp;
  }
  EXPECT_TRUE(temp == -1);
}


}  // namespace iterator_test