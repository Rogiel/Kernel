
//  Copyright 2015-2016 Denis Blank <denis.blank at outlook dot com>
//     Distributed under the Boost Software License, Version 1.0
//       (See accompanying file LICENSE_1_0.txt or copy at
//             http://www.boost.org/LICENSE_1_0.txt)


#include "function2/function2.hpp"

using namespace fu2::detail;

struct MyClass {
  int get(int) {
    return 0;
  }
};

int main(int, char**) {

#ifdef _WIN32
  auto ptr = &MyClass::get;

#endif // _WIN32
  return 0;
}
