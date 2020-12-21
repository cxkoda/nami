//
// Created by dave on 17.12.20.
//

#ifndef NAMI_ARRAYARITHMETIC_H
#define NAMI_ARRAYARITHMETIC_H

#include <algorithm>
#include <array>
#include <cassert>
#include <concepts>

namespace nami::core::array {

  namespace internal {
    template <typename T> concept Iterable = requires(T a) {
      {std::begin(a)};
      {std::end(a)};
      {std::size(a)};
    };
  }  // namespace internal

  inline constexpr auto& operator+=(internal::Iterable auto& lhs, internal::Iterable auto& rhs) {
    assert(std::size(lhs) == std::size(rhs));
    std::transform(std::begin(lhs), std::end(lhs), std::begin(rhs), std::begin(lhs),
                   [](const auto& a, const auto& b) { return a + b; });
    return lhs;
  }

  inline constexpr auto operator+(internal::Iterable auto lhs, const internal::Iterable auto& rhs) {
    lhs += rhs;
    return lhs;
  }

  inline constexpr auto& operator-=(internal::Iterable auto& lhs, internal::Iterable auto& rhs) {
    assert(std::size(lhs) == std::size(rhs));
    std::transform(std::begin(lhs), std::end(lhs), std::begin(rhs), std::begin(lhs),
                   [](const auto& a, const auto& b) { return a - b; });
    return lhs;
  }

  inline constexpr auto operator-(internal::Iterable auto lhs, const internal::Iterable auto& rhs) {
    lhs -= rhs;
    return lhs;
  }

  inline constexpr auto& operator/=(internal::Iterable auto& lhs, internal::Iterable auto& rhs) {
    assert(std::size(lhs) == std::size(rhs));
    std::transform(std::begin(lhs), std::end(lhs), std::begin(rhs), std::begin(lhs),
                   [](const auto& a, const auto& b) { return a / b; });
    return lhs;
  }

  inline constexpr auto operator/(internal::Iterable auto lhs, const internal::Iterable auto& rhs) {
    lhs /= rhs;
    return lhs;
  }

  inline constexpr auto& operator*=(internal::Iterable auto& lhs, internal::Iterable auto& rhs) {
    assert(std::size(lhs) == std::size(rhs));
    std::transform(std::begin(lhs), std::end(lhs), std::begin(rhs), std::begin(lhs),
                   [](const auto& a, const auto& b) { return a * b; });
    return lhs;
  }

  inline constexpr auto operator*(internal::Iterable auto lhs, const internal::Iterable auto& rhs) {
    lhs *= rhs;
    return lhs;
  }

}  // namespace nami::core::array

#endif  // NAMI_ARRAYARITHMETIC_H
