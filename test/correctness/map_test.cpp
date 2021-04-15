#include <gtest/gtest.h>

#include "../../src/sstl_map.hpp"
#include <utility>
#include <string>

// As most of the methods are wrapper methods for 
// the RB tree, the test is mainly to ensure successfuly 
// compiling and provide basic sanity checks for the 
// implementation

namespace map_int_str_test {

TEST(map_int_str_test, construct) {
  sup::map<int, std::string> m;
}

TEST(map_int_str_test, insert) {
  int a[] = {6, 4, 8, 5, 7, 2, 9, 1, 0, 3};
  int n = 10;
  std::string str = "123";
  sup::map<int, std::string> m;
  for (int i = 0 ; i<n; ++i)
    m.insert(std::make_pair(a[i], str));
  
  // does the same insersion again
  for (int i = 0 ; i<n; ++i)
    m.insert(std::make_pair(a[i], str));
  EXPECT_TRUE(m.size() == 10);
}

TEST(map_int_str_test, equal_range) {
  int a[] = {6, 4, 8, 5, 7, 2, 9, 1, 0, 3};
  int n = 10;
  std::string str = "123";
  sup::map<int, std::string> m;
  for (int i = 0 ; i<n; ++i)
    m.insert(std::make_pair(a[i], str));
  
  // does the same insersion again
  EXPECT_TRUE(m.size() == 10);

  // make sure there is only one (3, "123") pair in the map
  std::pair<sup::map<int, std::string>::iterator, 
    sup::map<int, std::string>::iterator> range = m.equal_range(3);
  int count= 0;
  while (range.first != range.second) {
    ++range.first;
    ++count;
  }
  EXPECT_TRUE(count == 1);
}

}


