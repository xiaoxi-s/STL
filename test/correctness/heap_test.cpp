#include <gtest/gtest.h>

#include "../../src/sstl_algorithm.hpp"
#include "../../src/sstl_iterator.hpp"

namespace heap_int_test {

TEST(heap_int_test, make_heap_simple) {
  // test for making a max heap
  int n = 10;
  int a[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  sup::make_heap(a, a + 10);
  int * last = a + 10;
  for (int i = 0; i < n/2; ++i) {
    int* cur = a + i;
    int* left_child = sup::__get_left_child(i) + a;

    EXPECT_FALSE(left_child < last && *cur < *left_child );

    EXPECT_FALSE(left_child + 1 < last && *cur < *(left_child + 1));
  }
}

TEST(heap_int_test, make_heap_comparator) {
  // test for making a min heap
  int n = 10;
  int a[10];
  for (int i =0 ; i < n; ++i) {
    a[i] = 10 - i;
  }

  sup::make_heap(a, a + 10,[] (int x, int y) { return x > y; });

  int * last = a + 10;
  for (int i = 0; i < n/2; ++i) {
    int* cur = a + i;
    int* left_child = sup::__get_left_child(i) + a;

    EXPECT_FALSE(left_child < last && *cur > *left_child );

    EXPECT_FALSE(left_child + 1 < last && *cur > *(left_child + 1));
  }
}

TEST(heap_int_test, is_heap_simple_and_comparator) {
  // test for a min heap
  int n = 10;
  int a[10];
  for (int i =0 ; i < n; ++i) {
    a[i] = i;
  }
  int* first = a;
  int* last = a + 10;
  // not a max heap
  EXPECT_FALSE(sup::is_heap(first, last)); 
  // is a min heap
  EXPECT_TRUE(sup::is_heap(first, last, [](int x, int y) { return x > y; }));

  // test for a max heap
  for (int i =0 ; i < n; ++i) {
    a[i] = 10 - i;
  }
  // is a max heap
  EXPECT_TRUE(sup::is_heap(first, last)); 
  // not a min heap
  EXPECT_FALSE(sup::is_heap(first, last, [](int x, int y) { return x > y; }));
}

TEST(heap_int_test, push_heap_simple_and_comparator) {
  // test for a min heap
  int n = 11;
  int a[11];
  int* first = a;
  int* last = a + n;

  for (int i =0 ; i < n; ++i) {
    a[i] = i;
  }

  // not a heap now! make it a min heap
  a[10] = -1;
  sup::push_heap(a, a + n, [](int x, int y) { return x > y; });

  // not a max heap
  EXPECT_FALSE(sup::is_heap(first, last)); 
  // is a min heap
  EXPECT_TRUE(sup::is_heap(first, last, [](int x, int y) { return x > y; }));

  // test for a max heap
  for (int i =0 ; i < n; ++i) {
    a[i] = 10 - i;
  }

  // not a heap now! make it a max heap
  a[10] = 20;
  sup::push_heap(a, a + n);

  // is a max heap
  EXPECT_TRUE(sup::is_heap(first, last)); 
  // not a min heap
  EXPECT_FALSE(sup::is_heap(first, last, [](int x, int y) { return x > y; }));
}

TEST(heap_int_test, pop_heap_simple_and_comparator) {
  // test max heap
  int n = 10;
  int a[10];
  for (int i =0 ; i < n; ++i) {
    a[i] = 10 - i;
  }

  int* first = a;
  int* last = a + n;
  sup::pop_heap(first, last); // 10 is popped
  EXPECT_TRUE( a[0] = 9); // remaining maxi element: 9
  EXPECT_TRUE(sup::is_heap(first, last - 1));

  // test min heap
  for (int i = 0 ; i < n; ++i) {
    a[i] = i;
  }

  sup::pop_heap(first, last, [](int x, int y) { return x > y; }); // 0 is popped
  EXPECT_TRUE( a[0] = 1); // remaining smallest element: 1
  EXPECT_TRUE(sup::is_heap(first, last - 1, [](int x, int y) { return x > y; }));
}

TEST(heap_int_test, sort_heap_simple_and_comparator) {
  // test max heap
  int n = 10;
  int a[10];
  for (int i =0 ; i < n; ++i) {
    a[i] = 10 - i;
  }
  
  int* first = a;
  int* last = a + n;
  sup::sort_heap(first, last); // increasing order
  for (int i = 0; i < n-1; ++i) { 
    EXPECT_TRUE(a[i] <= a[i+1]);
    EXPECT_TRUE(a[i] == i+1);
  }
  EXPECT_TRUE(a[9] == 10);

  // test min heap
  for (int i = 0 ; i < n; ++i) {
    a[i] = i;
  }

  sup::sort_heap(first, last, [](int x, int y) { return x > y; }); // descreasing order
  for (int i = 0; i < n-1; ++i) { 
    EXPECT_TRUE(a[i] >= a[i+1]);
    EXPECT_TRUE(a[i] == 9 - i);
  }
  EXPECT_TRUE(a[9] == 0);
}

}