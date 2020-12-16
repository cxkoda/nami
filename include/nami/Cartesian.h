//
// Created by dave on 16.12.20.
//

#ifndef NAMI_CARTESIAN_H
#define NAMI_CARTESIAN_H

#include "nami/core/IndexTuple.h"

namespace nami {
  namespace coordinates {
    /**
     * Defines geometric scales
     * @tparam dimension
     * @tparam T
     */
    template <core::Dimension_t dimension, typename T> struct CoordinateSystem {};

    template <core::Dimension_t dimension> struct Cartesian {};

  }  // namespace coordinates
}  // namespace nami

#endif  // NAMI_CARTESIAN_H
