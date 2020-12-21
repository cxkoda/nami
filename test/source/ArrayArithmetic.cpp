//
// Created by dave on 17.12.20.
//

#include "nami/core/ArrayArithmetic.h"

#include <doctest/doctest.h>

#include <array>
using namespace nami;
using namespace nami::core::array;

TEST_CASE("Array Addition") {
  std::array<double, 2> a{1, 2};
  std::array<double, 2> b{10, 20};

  auto c = a + b;

  CHECK(c[0] == 11);
  CHECK(c[1] == 22);
}

TEST_CASE("Array Subtraction") {
  std::array<double, 2> a{1, 2};
  std::array<double, 2> b{10, 20};

  auto c = a - b;

  CHECK(c[0] == -9);
  CHECK(c[1] == -18);
}