//
// Created by dave on 17.12.20.
//

#include "nami/core/ArrayArithmetic.h"

#include <doctest/doctest.h>

#include <array>
using namespace nami;
using namespace nami::core::array;

template <typename T1, typename T2> inline void doArithmeticTests() {
  T1 a{1, 2};
  T2 b{10, 20};

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

  SUBCASE("LESS") {
    T2 c{1, 2};
    CHECK((a < c) == false);

    T2 d{1, 3};
    CHECK((a < d) == true);

    T2 e{0, 3};
    CHECK((a < e) == false);
  }
}

TEST_CASE("Array Array Arithmetic") {
  using T1 = std::array<double, 2>;
  using T2 = std::array<double, 2>;
  doArithmeticTests<T1, T2>();
}

TEST_CASE("Array Vector Arithmetic") {
  using T1 = std::array<double, 2>;
  using T2 = std::vector<double>;
  doArithmeticTests<T1, T2>();
}

TEST_CASE("Vector Array Arithmetic") {
  using T1 = std::vector<double>;
  using T2 = std::array<double, 2>;
  doArithmeticTests<T1, T2>();
}

TEST_CASE("Vector Vector Arithmetic") {
  using T1 = std::vector<double>;
  using T2 = std::vector<double>;
  doArithmeticTests<T1, T2>();
}