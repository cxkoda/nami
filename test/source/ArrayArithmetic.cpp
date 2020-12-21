//
// Created by dave on 17.12.20.
//

#include "nami/core/ArrayArithmetic.h"

#include <doctest/doctest.h>

#include <array>
using namespace nami;
using namespace nami::core::array;

TEST_CASE("Array Array Arithmetic") {
  std::array<double, 2> a{1, 2};
  std::array<double, 2> b{10, 20};

  SUBCASE("Addition") {
    auto c = a + b;
    CHECK(c[0] == 11);
    CHECK(c[1] == 22);
  }
  SUBCASE("Subtraction") {
    auto c = a - b;
    CHECK(c[0] == -9);
    CHECK(c[1] == -18);
  }
  SUBCASE("Multiplication") {
    auto c = a * b;
    CHECK(c[0] == 10);
    CHECK(c[1] == 40);
  }
  SUBCASE("Division") {
    auto c = b / a;
    CHECK(c[0] == 10);
    CHECK(c[1] == 10);
  }
  SUBCASE("Implicit Typecast") {
    std::array<std::size_t, 2> d{2, 8};
    auto c = a / d;
    CHECK(c[0] == 0.5);
    CHECK(c[1] == 0.25);
  }
}

TEST_CASE("Array Vector Arithmetic") {
  std::array<double, 2> a{1, 2};
  std::vector<double> b{10, 20};

  SUBCASE("Addition") {
    auto c = a + b;
    CHECK(c[0] == 11);
    CHECK(c[1] == 22);
  }
  SUBCASE("Subtraction") {
    auto c = a - b;
    CHECK(c[0] == -9);
    CHECK(c[1] == -18);
  }
  SUBCASE("Multiplication") {
    auto c = a * b;
    CHECK(c[0] == 10);
    CHECK(c[1] == 40);
  }
  SUBCASE("Division") {
    auto c = b / a;
    CHECK(c[0] == 10);
    CHECK(c[1] == 10);
  }
}

TEST_CASE("Vector Array Arithmetic") {
  std::vector<double> a{1, 2};
  std::array<double, 2> b{10, 20};

  SUBCASE("Addition") {
    auto c = a + b;
    CHECK(c[0] == 11);
    CHECK(c[1] == 22);
  }
  SUBCASE("Subtraction") {
    auto c = a - b;
    CHECK(c[0] == -9);
    CHECK(c[1] == -18);
  }
  SUBCASE("Multiplication") {
    auto c = a * b;
    CHECK(c[0] == 10);
    CHECK(c[1] == 40);
  }
  SUBCASE("Division") {
    auto c = b / a;
    CHECK(c[0] == 10);
    CHECK(c[1] == 10);
  }
}