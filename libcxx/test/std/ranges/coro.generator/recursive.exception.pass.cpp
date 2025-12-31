//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// <generator>

// template<class Ref, class V = void, class Allocator = void>
//   class generator;

#include <algorithm>
#include <cassert>
#include <generator>
#include <ranges>
#include <vector>

class boom_exception : public std::exception {
public:
  const char* what() const noexcept override { return "boom"; };
};

std::generator<int> seven_boom(std::size_t begin, const std::size_t end) {
  for (; begin < end; ++begin) {
    if ((begin % 7) == 0) {
      throw boom_exception{};
    }
    co_yield begin;
  }
}

std::generator<int> recursive_seven_boom() {
  co_yield std::ranges::elements_of(seven_boom(1, 5));
  co_yield std::ranges::elements_of(seven_boom(5, 9));
}

bool test() {
  std::vector<std::size_t> yielded;
  try {
    std::ranges::copy(recursive_seven_boom(), std::back_inserter(yielded));
  } catch (const boom_exception& ex) {
    assert(std::ranges::equal(yielded, std::views::iota(1uz, 7uz)));
    return true;
  }
  return false;
}

int main() {
  assert(test());
  return 0;
}
