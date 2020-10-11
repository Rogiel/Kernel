
//  Copyright 2015-2016 Denis Blank <denis.blank at outlook dot com>
//     Distributed under the Boost Software License, Version 1.0
//       (See accompanying file LICENSE_1_0.txt or copy at
//             http://www.boost.org/LICENSE_1_0.txt)

#ifndef FU2_INCLUDED_FUNCTION2_TEST_HPP__
#define FU2_INCLUDED_FUNCTION2_TEST_HPP__

#include <cstdint>
#include <functional>
#include <memory>
#include <tuple>
#include <type_traits>
#include "gtest/gtest.h"
#include "function2/function2.hpp"

/// A function which always returns true
inline bool returnTrue() { return true; }
/// A function which always returns false
inline bool returnFalse() { return false; }

/// NonCopyable functions with several SFO capacities
template<typename Fn, bool Throwing = true>
using unique_no_sfo = fu2::function_base<Fn, false, 0, Throwing>;
template<typename Fn, bool Throwing = true>
using unique_256_sfo = fu2::function_base<Fn, false, 256, Throwing>;
template<typename Fn, bool Throwing = true>
using unique_512_sfo = fu2::function_base<Fn, false, 512, Throwing>;
/// Copyable functions with several SFO capacities
template<typename Fn, bool Throwing = true>
using copyable_no_sfo = fu2::function_base<Fn, true, 0, Throwing>;
template<typename Fn, bool Throwing = true>
using copyable_256_sfo = fu2::function_base<Fn, true, 256, Throwing>;
template<typename Fn, bool Throwing = true>
using copyable_512_sfo = fu2::function_base<Fn, true, 512, Throwing>;
/// std::function
template<typename Fn, bool Throwing = true>
using std_function = std::function<Fn>;

/// Adds given types to the type list
template<typename... Rest>
struct MergeTypes
  : std::common_type<std::tuple<Rest...>> { };

template<typename... Tuple, typename... Rest>
struct MergeTypes<std::tuple<Tuple...>, Rest...>
  : MergeTypes<Rest..., Tuple...> { };

template<typename Tuple>
struct TupleToTypes;

template<typename... Args>
struct TupleToTypes<std::tuple<Args...>>
  : std::common_type<testing::Types<Args...>> { };

/// Provides the left type which is used in this test case
template<template<typename, bool> class Left>
struct LeftType
{
  /// Left function type which is provided by this test case.
  /// The left type isn't assignable to the right type!
  template<typename Fn, bool Throwing = true>
  using left_t = Left<Fn, Throwing>;
};

/// Provides the left and right type which is used in this test case
template<template<typename, bool> class Left,
         template<typename, bool> class Right>
struct LeftRightType
  : LeftType<Left>
{
  /// Right function type which is provided by this test case.
  /// The right type is assignable to the left type.
  template<typename Fn, bool Throwing = true>
  using right_t = Right<Fn, Throwing>;
};

/// Base class for typed function tests
template<typename Provider>
struct FunctionTesterBase
  : Provider, testing::Test { };

/// Declares a typed test with the given name and types.
#define DEFINE_FUNCTION_TEST_CASE(TEST_CASE_NAME, EXPANDED_TYPES) \
  template<typename Provider> \
  using TEST_CASE_NAME = FunctionTesterBase<Provider>; \
  TYPED_TEST_CASE(TEST_CASE_NAME, \
    typename TupleToTypes< EXPANDED_TYPES >::type);

/// Testing copyable types which are used in a test case
/// when a left type is required only
/// The functions are only capable of wrapping a copyable functor.
using CopyableLeftExpandedTypes = std::tuple<
  LeftType<copyable_no_sfo>,
  LeftType<copyable_256_sfo>,
  LeftType<copyable_512_sfo>
>;

/// Declares a typed test case where all possibilities of copyable
/// functions are used as left parameter.
/// The functions are only capable of wrapping a copyable functor.
#define COPYABLE_LEFT_TYPED_TEST_CASE(TEST_CASE_NAME) \
  DEFINE_FUNCTION_TEST_CASE(TEST_CASE_NAME, CopyableLeftExpandedTypes)

/// Testing unique types which are used in a test case
/// when a left type is required only
/// The functions are capable of wrapping a unique functor.
using UniqueLeftExpandedTypes = std::tuple<
  LeftType<unique_no_sfo>,
  LeftType<unique_256_sfo>,
  LeftType<unique_512_sfo>
>;

/// Declares a typed test case where all possibilities of copyable sfo
/// functions are used as left parameter.
/// The functions are capable of wrapping a unique functor.
#define UNIQUE_LEFT_TYPED_TEST_CASE( TEST_CASE_NAME ) \
  DEFINE_FUNCTION_TEST_CASE(TEST_CASE_NAME, UniqueLeftExpandedTypes)

/// Testing all types which are used in a test case
/// when a left type is required only.
/// The functions are only capable of wrapping a copyable functor.
using AllLeftExpandedTypes = typename MergeTypes<
  CopyableLeftExpandedTypes,
  UniqueLeftExpandedTypes
>::type;

/// Declares a typed test case where all possibilities of
/// functions are used as left parameter.
/// The functions are only capable of wrapping a copyable functor.
#define ALL_LEFT_TYPED_TEST_CASE( TEST_CASE_NAME ) \
  DEFINE_FUNCTION_TEST_CASE(TEST_CASE_NAME, AllLeftExpandedTypes)

/// Testing copyable types which are used in a test case
/// when a left and a right type is required.
/// The right and left types are always copy assignable to each other.
/// The types are only capable of wrapping a copyable functor.
using CopyableLeftRightExpandedTypes = std::tuple<
  // copyable_no_sfo = ?
  LeftRightType<copyable_no_sfo, copyable_no_sfo>,
  LeftRightType<copyable_no_sfo, copyable_256_sfo>,
  LeftRightType<copyable_no_sfo, copyable_512_sfo>,
  LeftRightType<copyable_no_sfo, std_function>,
  // copyable_256_sfo = ?
  LeftRightType<copyable_256_sfo, copyable_no_sfo>,
  LeftRightType<copyable_256_sfo, copyable_256_sfo>,
  LeftRightType<copyable_256_sfo, copyable_512_sfo>,
  LeftRightType<copyable_256_sfo, std_function>,
  // copyable_256_sfo = ?
  LeftRightType<copyable_512_sfo, copyable_no_sfo>,
  LeftRightType<copyable_512_sfo, copyable_256_sfo>,
  LeftRightType<copyable_512_sfo, copyable_512_sfo>,
  LeftRightType<copyable_512_sfo, std_function>,
  // std::function = ?
  LeftRightType<std_function, copyable_no_sfo>,
  LeftRightType<std_function, copyable_256_sfo>,
  LeftRightType<std_function, copyable_512_sfo>,
  LeftRightType<std_function, std_function>
>;

/// Declares a typed test case where all possibilities of copyable sfo
/// functions are used as left and right parameter,
/// The right and left types are always copy assignable to each other.
/// The types are only capable of wrapping a copyable functor.
#define COPYABLE_LEFT_RIGHT_TYPED_TEST_CASE( TEST_CASE_NAME ) \
  DEFINE_FUNCTION_TEST_CASE(TEST_CASE_NAME, CopyableLeftRightExpandedTypes)

/// Testing unique types which are used in a test case
/// when a left and a right type is required.
/// The right and left types are always move assignable to each other.
/// The types are capable of wrapping an unique functor.
using UniqueLeftRightExpandedTypes = std::tuple<
  // unique_no_sfo = ?
  LeftRightType<unique_no_sfo, unique_no_sfo>,
  LeftRightType<unique_no_sfo, unique_256_sfo>,
  LeftRightType<unique_no_sfo, unique_512_sfo>,
  // unique_256_sfo = ?
  LeftRightType<unique_256_sfo, unique_no_sfo>,
  LeftRightType<unique_256_sfo, unique_256_sfo>,
  LeftRightType<unique_256_sfo, unique_512_sfo>,
  // unique_512_sfo = ?
  LeftRightType<unique_512_sfo, unique_no_sfo>,
  LeftRightType<unique_512_sfo, unique_256_sfo>,
  LeftRightType<unique_512_sfo, unique_512_sfo>
>;

/// Declares a typed test case where all possibilities of unique sfo
/// functions are used as left and right parameter,
/// where the right parameter is always move assignable to the left one.
/// The types are capable of wrapping an unique functor.
#define UNIQUE_LEFT_RIGHT_TYPED_TEST_CASE( TEST_CASE_NAME )  \
  DEFINE_FUNCTION_TEST_CASE(TEST_CASE_NAME, UniqueLeftRightExpandedTypes)

/// Testing types which are used in a test case
/// when a left and a right type is required.
/// The right type is always move assignable to the left one,
/// but not the other way round.
/// The types are only capable of wrapping a copyable functor.
using AllLeftRightExpandedTypes = typename MergeTypes <
  UniqueLeftRightExpandedTypes,
  CopyableLeftRightExpandedTypes,
  // unique_no_sfo = ?
  LeftRightType<unique_no_sfo, copyable_no_sfo>,
  LeftRightType<unique_no_sfo, copyable_256_sfo>,
  LeftRightType<unique_no_sfo, copyable_512_sfo>,
  LeftRightType<unique_no_sfo, std_function>,
  // unique_256_sfo = ?
  LeftRightType<unique_256_sfo, copyable_no_sfo>,
  LeftRightType<unique_256_sfo, copyable_256_sfo>,
  LeftRightType<unique_256_sfo, copyable_512_sfo>,
  LeftRightType<unique_256_sfo, std_function>,
  // unique_512_sfo = ?
  LeftRightType<unique_512_sfo, copyable_no_sfo>,
  LeftRightType<unique_512_sfo, copyable_256_sfo>,
  LeftRightType<unique_512_sfo, copyable_512_sfo>,
  LeftRightType<unique_512_sfo, std_function>
> ::type;

/// Declares a typed test case where all possibilities of
/// functions are used as left and right parameter,
/// where the right parameter is always move assignable to the left one.
/// The types are only capable of wrapping a copyable functor.
#define ALL_LEFT_RIGHT_TYPED_TEST_CASE( TEST_CASE_NAME )  \
  DEFINE_FUNCTION_TEST_CASE(TEST_CASE_NAME, AllLeftRightExpandedTypes)

template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args)
{
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

#endif // FU2_INCLUDED_FUNCTION2_TEST_HPP__
