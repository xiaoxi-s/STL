#include <gtest/gtest.h>

#include "../src/sstl_allocator_base.hpp"

// Need to implement: Tests for range construct & destruct

namespace allocator_base_test {

TEST(Allocator_base_test, allocate_return_null) {
  sup::allocator_base<int> a;
  int* ptr = a.allocate(0);
  EXPECT_TRUE(ptr == nullptr);
}

class ClassForTest {
 public:
  static int object_counter_;  // naming following google c++ style
  static int destroy_times_;
  ClassForTest() { ++object_counter_; }
  ~ClassForTest() { ++destroy_times_; }
};

int ClassForTest::object_counter_ = 0;
int ClassForTest::destroy_times_ = 0;

TEST(Allocator_base_test, allocate_not_call_constructor) {
  sup::allocator_base<ClassForTest> a;
  ClassForTest* ptr = a.allocate(10);
  EXPECT_TRUE(ClassForTest::object_counter_ == 0);

  // 10 is a dummy variable
  a.deallocate(ptr, 10);
  // deallocate without calling destructors
  EXPECT_TRUE(ClassForTest::destroy_times_ == 0);
}

}  // namespace allocator_base_test