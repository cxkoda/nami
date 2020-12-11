//
// Created by dave on 03.12.20.
//

#ifndef NAMI_BURGER_H
#define NAMI_BURGER_H

#include <cmath>

#include "Fields.h"

namespace nami {
  namespace Burger {
    using namespace nami::core;

    template <typename T> struct Stack {
      [[nodiscard]] constexpr inline auto&& u() & noexcept { return std::get<0>(data); }
      [[nodiscard]] constexpr inline auto&& u() const& noexcept { return std::get<0>(data); }
      [[nodiscard]] constexpr inline auto&& u() && noexcept { return std::move(std::get<0>(data)); }

      Fields<T> data;
    };

    template <typename Stack>
    inline constexpr auto computeSignalSpeed(Stack&& stack) noexcept {
      return stack.u();
    }

    template <typename Stack> inline constexpr auto computeFlux(Stack&& stack) {
      return 0.5 * stack.u() * stack.u();
    }
  }  // namespace Burger
}  // namespace nami

#endif  // NAMI_BURGER_H
