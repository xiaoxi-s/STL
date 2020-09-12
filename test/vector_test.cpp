#include <gtest/gtest.h>

#include "../src/sstl_vector.hpp"

// Tests for string or other types would be
// ensured by construct_test & uninitialize_test

// test for vector <int>
namespace vector_int_test {
TEST(vector_int_test, constructor) {
  sup::vector<int> vec1;
  EXPECT_TRUE(vec1.size() == 0);
  EXPECT_TRUE(vec1.capacity() == 0);

  sup::vector<int> vec2(10);
  EXPECT_TRUE(vec2.size() == 10);
  EXPECT_TRUE(vec2.capacity() == 10);
  for (int i = 0; i < vec2.size(); ++i) {
    EXPECT_TRUE(vec2[i] == 0);  // default constructor called to set 0
  }

  sup::vector<int> vec3(10, 1);
  EXPECT_TRUE(vec3.size() == 10);
  EXPECT_TRUE(vec3.capacity() == 10);
  for (int i = 0; i < vec2.size(); ++i) {
    EXPECT_TRUE(vec3[i] == 1);  // non-default constrcutor
  }
}

TEST(vector_int_test, iterators) {
  sup::vector<int> vec1;
  EXPECT_TRUE(vec1.begin() == vec1.end());

  sup::vector<int> vec2(10);
  sup::vector<int>::reverse_iterator rit_s = vec2.rbegin();
  sup::vector<int>::reverse_iterator rit_e = vec2.rend();
  EXPECT_TRUE(vec2.begin() + 10 == vec2.end());
  EXPECT_TRUE(rit_s + 10 == rit_e);

  sup::vector<int> vec3(10, 1);
  EXPECT_TRUE(vec3.begin() + 10 == vec3.end());
}

TEST(vector_int_test, size) {
  sup::vector<int> vec1;
  EXPECT_TRUE(vec1.empty());

  sup::vector<int> vec2(10);
  EXPECT_TRUE(!vec2.empty());
  vec2.resize(20, -1);
  for (int i = 0; i < 20; ++i) {
    vec2[i] == -1;
  }
  EXPECT_TRUE(vec2.size() == 20);
  EXPECT_TRUE(!vec2.empty());

  sup::vector<int> vec3(10, 1);
  vec3.resize(5);  // 0 is a dummy variable
  EXPECT_TRUE(vec3.size() == 5);
  EXPECT_TRUE(!vec3.empty());
}

TEST(vector_int_test, element_access) {
  sup::vector<int> vec(10);
  for (int i = 0; i < 10; ++i) {
    vec[i] = i;
  }
  for (int i = 0; i < 10; ++i) {
    EXPECT_TRUE(vec[i] == i);
  }
  EXPECT_TRUE(vec.front() == 0);
  EXPECT_TRUE(vec.back() == 9);
}

TEST(vector_int_test, push_and_pop) {
  // push back 7, 8, 9,..., 16
  sup::vector<int> vec(10);
  for (int i = 0; i < 10; ++i) {
    vec.push_back(7 + i);
  }
  EXPECT_TRUE(vec.size() == 20);
  EXPECT_TRUE(vec.capacity() == 20);
  for (int i = 0; i < 10; ++i) EXPECT_TRUE(vec[i + 10] == 7 + i);

  // pop back
  for (int i = 0; i < 10; ++i) {
    EXPECT_TRUE(vec.back() == 7 + 10 - i - 1);
    vec.pop_back();
  }
  EXPECT_TRUE(vec.size() == 10);
  EXPECT_TRUE(vec.capacity() == 20);
  sup::vector<int>::iterator it = vec.begin();

  // erase (first, last)
  for (int i = 0; i < 10; ++i) {
    vec.erase(it, it + 1);
    EXPECT_TRUE(vec.size() == 10 - i - 1);
  }
  EXPECT_TRUE(vec.size() == 0);
  EXPECT_TRUE(vec.capacity() == 20);

  // erase (position)
  for (int i = 0; i < 10; ++i) {
    vec.push_back(i);
  }
  EXPECT_TRUE(vec.size() == 10);
  EXPECT_TRUE(vec.capacity() == 20);
  for (int i = 0; i < 9; ++i) {
    vec.erase(vec.begin());
    EXPECT_TRUE(vec[0] == i + 1);
  }
  EXPECT_TRUE(vec.size() == 1);
  EXPECT_TRUE(vec.capacity() == 20);
}

/******** Clear ********/
TEST(vector_int_test, clear) {
  sup::vector<int> vec(10);
  EXPECT_TRUE(vec.size() == 10);
  vec.clear();
  EXPECT_TRUE(vec.size() == 0);
}


TEST(vector_int_test, assign) {
  sup::vector<int> vec0(5, 0);
  sup::vector<int> vec1(10, 1);
  sup::vector<int> vec2 (15, 2);
  EXPECT_TRUE(vec0.size()==5);
  EXPECT_TRUE(vec1.size() == 10);
  EXPECT_TRUE(vec2.size() == 15);
  
  vec0.assign(vec1.begin(), vec1.end());
  // Expected behaviors like std::vector
  EXPECT_TRUE(vec0.size() == 10);
  EXPECT_TRUE(vec0.capacity() == 10);

  vec1.assign(vec2.begin(), vec2.end());
  EXPECT_TRUE(vec1.size() == 15);
  EXPECT_TRUE(vec1.capacity() == 15);
}

/******** Insert one element *******/
TEST(vector_int_test, insert_one_enough_capacity) {
  sup::vector<int> vec(10);
  // make room for one element
  vec.erase(vec.begin());
  EXPECT_TRUE(vec.size() == 9);

  // insert at the begining
  vec.insert(vec.begin(), -1);
  EXPECT_TRUE(vec.size() == 10);
  EXPECT_TRUE(vec[0] == -1);
  for (int i = 1; i < 10; ++i) {
    EXPECT_TRUE(vec[i] == 0);
  }

  // make room for one element
  vec.erase(vec.begin());  // erase the -1
  EXPECT_TRUE(vec.size() == 9);

  // insert at the end
  vec.insert(vec.end(), -1);
  EXPECT_TRUE(vec.size() == 10);
  EXPECT_TRUE(vec[9] == -1);
  for (int i = 0; i < 9; ++i) {
    EXPECT_TRUE(vec[i] == 0);
  }

  // make room for one element
  vec.erase(vec.end() - 1);  // erase the -1
  EXPECT_TRUE(vec.size() == 9);

  vec.insert(vec.begin() + 3, -1);
  EXPECT_TRUE(vec.size() == 10);
  EXPECT_TRUE(vec[3] == -1);
  // other elements are unchanged
  for (int i = 0; i < 3; ++i) {
    EXPECT_TRUE(vec[i] == 0);
  }
  for (int i = 4; i < 10; ++i) {
    EXPECT_TRUE(vec[i] == 0);
  }
}

/******** Insert multiple elements *******/
TEST(vector_int_test, insert_multiple_enough_capacity_small_n) {
  sup::vector<int> vec(10);
  sup::vector<int>::iterator start = vec.begin();
  // force extending vector to capacity of 20;
  vec.push_back(0);
  // elem after >= n
  vec.insert(vec.begin() + 5, 3, 4);  // insert 3 4s into vec
  EXPECT_TRUE(vec.size() == 11 + 3);
  for (int i = 0; i < 3; ++i) {
    EXPECT_TRUE(vec[i + 5] == 4);
  }
  for (int i = 0; i < 5; ++i) {
    // original value;
    EXPECT_TRUE(vec[i] == 0);
  }
  for (int i = 0; i < 6; ++i) {
    // elements after
    EXPECT_TRUE(vec[i + 8] == 0);
  }
}

TEST(vector_int_test, insert_multiple_enough_capacity_large_n) {
  sup::vector<int> vec(20);
  sup::vector<int>::iterator start = vec.begin();
  // force extending vector to capacity of 20;
  vec.push_back(0);
  // elem after >= n
  vec.insert(vec.begin() + 5, 10, 4);  // insert 10 4s into vec
  EXPECT_TRUE(vec.size() == 21 + 10);
  for (int i = 0; i < 10; ++i) {
    EXPECT_TRUE(vec[i + 5] == 4);
  }
  for (int i = 0; i < 5; ++i) {
    // original value;
    EXPECT_TRUE(vec[i] == 0);
  }
  for (int i = 0; i < 6; ++i) {
    // elements after
    EXPECT_TRUE(vec[i + 15] == 0);
  }
}

TEST(vector_int_test, insert_multiple_no_capacity_insert_at_start) {
  sup::vector<int> vec(10);

  vec.insert(vec.begin(), 3, 4);  // insert 3 4s at the start
  std::cout << vec.size() << std::endl;
  EXPECT_TRUE(vec.size() == 10 + 3);
  for (int i = 0; i < 3; ++i) {
    EXPECT_TRUE(vec[i] == 4);
  }
  for (int i = 3; i < 13; ++i) {
    // original value;
    EXPECT_TRUE(vec[i] == 0);
  }
}

TEST(vector_int_test, insert_multiple_no_capacity_insert_in_mid) {
  sup::vector<int> vec(10);

  vec.insert(vec.begin() + 5, 3, 4);  // insert 3 4s in the middle
  EXPECT_TRUE(vec.size() == 10 + 3);
  for (int i = 0; i < 3; ++i) {
    EXPECT_TRUE(vec[i + 5] == 4);
  }
  for (int i = 0; i < 5; ++i) {
    // original value;
    EXPECT_TRUE(vec[i] == 0);
  }
  for (int i = 0; i < 5; ++i) {
    // elements after
    EXPECT_TRUE(vec[i + 8] == 0);
  }
}

TEST(vector_int_test, insert_multiple_no_capacity_insert_at_end) {
  sup::vector<int> vec(10);

  vec.insert(vec.end(), 3, 4);  // insert 3 4s into vec
  EXPECT_TRUE(vec.size() == 10 + 3);
  for (int i = 0; i < 10; ++i) {
    EXPECT_TRUE(vec[i] == 0);
  }
  for (int i = 10; i < 13; ++i) {
    EXPECT_TRUE(vec[i] == 4);
  }
}

TEST(vector_int_test, insert_range_enough_capacity_small_n) {
  sup::vector<int> vec(10);
  sup::vector<int>::iterator start = vec.begin();

  // force extending vector to capacity of 20;
  vec.push_back(0);
  // elem after >= n
  int array[] = {1, 2, 3};
  vec.insert(vec.begin() + 5, array, array + 3);
  EXPECT_TRUE(vec.size() == 11 + 3);
  for (int i = 0; i < 3; ++i) {
    EXPECT_TRUE(vec[i + 5] == array[i]);
  }
  for (int i = 0; i < 5; ++i) {
    // original value;
    EXPECT_TRUE(vec[i] == 0);
  }
  for (int i = 0; i < 6; ++i) {
    // elements after
    EXPECT_TRUE(vec[i + 8] == 0);
  }
}

TEST(vector_int_test, insert_range_enough_capacity_large_n) {
  sup::vector<int> vec(20);
  int array[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  sup::vector<int>::iterator start = vec.begin();

  // force extending vector to capacity of 20;
  vec.push_back(0);

  // elem after >= n
  vec.insert(vec.begin() + 5, array, array + 10);  // insert array elements
  EXPECT_TRUE(vec.size() == 21 + 10);

  for (int i = 0; i < 10; ++i) {
    EXPECT_TRUE(vec[i + 5] == array[i]);
  }
  for (int i = 0; i < 5; ++i) {
    // original value;
    EXPECT_TRUE(vec[i] == 0);
  }
  for (int i = 0; i < 6; ++i) {
    // elements after
    EXPECT_TRUE(vec[i + 15] == 0);
  }
}

TEST(vector_int_test, insert_range_no_capacity_insert_at_start) {
  sup::vector<int> vec(10);
  int array[] = {1, 2, 3};

  vec.insert(vec.begin(), array, array + 3);  // insert array elements at start
  std::cout << vec.size() << std::endl;
  EXPECT_TRUE(vec.size() == 10 + 3);

  for (int i = 0; i < 3; ++i) {
    EXPECT_TRUE(vec[i] == array[i]);
  }
  for (int i = 3; i < 13; ++i) {
    // original value;
    EXPECT_TRUE(vec[i] == 0);
  }
}

TEST(vector_int_test, insert_range_no_capacity_insert_in_mid) {
  sup::vector<int> vec(10);

  int array[] = {1, 2, 3};
  vec.insert(vec.begin() + 5, array,
             array + 3);  // insert array elements in the middle
  EXPECT_TRUE(vec.size() == 10 + 3);
  for (int i = 0; i < 3; ++i) {
    EXPECT_TRUE(vec[i + 5] == array[i]);
  }
  for (int i = 0; i < 5; ++i) {
    // original value;
    EXPECT_TRUE(vec[i] == 0);
  }
  for (int i = 0; i < 5; ++i) {
    // elements after
    EXPECT_TRUE(vec[i + 8] == 0);
  }
}

TEST(vector_int_test, insert_range_no_capacity_insert_at_end) {
  sup::vector<int> vec(10);
  int array[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

  vec.insert(vec.end(), array, array + 10);  // insert array elements at the end
  for (int i = 0; i < 10; ++i) {
    EXPECT_TRUE(vec[i] == 0);
  }
  for (int i = 10; i < 20; ++i) {
    EXPECT_TRUE(vec[i] == array[i - 10]);
  }
}

TEST(vector_int_test, swap) {
  sup::vector<int> vec1(10);
  sup::vector<int> vec2(5, 8);
  vec1.swap(vec2);

  EXPECT_TRUE(vec1.size() == 5);
  EXPECT_TRUE(vec2.size() == 10);

  for (int i = 0; i < 5; ++i) {
    EXPECT_TRUE(vec1[i] == 8);
  }

  for (int i = 0; i < 10; ++i) {
    EXPECT_TRUE(vec2[i] == 0);
  }
}

TEST (vector_int_test, get_allocator) {
  sup::vector<int> v(10, 1);

  sup::allocator<int> a = v.get_allocator();
  int *p = a.allocate(10);
  EXPECT_TRUE(p != nullptr);
}
}  // namespace vector_int_test
