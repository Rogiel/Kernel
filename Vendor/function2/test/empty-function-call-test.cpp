
//  Copyright 2015-2016 Denis Blank <denis.blank at outlook dot com>
//     Distributed under the Boost Software License, Version 1.0
//       (See accompanying file LICENSE_1_0.txt or copy at
//             http://www.boost.org/LICENSE_1_0.txt)

#include "function2-test.hpp"

ALL_LEFT_TYPED_TEST_CASE(AllEmptyFunctionCallTests)

TYPED_TEST(AllEmptyFunctionCallTests, CallSucceedsIfNonEmpty)
{
  typename TestFixture::template left_t<bool()> left = returnTrue;
  EXPECT_TRUE(left());
}

#ifndef TESTS_NO_EXCEPTIONS

TYPED_TEST(AllEmptyFunctionCallTests, CallThrowsIfEmpty)
{
  typename TestFixture::template left_t<bool()> left;
  EXPECT_THROW(left(), fu2::bad_function_call);
}

#endif // TESTS_NO_EXCEPTIONS
