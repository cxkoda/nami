#include <doctest/doctest.h>
#include <nami/VariableStack.h>

#include <string>

using namespace nami::attempt1;

// TEST_CASE("VariableStack Arithmetic") {
//  nami::VariableStack<double> state{0.6};
//  auto state2{state + state};
//  CHECK(get<0>(state2) == 1.2);
//  //  const double value = 10;
//  //  auto state2 = value * state;
//  //
//  //  CHECK(state2.quantity_ == 20);
//}

template <typename... T> struct TestStack : VariableStack<TestStack<T...>, T...> {
  TestStack(T&&... args) : VariableStack<TestStack<T...>, T...>(std::forward<T>(args)...) {}
};

TEST_CASE("VariableStack Addition") {
  TestStack<int, double, std::string> a{2, 0.3, "hello"};
  TestStack<int, double, std::string> b{3, 0.4, " world"};
  auto c = a + b;

  CHECK(std::get<0>(c) == 5);
  CHECK(std::get<1>(c) == 0.7);
  CHECK(std::get<2>(c) == "hello world");
  CHECK(std::get<2>(b + a) == " worldhello");
}

TEST_CASE("VariableStack Addition Assignment") {
  TestStack<int, double, std::string> a{2, 0.3, "hello"};
  TestStack<int, double, std::string> b{3, 0.4, " world"};
  a += b;

  CHECK(std::get<0>(a) == 5);
  CHECK(std::get<1>(a) == 0.7);
  CHECK(std::get<2>(a) == "hello world");
}

template <typename T> struct TestStack2 : VariableStack<TestStack2<T>, T, T> {
  TestStack2(T&& a1, T&& a2)
      : VariableStack<TestStack2<T>, T, T>(std::forward<T>(a1), std::forward<T>(a2)) {}

  T& q1{std::get<0>(*this)};
  T& q2{std::get<1>(*this)};
};

TEST_CASE("") {
  TestStack2<int> a{1, 2};
  CHECK(std::get<0>(a) == 1);
  CHECK(std::get<1>(a) == 2);

  a.q1 = 10;
  CHECK(std::get<0>(a) == 10);
}