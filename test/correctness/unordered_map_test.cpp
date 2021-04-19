#include <gtest/gtest.h>
#include <string>

#include "../../src/sstl_unordered_map.hpp"

namespace unordered_map_int_string_test {

TEST(unordered_map_int_string_test, construct) {
  int n = 4;
  sup::unordered_map<int, std::string> mp1;
  EXPECT_TRUE(mp1.size() == 0);

  std::string strings[] = {"123", "456", "789", "111"};
  std::pair<int, std::string> pairs[4];
  for (int i = 0; i < n; ++i) {
    pairs[i] = std::make_pair(i, strings[i]);
  }
  sup::unordered_map<int, std::string> mp2(pairs, pairs + 4);
  EXPECT_TRUE(mp2.size() == 4);
  sup::unordered_map<int, std::string> mp3(pairs, pairs + 4);
  EXPECT_TRUE(mp3.size() == 4);
}

TEST(unordered_map_int_string_test, insert) {
  int n = 4;
  sup::unordered_map<int, std::string> mp;

  std::string strings[] = {"123", "456", "789", "111"};
  std::pair<int, std::string> pairs[4];
  for (int i = 0; i < n; ++i) {
    pairs[i] = std::make_pair(i, strings[i]);
  }

  mp.insert(pairs, pairs+4);
  EXPECT_TRUE(mp.size() == 4);
  mp.insert(std::make_pair(5943, "23432"));
  EXPECT_TRUE(mp.size() == 5);
}

TEST(unordered_map_int_string_test, find_iterator) {
  int n = 4;
  sup::unordered_map<int, std::string> mp;

  std::string strings[] = {"123", "456", "789", "111"};
  std::pair<int, std::string> pairs[4];
  for (int i = 0; i < n; ++i) {
    pairs[i] = std::make_pair(i, strings[i]);
  }
  
  mp.insert(pairs, pairs+4);
  EXPECT_TRUE(mp.size() == 4);
  mp.insert(std::make_pair(5943, "23432"));
  EXPECT_TRUE(mp.size() == 5);
  sup::unordered_map<int, std::string>::const_iterator 
    it = mp.find(5943);
  EXPECT_TRUE(it->first == 5943);
  EXPECT_TRUE(it->second == std::to_string(23432));
}

TEST(unordered_map_int_string_test, erase) {
  int n = 4;
  sup::unordered_map<int, std::string> mp;

  std::string strings[] = {"123", "456", "789", "111"};
  std::pair<int, std::string> pairs[4];
  for (int i = 0; i < n; ++i) {
    pairs[i] = std::make_pair(i, strings[i]);
  }
  
  mp.insert(pairs, pairs+4);
  for (int i = 0; i < 4; ++i) {
    mp.erase(mp.find(i));
    EXPECT_TRUE(mp.size() == 3-i);
  }
}

}