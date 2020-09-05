#include <gtest/gtest.h>

#include "../src/sstl_allocator_base.hpp"

// Need to implement: Tests for range construct & destruct

namespace allocator_base_test {

// test allocate return 0 with 0 size
TEST(allocator_base_test, allocate_return_null) {
  sup::allocator_base<int> a;
  int* ptr = a.allocate(0);
  EXPECT_TRUE(ptr == nullptr);
}

class ClassForTest {
public:
  int a;
  static int object_counter_;  // naming following google c++ style
  static int destroy_times_;
  ClassForTest() { ++object_counter_; }
  ClassForTest(int temp) {a = temp;}
  ~ClassForTest() { a = a - 1; ++destroy_times_; }
};

int ClassForTest::object_counter_ = 0;
int ClassForTest::destroy_times_ = 0;

// test not calling constructor while allocating space
TEST(allocator_base_test, not_call_constructor) {
  sup::allocator_base<ClassForTest> a;
  ClassForTest* ptr = a.allocate(10);
  EXPECT_TRUE(ClassForTest::object_counter_ == 0);

  // 10 is a dummy variable
  a.deallocate(ptr, 10);
  // deallocate without calling destructors
  EXPECT_TRUE(ClassForTest::destroy_times_ == 0);
}

// test on construct defined in allocator base
TEST(allocator_base_test, construct_destroy) {
  sup::allocator_base<ClassForTest> a;
  ClassForTest* ptr = a.allocate(1);
  // no construtor called
  EXPECT_TRUE(ClassForTest::object_counter_ == 0);
  a.construct(ptr, 1);
  EXPECT_TRUE(ptr->a == 1);
  
  a.destroy(ptr);
  // space is still there, but the object is destroyed
  EXPECT_TRUE(ptr->a == 0);
  EXPECT_TRUE(ClassForTest::object_counter_ == 0);
}

}  // namespace allocator_base_test