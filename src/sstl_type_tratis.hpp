#ifndef _SSTL_TYPE_TRAITS_H
#define _SSTL_TYPE_TRAITS_H

/**
 * @author Xiaoxi Sun
 **/

/**
 * Concepts:
 *  Can use this header directly
 **/

/**
 * Questions:
 * 
 **/

namespace sup {

class __true_type {};
class __false_type{};

template <class type>
class __type_traits {
 public:
  typedef __true_type this_dummy_member_must_be_first;

  // define the most conservative type - __false_type
  typedef __false_type has_trivial_default_constructor;
  typedef __false_type has_trivial_copy_constructor;
  typedef __false_type has_trivial_assignment_operator;
  typedef __false_type has_trivial_destructor;
  typedef __false_type is_POD_type;
};

// template specialization for buildin types (scalars)
template <>
class __type_traits<char> {
 public:
  typedef __true_type this_dummy_member_must_be_first;

  // define the most conservative type - __false_type
  typedef __true_type has_trivial_default_constructor;
  typedef __true_type has_trivial_copy_constructor;
  typedef __true_type has_trivial_assignment_operator;
  typedef __true_type has_trivial_destructor;
  typedef __true_type is_POD_type;
};

template <>
class __type_traits<signed char> {
 public:
  typedef __true_type this_dummy_member_must_be_first;

  // define the most conservative type - __false_type
  typedef __true_type has_trivial_default_constructor;
  typedef __true_type has_trivial_copy_constructor;
  typedef __true_type has_trivial_assignment_operator;
  typedef __true_type has_trivial_destructor;
  typedef __true_type is_POD_type;
};

template <>
class __type_traits<unsigned char> {
 public:
  typedef __true_type this_dummy_member_must_be_first;

  // define the most conservative type - __false_type
  typedef __true_type has_trivial_default_constructor;
  typedef __true_type has_trivial_copy_constructor;
  typedef __true_type has_trivial_assignment_operator;
  typedef __true_type has_trivial_destructor;
  typedef __true_type is_POD_type;
};

template <>
class __type_traits<short> {
 public:
  typedef __true_type this_dummy_member_must_be_first;

  typedef __true_type has_trivial_default_constructor;
  typedef __true_type has_trivial_copy_constructor;
  typedef __true_type has_trivial_assignment_operator;
  typedef __true_type has_trivial_destructor;
  typedef __true_type is_POD_type;
};

template <>
class __type_traits<unsigned short> {
 public:
  typedef __true_type this_dummy_member_must_be_first;

  typedef __true_type has_trivial_default_constructor;
  typedef __true_type has_trivial_copy_constructor;
  typedef __true_type has_trivial_assignment_operator;
  typedef __true_type has_trivial_destructor;
  typedef __true_type is_POD_type;
};

template <>
class __type_traits<int> {
 public:
  typedef __true_type this_dummy_member_must_be_first;

  typedef __true_type has_trivial_default_constructor;
  typedef __true_type has_trivial_copy_constructor;
  typedef __true_type has_trivial_assignment_operator;
  typedef __true_type has_trivial_destructor;
  typedef __true_type is_POD_type;
};

template <>
class __type_traits<unsigned int> {
 public:
  typedef __true_type this_dummy_member_must_be_first;

  typedef __true_type has_trivial_default_constructor;
  typedef __true_type has_trivial_copy_constructor;
  typedef __true_type has_trivial_assignment_operator;
  typedef __true_type has_trivial_destructor;
  typedef __true_type is_POD_type;
};

template <>
class __type_traits<long> {
 public:
  typedef __true_type this_dummy_member_must_be_first;

  // define the most conservative type - __false_type
  typedef __true_type has_trivial_default_constructor;
  typedef __true_type has_trivial_copy_constructor;
  typedef __true_type has_trivial_assignment_operator;
  typedef __true_type has_trivial_destructor;
  typedef __true_type is_POD_type;
};

template <>
class __type_traits<unsigned long> {
 public:
  typedef __true_type this_dummy_member_must_be_first;

  typedef __true_type has_trivial_default_constructor;
  typedef __true_type has_trivial_copy_constructor;
  typedef __true_type has_trivial_assignment_operator;
  typedef __true_type has_trivial_destructor;
  typedef __true_type is_POD_type;
};

template <>
class __type_traits<float> {
 public:
  typedef __true_type this_dummy_member_must_be_first;

  typedef __true_type has_trivial_default_constructor;
  typedef __true_type has_trivial_copy_constructor;
  typedef __true_type has_trivial_assignment_operator;
  typedef __true_type has_trivial_destructor;
  typedef __true_type is_POD_type;
};


template <>
class __type_traits<double> {
 public:
  typedef __true_type this_dummy_member_must_be_first;

  typedef __true_type has_trivial_default_constructor;
  typedef __true_type has_trivial_copy_constructor;
  typedef __true_type has_trivial_assignment_operator;
  typedef __true_type has_trivial_destructor;
  typedef __true_type is_POD_type;
};

template <>
class __type_traits<long double> {
 public:
  typedef __true_type this_dummy_member_must_be_first;

  typedef __true_type has_trivial_default_constructor;
  typedef __true_type has_trivial_copy_constructor;
  typedef __true_type has_trivial_assignment_operator;
  typedef __true_type has_trivial_destructor;
  typedef __true_type is_POD_type;
};

// native pointers 
template <class T>
class __type_traits<T*> {
  typedef __true_type this_dummy_member_must_be_first;

  typedef __true_type has_trivial_default_constructor;
  typedef __true_type has_trivial_copy_constructor;
  typedef __true_type has_trivial_assignment_operator;
  typedef __true_type has_trivial_destructor;
  typedef __true_type is_POD_type;
};

}  // namespace sup

#endif