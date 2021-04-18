#include <gtest/gtest.h>
#include <string>

#include "../../src/sstl_unordered_set.hpp"

namespace unordered_set_string_test {

// as hashtable_test contains more detailed tests,
// this test is just for sanity check.

TEST(unordered_set_string_test, construct) {
  sup::unordered_set<std::string> set1;
  EXPECT_TRUE(set1.size() == 0);

  std::string strings[] = {"123", "456", "789", "111"};
  sup::unordered_set<std::string> set2(strings, strings + 4);
  EXPECT_TRUE(set2.size() == 4);
  sup::unordered_set<std::string> set3(strings, strings + 4);
  EXPECT_TRUE(set3.size() == 4);
}

TEST(unordered_set_string_test, insert) {
  std::string strings[] = {"123", "456", "789", "111"};
  
  sup::unordered_set<std::string> set; 
  set.insert(strings, strings+4);
  EXPECT_TRUE(set.size() == 4);
  set.insert(std::to_string(5943));
  EXPECT_TRUE(set.size() == 5);
}

TEST(unordered_set_string_test, find_iterator) {
  std::string strings[] = {"123", "456", "789", "111"};
  
  sup::unordered_set<std::string> set; 
  set.insert(strings, strings+4);
  EXPECT_TRUE(set.size() == 4);
  set.insert(std::to_string(5943));
  EXPECT_TRUE(set.size() == 5);
  sup::unordered_set<std::string>::const_iterator 
    it = set.find(std::to_string(5943));
  EXPECT_TRUE(*it == std::to_string(5943));
}

TEST(unordered_set_string_test, erase) {
  std::string strings[] = {"123", "456", "789", "111"};
  
  sup::unordered_set<std::string> set; 
  set.insert(strings, strings+4);
  for (int i = 0; i < 4; ++i) {
    set.erase(set.find(strings[i]));
    EXPECT_TRUE(set.size() == 3-i);
  }
}

}