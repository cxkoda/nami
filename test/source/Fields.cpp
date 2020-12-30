#include <doctest/doctest.h>
#include <nami/Fields.h>

#include <string>
#include <type_traits>

using namespace nami::core;

template <typename... T>
struct TestStack {
  TestStack(T&&... arg) : data{std::forward<T>(arg)...} {}

  Fields<T...> data{};
};

TEST_CASE("Stack Access")
{
  TestStack<int, int> a{2, 3};

  CHECK(get<0>(a) == 2);
  CHECK(get<1>(a) == 3);

  get<1>(a) = 5;
  CHECK(get<0>(a) == 2);
  CHECK(get<1>(a) == 5);

  // rvalue access
  CHECK(get<0>(TestStack<int, int>{1, 2}) == 1);
  CHECK(get<1>(TestStack<int, int>{1, 2}) == 2);

  int b = get<1>(TestStack<int, int>{1, 2});
  CHECK(b == 2);
}

TEST_CASE("Stack Addition")
{
  TestStack<int, int> a{1, 2};
  TestStack<int, int> b{10, 20};
  CHECK(get<0>(a) == 1);
  CHECK(get<1>(a) == 2);

  auto c = a + b;

  CHECK(get<0>(c) == 11);
  CHECK(get<1>(c) == 22);
}

TEST_CASE("Stack Addition")
{
  const TestStack<int, double, std::string> a{2, 0.3, "hello"};
  const TestStack<int, double, std::string> b{3, 0.4, " world"};

  {
    auto c = a + b;
    CHECK(get<0>(c) == 5);
    CHECK(get<1>(c) == 0.7);
    CHECK(get<2>(c) == "hello world");
  }
  {
    auto c = b + a;
    CHECK(get<0>(c) == 5);
    CHECK(get<1>(c) == 0.7);
    CHECK(get<2>(c) == " worldhello");
  }
  {
    auto c{a};
    c += b;
    CHECK(get<0>(c) == 5);
    CHECK(get<1>(c) == 0.7);
    CHECK(get<2>(c) == "hello world");
  }
}

template <typename T>
struct ConcretePair {
  template <typename T1, typename T2>
  ConcretePair(T1&& a1, T2&& a2) : data{std::forward<T1>(a1), std::forward<T2>(a2)}
  {
  }

  ConcretePair() = default;

  Fields<T, T> data{};

  constexpr inline auto&& q1() & { return std::get<0>(data); }
  constexpr inline auto&& q1() const& { return std::get<0>(data); }
  constexpr inline auto&& q1() && { return std::move(std::get<0>(data)); }

  constexpr inline auto&& q2() & { return std::get<1>(data); }
  constexpr inline auto&& q2() const& { return std::get<1>(data); }
  constexpr inline auto&& q2() && { return std::move(std::get<1>(data)); }
};

TEST_CASE("ConcretePair Access")
{
  {
    ConcretePair<int> a{2, 3};

    CHECK(get<0>(a) == 2);
    CHECK(get<1>(a) == 3);

    CHECK(a.q1() == 2);
    CHECK(a.q2() == 3);

    a.q1() = 4;
    CHECK(get<0>(a) == 4);
    CHECK(a.q1() == 4);

    get<1>(a) = 5;
    CHECK(get<1>(a) == 5);
    CHECK(a.q2() == 5);

    // rvalue access
    CHECK(get<0>(ConcretePair<int>{1, 2}) == 1);
    CHECK(ConcretePair<int>{1, 2}.q2() == 2);

    int b = get<1>(ConcretePair<int>{1, 2});
    CHECK(b == 2);
  }
}

#include <memory>

TEST_CASE("ConcretePair Construction")
{
  using Copyable = ConcretePair<int>;
  using NonCopyable = ConcretePair<std::unique_ptr<int>>;
  {
    NonCopyable a{std::make_unique<int>(1), std::make_unique<int>(2)};
    NonCopyable b{};

    b = std::move(a);
    CHECK(*b.q1() == 1);
    CHECK(*b.q2() == 2);
    CHECK(a.q1() == nullptr);
    CHECK(a.q2() == nullptr);
  }
  {
    NonCopyable a{std::make_unique<int>(1), std::make_unique<int>(2)};
    NonCopyable b{std::move(a)};

    CHECK(*b.q1() == 1);
    CHECK(*b.q2() == 2);
    CHECK(a.q1() == nullptr);
    CHECK(a.q2() == nullptr);
  }
  {
    const Copyable a{1, 2};
    Copyable b{a};

    CHECK(a.q1() == 1);
    CHECK(a.q2() == 2);
    CHECK(b.q1() == 1);
    CHECK(b.q2() == 2);
  }
  {
    const Copyable a{1, 2};
    Copyable b{};

    b = a;
    CHECK(a.q1() == 1);
    CHECK(a.q2() == 2);
    CHECK(b.q1() == 1);
    CHECK(b.q2() == 2);
  }
  {
    NonCopyable a{std::make_unique<int>(1), std::make_unique<int>(2)};
    auto b = std::move(a).q1();
    CHECK(*b == 1);
    CHECK(a.q1() == nullptr);
  }
}

template <typename T>
struct ConcreteDerived {
  template <typename T1, typename T2>
  ConcreteDerived(T1&& a1, T2&& a2) : data{std::forward<T1>(a1), std::forward<T2>(a2)}
  {
  }

  Fields<T, ConcretePair<T>> data{};

  constexpr inline auto&& q() & { return std::get<0>(data); }
  constexpr inline auto&& q() const& { return std::get<0>(data); }
  constexpr inline auto&& q() && { return std::move(std::get<0>(data)); }

  constexpr inline auto&& pair() & { return std::get<1>(data); }
  constexpr inline auto&& pair() const& { return std::get<1>(data); }
  constexpr inline auto&& pair() && { return std::move(std::get<1>(data)); }
};

TEST_CASE("ConcreteDerived Access")
{
  ConcreteDerived<int> a{1, ConcretePair<int>{2, 3}};

  CHECK(get<0>(a) == 1);
  CHECK(get<0>(get<1>(a)) == 2);
  CHECK(get<1>(get<1>(a)) == 3);

  CHECK(a.q() == 1);
  CHECK(a.pair().q1() == 2);
  CHECK(a.pair().q2() == 3);

  a.pair().q1() = 4;
  CHECK(get<0>(get<1>(a)) == 4);
  CHECK(a.pair().q1() == 4);

  get<1>(get<1>(a)) = 5;
  CHECK(get<1>(get<1>(a)) == 5);
  CHECK(a.pair().q2() == 5);
}

TEST_CASE("ConcreteDerived Addition")
{
  ConcreteDerived<int> a{1, ConcretePair<int>{2, 3}};
  ConcreteDerived<int> b{10, ConcretePair<int>{20, 30}};

  auto c = a + b;
  CHECK(c.q() == 11);
  CHECK(c.pair().q1() == 22);
  CHECK(c.pair().q2() == 33);
}
