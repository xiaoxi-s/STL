#include <gtest/gtest.h>
#include <string> 

#include "../../src/sstl_hashtable.hpp"

namespace hashtable_int_string_test {

template<class T>
struct extract_key {
  int operator() (const T& i) const {
    return std::stoi(i);
  }
};

template<class T>
struct identity {
  T operator() (const T& i) const {
    return i;
  }
};

template<class T>
struct equal {
  bool operator() (const T& i, const T& j) {
    return i == j;
  }
};

TEST(hashtable_int_string_test, construct) {
  identity<int> id;
  equal<int> eq;
  sup::hashtable<int, int, identity<int>, identity<int>, equal<int>> 
    ht(10, id, eq);
  EXPECT_TRUE(ht.size() == 0);
  EXPECT_TRUE(ht.bucket_count() == 53);
}

TEST(hashtable_int_string_test, insert_equal) {
  identity<int> id;
  equal<int> eq;
  sup::hashtable<int, std::string, identity<int>, extract_key<std::string>, equal<int>> 
  ht(10, id, eq);
  int a[] = {6, 4, 8, 5, 7, 2, 9, 1, 0, 3};
  int n = 10;
  for (int i = 0 ; i < n; ++i)
    ht.insert_equal(std::to_string(a[i]));
  EXPECT_TRUE(ht.size() == n);

  for (int i = 0 ; i < n; ++i)
    ht.insert_equal(std::to_string(a[i]));
  EXPECT_TRUE(ht.size() == 2*n);
  
  for (int i = 0; i < n; ++i) {
    sup::hashtable<int, std::string, identity<int>, extract_key<std::string>, equal<int>>::iterator 
      it = ht.find(i);
    EXPECT_TRUE(*it == std::to_string(i));
  }
}

TEST(hashtable_int_string_test, insert_unique) {
  identity<int> id;
  equal<int> eq;
  sup::hashtable<int, std::string, identity<int>, extract_key<std::string>, equal<int>> 
  ht(10, id, eq);
  int a[] = {6, 4, 8, 5, 7, 2, 9, 1, 0, 3};
  int n = 10;
  for (int i = 0 ; i < n; ++i)
    ht.insert_unique(std::to_string(a[i]));
  EXPECT_TRUE(ht.size() == n);

  // size should not change
  for (int i = 0 ; i < n; ++i) {
    std::pair<sup::hashtable<int, std::string, identity<int>, extract_key<std::string>, equal<int>>::iterator,bool>
      tmp_pair = ht.insert_unique(std::to_string(a[i]));
    EXPECT_TRUE(*(tmp_pair.first) == std::to_string(a[i]));
    EXPECT_FALSE(tmp_pair.second);
  }
  EXPECT_TRUE(ht.size() == n);
  
  // all the previous elements could be found
  for (int i = 0; i < n; ++i) {
    sup::hashtable<int, std::string, identity<int>, extract_key<std::string>, equal<int>>::iterator 
      it = ht.find(i);
    EXPECT_TRUE(*it == std::to_string(i));
  }
}

TEST(hashtable_int_string_test, resize) {
  identity<int> id;
  equal<int> eq;
  sup::hashtable<int, int, identity<int>, identity<int>, equal<int>> 
    ht(10, id, eq);
  
  int n = 10000;
  for (int i = 0; i < n; ++i) {
    ht.insert_equal(i);
  }

  EXPECT_TRUE(ht.size() == n);
  for (int i = 0; i < n; ++i) {
    sup::hashtable<int, int, identity<int>, identity<int>, equal<int>> ::iterator 
      it = ht.find(i);
    EXPECT_TRUE(*it == i);
  }
}

TEST(hashtable_int_string_test, iterator_test) {
  identity<int> id;
  equal<int> eq;
  sup::hashtable<int, int, identity<int>, identity<int>, equal<int>> 
    ht(10, id, eq);
  
  int n = 10000;
  for (int i = 0; i < n; ++i) {
    ht.insert_equal(i);
  }

  EXPECT_TRUE(ht.size() == n);
  sup::hashtable<int, int, identity<int>, identity<int>, equal<int>> ::iterator 
    it = ht.begin();

  for (int i = 0; it != ht.end(); ++it, ++i) {
    EXPECT_TRUE(*it == i);
  }
}

TEST(hashtable_int_string_test, index_operator_overloading_test) {
  identity<int> id;
  equal<int> eq;
  sup::hashtable<int, std::string, identity<int>, extract_key<std::string>, equal<int>> 
    ht(10, id, eq);

  int n = 1000;
  for (int i = 0; i < n; ++i) {
    // still the same key, but use 
    // [] to insert
    // note this operation is very dangerous as 
    // the key might rely on the value.
    ht[i] = std::to_string(i);
  }

  // test whether 
  EXPECT_TRUE(ht.size() == n);
  for (int i = 0; i < n; ++i) {
    sup::hashtable<int, std::string, identity<int>, extract_key<std::string>, equal<int>>::iterator 
      it = ht.find(i);
    EXPECT_TRUE(*it == std::to_string(i));
  }
}

TEST(hashtable_int_string_test, equal_range) {
  identity<int> id;
  equal<int> eq;
  sup::hashtable<int, std::string, identity<int>, extract_key<std::string>, equal<int>> 
    ht(10, id, eq);

  int n = 20;
  // insert 20 "10"s. 
  for (int i = 0; i < n; ++i) {
    // still the same key, but use 
    // [] to insert
    ht[i] = std::to_string(10);
  }

  // insert some other elements
  for (int i = 20; i < 1000; ++i) {
    ht[i] = std::to_string(i);
  }

  // get the equal range of key 10
  typedef sup::hashtable<int, std::string, identity<int>, extract_key<std::string>, equal<int>>::iterator 
  hashtable_int_string_test_iterator_test; 
  std::pair<hashtable_int_string_test_iterator_test, hashtable_int_string_test_iterator_test>
    tmp_range = ht.equal_range(10);
  int sum_of_elements_with_equal_key = 0;

  // test the values are correct
  for (;tmp_range.first != tmp_range.second;) {
    EXPECT_TRUE(*(tmp_range.first) == std::to_string(10));
    ++tmp_range.first;
    ++sum_of_elements_with_equal_key;
  }

  // test there are only 20 of "10"s
  EXPECT_TRUE(sum_of_elements_with_equal_key == 20);
}

TEST(hashtable_int_string_test, erase_given_key) {
    identity<int> id;
  equal<int> eq;
  sup::hashtable<int, std::string, identity<int>, extract_key<std::string>, equal<int>> 
    ht(10, id, eq);
  int n = 20;
  
  // insert 10 "10"s
  for (int i = 0; i < n; ++i) {
    ht.insert_equal(std::to_string(10));
  }

  // insert other elements
  for (int i = 20; i < 1000; ++i) {
    ht.insert_equal(std::to_string(i));
  }

  // erase all elements with key 10, i.e. strings "10"
  EXPECT_TRUE(ht.erase(10) == 20);

  // erase newly added elements
  for (int i = 20; i < 1000; ++i) {
    EXPECT_TRUE(ht.erase(i) == 1);
  }

  // insert again
  for(int i = 0; i < 1000; ++i) {
    ht.insert_equal(std::to_string(i));
  }
  
  // newly inserted elements should be found
  for(int i = 0; i < 1000; ++i) {
    sup::hashtable<int, std::string, identity<int>, extract_key<std::string>, equal<int>>::iterator 
      it = ht.find(i);
    EXPECT_TRUE(*it == std::to_string(i));
  }
  EXPECT_TRUE(ht.size() == 1000);
}

TEST(hashtable_int_string_test, erase_given_iterator) {
    identity<int> id;
  equal<int> eq;
  sup::hashtable<int, std::string, identity<int>, extract_key<std::string>, equal<int>> 
    ht(10, id, eq);
  int a[] = {6, 4, 8, 5, 7, 2, 9, 1, 0, 3};
  int n = 10;

  // insert some elements
  for (int i = 0; i < n; ++i) {
    ht.insert_unique(std::to_string(a[i]));
  }
  
  EXPECT_TRUE(ht.size() == n);
  for (int i = 0; i < n; ++i) {
    sup::hashtable<int, std::string, identity<int>, extract_key<std::string>, equal<int>>::iterator 
      it = ht.find(i);
    ht.erase(it);
    // find it after erase
    EXPECT_TRUE(ht.find(i) == ht.end());
  }

  // after erasing all elements, the size should be 0
  EXPECT_TRUE(ht.size() == 0);

  // insert again
  for (int i = 0; i < n; ++i) {
    ht.insert_unique(std::to_string(a[i]));
  }

  // the values must be found
  for (int i = 0; i < n; ++i) {
    sup::hashtable<int, std::string, identity<int>, extract_key<std::string>, equal<int>>::iterator 
      it = ht.find(a[i]);
    EXPECT_TRUE(*it == std::to_string(a[i]));
  }
}

TEST(hashtable_int_string_test, erase_given_range) {
    identity<int> id;
  equal<int> eq;
  sup::hashtable<int, std::string, identity<int>, extract_key<std::string>, equal<int>> 
    ht(10, id, eq);
  int a[] = {6, 4, 8, 5, 7, 2, 9, 1, 0, 3};
  int n = 10;

  // insert some elements
  for (int i = 0; i < n; ++i) {
    ht.insert_unique(std::to_string(a[i]));
  }
  
  sup::hashtable<int, std::string, identity<int>, extract_key<std::string>, equal<int>>::iterator
    first = ht.find(3);
  sup::hashtable<int, std::string, identity<int>, extract_key<std::string>, equal<int>>::iterator
    last = ht.find(6);
  
  // remvove elements of key 3, 4, 5
  ht.erase(first, last);
  EXPECT_TRUE(ht.size() == 7);

  for (int i = 0; i < n; ++i) {
    sup::hashtable<int, std::string, identity<int>, extract_key<std::string>, equal<int>>::iterator
      it = ht.find(i);
    if ( i < 3 || i >= 6) {
      EXPECT_TRUE(*it == std::to_string(i));
    } else {
      EXPECT_TRUE(it == ht.end());
    }
  }
}


TEST(hashtable_int_string_test, clear) {
    identity<int> id;
  equal<int> eq;
  sup::hashtable<int, std::string, identity<int>, extract_key<std::string>, equal<int>> 
    ht(10, id, eq);

  int n = 20;
  // insert 10 "10"s. 
  for (int i = 0; i < n; ++i) {
    // still the same key, but use 
    // [] to insert
    ht[i] = std::to_string(10);
  }

  // insert some other elements
  for (int i = 20; i < 1000; ++i) {
    ht[i] = std::to_string(i);
  }
  ht.clear();

  EXPECT_TRUE(ht.size() == 0);
  EXPECT_TRUE(ht.begin() == ht.end());
}

}