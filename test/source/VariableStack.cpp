#include <doctest/doctest.h>
#include <nami/VariableStack.h>

#include <string>

using namespace nami;

// TEST_CASE("VariableStack Arithmetic") {
//  nami::VariableStack<double> state{0.6};
//  auto state2{state + state};
//  CHECK(get<0>(state2) == 1.2);
//  //  const double value = 10;
//  //  auto state2 = value * state;
//  //
//  //  CHECK(state2.quantity_ == 20);
//}

TEST_CASE("VariableStack Addition") {
  VariableStack<int, double, std::string> a{2, 0.3, "hello"};
  VariableStack<int, double, std::string> b{3, 0.4, " world"};
  auto c = a + b;

  CHECK(std::get<0>(c) == 5);
  CHECK(std::get<1>(c) == 0.7);
  CHECK(std::get<2>(c) == "hello world");
  CHECK(std::get<2>(b + a) == " worldhello");
}

TEST_CASE("VariableStack Addition Assignment") {
  nami::VariableStack<int, double, std::string> a{2, 0.3, "hello"};
  nami::VariableStack<int, double, std::string> b{3, 0.4, " world"};
  a += b;

  CHECK(std::get<0>(a) == 5);
  CHECK(std::get<1>(a) == 0.7);
  CHECK(std::get<2>(a) == "hello world");
}