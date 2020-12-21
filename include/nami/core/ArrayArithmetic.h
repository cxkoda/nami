//
// Created by dave on 17.12.20.
//

#ifndef NAMI_ARRAYARITHMETIC_H
#define NAMI_ARRAYARITHMETIC_H

#include <algorithm>
#include <array>
#include <cassert>

namespace nami::core::array {

  template <typename Iterable>
  inline constexpr Iterable& operator+=(Iterable& lhs, const Iterable& rhs) {
    assert(std::size(lhs) == std::size(rhs));
    std::transform(std::begin(lhs), std::end(lhs), std::begin(rhs), std::begin(lhs),
                   [](const auto& a, const auto& b) { return a + b; });
    return lhs;
  }

  template <typename Iterable>
  inline constexpr Iterable operator+(Iterable lhs, const Iterable& rhs) {
    lhs += rhs;
    return lhs;
  }

  template <typename Iterable>
  inline constexpr Iterable& operator-=(Iterable& lhs, const Iterable& rhs) {
    assert(std::size(lhs) == std::size(rhs));
    std::transform(std::begin(lhs), std::end(lhs), std::begin(rhs), std::begin(lhs),
                   [](const auto& a, const auto& b) { return a - b; });
    return lhs;
  }

  template <typename Iterable>
  inline constexpr Iterable operator-(Iterable lhs, const Iterable& rhs) {
    lhs -= rhs;
    return lhs;
  }

}  // namespace nami::core::array

#endif  // NAMI_ARRAYARITHMETIC_H
