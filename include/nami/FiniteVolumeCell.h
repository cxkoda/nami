//
// Created by dave on 16.12.20.
//

#ifndef NAMI_FINITEVOLUMECELL_H
#define NAMI_FINITEVOLUMECELL_H

#include <algorithm>
#include <memory>

#include "Gridding.h"
#include "nami/Cartesian.h"
#include "nami/core/IndexTuple.h"

namespace nami::fv {

  template <core::Dimension_t dimension> struct OrthogonalFiniteVolumeCell {
    std::array<double, dimension> from_{};
    std::array<double, dimension> to_{};
    std::array<double, dimension> center_{};
    double volume_{};
  };

  template <typename CoordinateSystem> struct FiniteVolumeCellBuilder {};

  template <core::Dimension_t dimension>
  struct FiniteVolumeCellBuilder<coordinates::Cartesian<dimension>> {
    inline constexpr auto operator()(OrthogonalCoordinateCell<dimension> coordinateCell) {
      std::array<double, dimension> center{};
      std::transform(std::begin(coordinateCell.to_), std::end(coordinateCell.to_),
                     std::begin(coordinateCell.from_), std::begin(center),
                     [](const auto to, const auto from) { return 0.5 * (to + from); });

      double volume = std::inner_product(
          std::begin(coordinateCell.to_), std::end(coordinateCell.to_),
          std::begin(coordinateCell.from_), 1, std::multiplies<double>{}, std::minus<double>{});

      OrthogonalFiniteVolumeCell<dimension> cell{std::move(coordinateCell.from_),
                                                 std::move(coordinateCell.to_), std::move(center),
                                                 std::move(volume)};
      return cell;
    }
  };
}  // namespace nami::fv

#endif  // NAMI_FINITEVOLUMECELL_H
