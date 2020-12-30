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
  using CellIdentifier = std::size_t;

  template <coordinates::CoordinateSystem CoordinateSystem> struct OrthogonalFiniteVolumeCell {
    static constexpr auto dimension = CoordinateSystem::dimension;
    using CoordinateSystem_t = CoordinateSystem;

    std::array<double, dimension> from_{};
    std::array<double, dimension> to_{};
    std::array<double, dimension> center_{};
    double volume_{};
    CellIdentifier id_{};
  };

  template <typename T, typename CoordinateSystem_t> concept OrthogonalFiniteVolumeCellBuilder
      = std::invocable<T, const CoordinateSystem_t&,
                       OrthogonalCoordinateCell<coordinates::dimension<CoordinateSystem_t>()>>;

  template <coordinates::CoordinateSystem CoordinateSystem_t>
  struct DefaultOrthogonalFiniteVolumeCellBuilder {};

  template <core::Dimension_t dimension>
  struct DefaultOrthogonalFiniteVolumeCellBuilder<coordinates::Cartesian<dimension>> {
    using CoordinateSystem_t = coordinates::Cartesian<dimension>;
    using Cell_t = OrthogonalFiniteVolumeCell<CoordinateSystem_t>;

    inline constexpr auto operator()(
        [[maybe_unused]] const CoordinateSystem_t& coordinateSystem,
        OrthogonalCoordinateCell<CoordinateSystem_t::dimension> coordinateCell) {
      std::array<double, dimension> center{};
      std::transform(std::begin(coordinateCell.to_), std::end(coordinateCell.to_),
                     std::begin(coordinateCell.from_), std::begin(center),
                     [](const auto to, const auto from) { return 0.5 * (to + from); });

      double volume = std::inner_product(
          std::begin(coordinateCell.to_), std::end(coordinateCell.to_),
          std::begin(coordinateCell.from_), 1, std::multiplies<double>{}, std::minus<double>{});

      OrthogonalFiniteVolumeCell<CoordinateSystem_t> cell{std::move(coordinateCell.from_),
                                                          std::move(coordinateCell.to_),
                                                          std::move(center), std::move(volume)};
      return cell;
    }
  };
}  // namespace nami::fv

#endif  // NAMI_FINITEVOLUMECELL_H
