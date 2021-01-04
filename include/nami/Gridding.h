//
// Created by dave on 16.12.20.
//

#ifndef NAMI_GRIDDING_H
#define NAMI_GRIDDING_H

#include "nami/Cartesian.h"
#include "nami/core/IndexTuple.h"

namespace nami::fv
{
  template <core::Dimension_t dimension>
  struct OrthogonalCoordinateCell {
    std::array<double, dimension> from_{};
    std::array<double, dimension> to_{};
  };

  template <core::Dimension_t dimension>
  struct OrthogonalCoordinateCellInteface {
    std::array<double, dimension> from_{};
    std::array<double, dimension> to_{};
    std::size_t dim_{};
  };

  enum CellInterfaceSide { left, right };

  template <typename T, core::Dimension_t dimension>
  concept OrthogonalGriddingStrategy = requires
  {
    std::is_invocable_v<T, std::array<double, dimension>, std::array<double, dimension>,
                        core::Extent<dimension>>;
  };

  template <core::Dimension_t dimension>
  struct LinearGridding {
    using CellId = std::size_t;
    using Cell = OrthogonalCoordinateCell<dimension>;
    using CellInterface = OrthogonalCoordinateCellInteface<dimension>;
    using IndexTuple = core::IndexTuple<dimension>;

    std::array<double, dimension> dx_{};
    std::array<double, dimension> from_{};
    std::array<double, dimension> to_{};
    core::Extent<dimension> physicalExtent_{};

    LinearGridding(std::array<double, dimension> from, std::array<double, dimension> to,
                   core::Extent<dimension> physicalExtent)
        : from_(std::move(from)), to_(std::move(to)), physicalExtent_(std::move(physicalExtent))
    {
      for (std::size_t dim = 0; dim < dimension; ++dim) {
        dx_[dim] = (to_[dim] - from_[dim]) / static_cast<double>(physicalExtent_[dim]);
      }
    }

    inline constexpr Cell getCell(const IndexTuple& indexTuple) const
    {
      Cell cell{};
      for (std::size_t dim = 0; dim < dimension; ++dim) {
        cell.from_[dim] = indexTuple[dim] * dx_[dim] + from_[dim];
        cell.to_[dim] = cell.from_[dim] + dx_[dim];
      }
      return cell;
    }

    inline constexpr CellInterface getInterface(const IndexTuple& indexTuple,
                                                const std::size_t& dim,
                                                const CellInterfaceSide& side) const
    {
      assert(dim < dimension);
      auto cell{getCell(indexTuple)};
      CellInterface interface{
          .from_ = std::move(cell.from_), .to_ = std::move(cell.to_), .dim_ = dim};

      switch (side) {
        case CellInterfaceSide::left:
          interface.to_[dim] = interface.from_[dim];
          break;
        case CellInterfaceSide::right:
          interface.from_[dim] = interface.to_[dim];
          break;
      }

      return interface;
    }
  };
}  // namespace nami::fv

#endif  // NAMI_GRIDDING_H
