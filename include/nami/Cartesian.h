//
// Created by dave on 16.12.20.
//

#ifndef NAMI_CARTESIAN_H
#define NAMI_CARTESIAN_H

#include <concepts>

#include "nami/core/IndexTuple.h"

namespace nami {
  namespace coordinates {
    /**
     * Defines geometric scales
     * @tparam dimension
     * @tparam T
     */
    template <typename T> concept CoordinateSystem = requires {
      std::same_as<decltype(std::remove_reference_t<T>::dimension), core::Dimension_t>;
    };

    template <coordinates::CoordinateSystem CoordinateSystem> inline constexpr auto dimension() {
      return std::remove_reference_t<CoordinateSystem>::dimension;
    }

    template <coordinates::CoordinateSystem CoordinateSystem>
    inline constexpr auto dimension(CoordinateSystem&&) {
      return dimension<CoordinateSystem>();
    }

    template <core::Dimension_t dim> struct Cartesian {
      static constexpr core::Dimension_t dimension = dim;
    };

    struct Polar {
      static constexpr core::Dimension_t dimension = 2;
    };

  }  // namespace coordinates
}  // namespace nami

#endif  // NAMI_CARTESIAN_H
