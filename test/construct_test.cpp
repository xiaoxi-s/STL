#include <gtest/gtest.h>
#include <stdlib.h>

#include "../src/sstl_construct.hpp"
#include "../src/sstl_allocator.hpp"

namespace construct_test {

TEST(construct_test, construct_int_in_place) {
  int a = 0;
  EXPECT_TRUE(a == 0);

  sup::_construct(&a, 10);
  EXPECT_TRUE(a == 10);
}

TEST(construct_test, destory_int) {
  int a = 0;
  // empty destroy function would be called but there is no way to test it
  sup::_destroy(&a); 
}

class NonEmptyClass {
public:
  int a;
  NonEmptyClass(){a = 0;}
  NonEmptyClass(int temp) {a = temp;}
  ~NonEmptyClass() {a = a  - 1;}
};

TEST(construct_test, construct_destory_one_trivial_class_in_place) {

  // for in-place new
  unsigned char array[sizeof(NonEmptyClass)];
  for (int i = 0; i < sizeof(NonEmptyClass); ++i) {
    array[i] = 255;
  }

  NonEmptyClass* p = (NonEmptyClass*) array;
  
  // for now, no constructor is called
  // Every bit in array is 1.

  EXPECT_TRUE(p->a == -1);
  sup::_construct(p, 10);
  EXPECT_TRUE(p->a == 10);
  sup::_destroy(p);
  // according to destuctor
  EXPECT_TRUE(p->a == 9); 
}

TEST(construct_test, destory_multiple_non_trivial_class_based_on_range) {
  NonEmptyClass array[16];
  NonEmptyClass *it = array;
  sup::_destroy(it, it + 10);

  for (int i = 0 ; i < 10 ; ++i) {
    // Note: destructor would be called. 
    EXPECT_TRUE(it->a == -1); 
  }
}

TEST(construct_test, destory_multiple_non_trivial_class_based_on_size) {
  NonEmptyClass array[16];
  NonEmptyClass *it = array;
  sup::_destroy_n(it, 10);

  for (int i = 0 ; i < 10 ; ++i) {
    // Note: destructor would be called. 
    EXPECT_TRUE(it->a == -1); 
  }
}


class TrivialDestructorClass {
public:
  int a;
  TrivialDestructorClass(){a = 0;}
  TrivialDestructorClass(int temp) {a = temp;}
};

TEST(construct_test, destory_multiple_trivial_class_based_on_range) {
  TrivialDestructorClass array[16];
  TrivialDestructorClass *it = array;
  // nothing would happen as this destory is an empty function
  sup::_destroy(it, it + 10);

  for (int i = 0 ; i < 10 ; ++i) {
    // Note: no destructor would be called. 
    EXPECT_TRUE(it->a == 0); 
  }
}

TEST(construct_test, destory_multiple_trivial_class_based_on_size) {
  TrivialDestructorClass array[16]; // default value 0
  TrivialDestructorClass *it = array;
  // nothing would happen as this destory is an empty function
  sup::_destroy_n(it, 10);

  for (int i = 0 ; i < 10 ; ++i) {
    // Note: no destructor would be called. 
    EXPECT_TRUE(it->a == 0); 
  }
}

TEST(construct_test, destory_multiple_trivial_class_with_allocator){
  sup::allocator<NonEmptyClass> a;
  NonEmptyClass *it = a.allocate(10);

  // construct value
  for (int i = 0 ; i < 10; ++i) {
    a.construct(it+i, 10);
    EXPECT_TRUE(it->a == 10);
  }

  sup::_destroy(it, it+10, a);
  for (int i = 0 ; i < 10 ; ++i) {
    // Note: no destructor would be called.     
    EXPECT_TRUE(it->a == 9); 
  }

  a.deallocate(it, 10);
}

}  // namespace construct_test