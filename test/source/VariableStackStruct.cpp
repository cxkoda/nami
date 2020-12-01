#include <doctest/doctest.h>
#include <nami/VariableStackStruct.h>

#include <string>

using namespace nami::attempt2;

template <typename T> struct TestStack {
  TestStack(T&& a1, T&& a2) : q1{std::forward<T>(a1)}, q2{std::forward<T>(a2)} {}

  T q1{};
  T q2{};

  //  inline constexpr auto getAll() noexcept { return all; }
  //  inline constexpr auto getAll() const noexcept { return all; }
  //
  // private:
  //  std::tuple<T&, T&> all{std::tie(q1, q2)};
  // Das hat funktioniert jedoch müssen copy ctors umgeschrieben werden.

  // Direktes iterieren über die member nicht möglich da cpp noch kein reflection hat
  // vielleicht wäre https://github.com/garbageslam/visit_struct eine Lösung?

  template <std::size_t idx> inline constexpr auto&& get() & noexcept {
    if constexpr (idx == 0) {
      return q1;
    } else if constexpr (idx == 1) {
      return q2;
    }
  }

  template <std::size_t idx> inline constexpr auto&& get() const & noexcept {
    if constexpr (idx == 0) {
      return q1;
    } else if constexpr (idx == 1) {
      return q2;
    }
  }

  constexpr static std::size_t size = 2;

  // diese variante ist leider ziemlich noisy
  // irgendwie als free function verpacken und qualifier weitergeben?
};

TEST_CASE("Stack Access") {
  TestStack<int> a{2, 3};

  CHECK(get<0>(a) == 2);
  CHECK(get<1>(a) == 3);

  CHECK(a.q1 == 2);
  CHECK(a.q2 == 3);

  a.q1 = 4;
  CHECK(get<0>(a) == 4);
  CHECK(a.q1 == 4);

  get<1>(a) = 5;
  CHECK(get<1>(a) == 5);
  CHECK(a.q2 == 5);

  // rvalue access
  CHECK(get<0>(TestStack<int>{1, 2}) == 1);
  CHECK(TestStack<int>{1, 2}.get<1>() == 2);

  int b = get<1>(TestStack<int>{1, 2});
  CHECK(b == 2);
}

TEST_CASE("Stack Arithmetic") {
  TestStack<int> a{2, 3};
  TestStack<int> b{1, 1};

  a += b;
  CHECK(a.q1 == 3);
  CHECK(a.q2 == 4);

  auto c = a + b;

  CHECK(a.q1 == 3);
  CHECK(a.q2 == 4);
  CHECK(c.q1 == 4);
  CHECK(c.q2 == 5);
}

// template <typename T> struct TestStack2 : TestStack<T> {
//  TestStack2(T&& a1, T&& a2, T&& a3)
//      : TestStack<T>{std::forward<T>(a1), std::forward<T>(a2)}, q3{std::forward<T>(a3)} {}
//
//  T q3{};
//
//  inline constexpr auto getAll() noexcept {
//    return std::tuple_cat(TestStack<T>::getAll(), std::make_tuple(q3));
//  }
//};
//
// TEST_CASE("VariableStack Addition") {
//  TestStack2<int> a{2, 3, 4};
//
//  CHECK(get<0>(a) == 2);
//  CHECK(get<1>(a) == 3);
//  CHECK(get<2>(a) == 4);
//
//  //  auto c = a + b;
//  //
//  //  CHECK(c.q1 == 5);
//  //  CHECK(c.q2 == 7);
//
//  //  CHECK(std::get<0>(c) == 5);
//  //  CHECK(std::get<1>(c) == 0.7);
//  //  CHECK(std::get<2>(c) == "hello world");
//  //  CHECK(std::get<2>(b + a) == " worldhello");
//}

//
// TEST_CASE("VariableStack Addition Assignment") {
//  TestStack<int, double, std::string> a{2, 0.3, "hello"};
//  TestStack<int, double, std::string> b{3, 0.4, " world"};
//  a += b;
//
//  CHECK(std::get<0>(a) == 5);
//  CHECK(std::get<1>(a) == 0.7);
//  CHECK(std::get<2>(a) == "hello world");
//}
//
// template <typename T> struct TestStack2 : VariableStack<TestStack2<T>, T, T> {
//  TestStack2(T&& a1, T&& a2)
//      : VariableStack<TestStack2<T>, T, T>(std::forward<T>(a1), std::forward<T>(a2)) {}
//
//  T& q1{std::get<0>(*this)};
//  T& q2{std::get<1>(*this)};
//};
//
// TEST_CASE("") {
//  TestStack2<int> a{1, 2};
//  CHECK(std::get<0>(a) == 1);
//  CHECK(std::get<1>(a) == 2);
//
//  a.q1 = 10;
//  CHECK(std::get<0>(a) == 10);
//}