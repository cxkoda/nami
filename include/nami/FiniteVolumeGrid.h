//
// Created by dave on 03.12.20.
//

#ifndef NAMI_FINITEVOLUMEGRID_H
#define NAMI_FINITEVOLUMEGRID_H

#include <algorithm>
#include <array>
#include <cassert>
#include <concepts>
#include <iterator>
#include <numeric>
#include <tuple>
#include <vector>

#include "Gridding.h"
#include "nami/FiniteVolumeCell.h"
#include "nami/core/ArrayArithmetic.h"
#include "nami/core/IndexTuple.h"

namespace nami
{
  namespace fv
  {
    using CellInterfaceIdentifier = std::size_t;

    struct CellInterface {
      CellInterface(CellIdentifier cell1, CellIdentifier cell2)
      {
        assert(cell1 != cell2);
        if (cell1 < cell2) {
          left_ = cell1;
          right_ = cell2;
        } else {
          left_ = cell2;
          right_ = cell1;
        }
      }

      inline bool operator==(const CellInterface& other) const
      {
        return std::tie(left_, right_) == std::tie(other.left_, other.right_);
      }

      CellIdentifier left_{};
      CellIdentifier right_{};
    };

    template <typename T>
    concept CellInterfaceInvocable = std::invocable<T, const CellInterface&>;

    //  template <typename GridType> struct FiniteVolumeGrid {
    //    template <typename Functor> inline constexpr decltype(auto) forEachCell(Functor&& fun) {
    //      return static_cast<GridType*>(this)->forEachCell(std::forward<Functor>(fun));
    //    }
    //    template <typename Functor>
    //    inline constexpr decltype(auto) forEachCellInterface(Functor&& fun) {
    //      return static_cast<GridType*>(this)->forEachCellInterface(std::forward<Functor>(fun));
    //    }
    //  };

    template <core::Dimension_t dimension>
    inline constexpr core::Extent<dimension> computeTotalExtent(core::Extent<dimension> extent,
                                                                const core::Rim_t rim)
    {
      std::transform(std::begin(extent), std::end(extent), std::begin(extent),
                     [rim](auto s) { return s + 2 * rim; });
      return extent;
    }

    template <coordinates::CoordinateSystem CoordinateSystem,
              fv::OrthogonalGriddingStrategy<coordinates::dimension<CoordinateSystem>()>
                  OrthogonalGriddingStrategy,
              OrthogonalFiniteVolumeCellBuilder<CoordinateSystem> CellBuilder
              = DefaultOrthogonalFiniteVolumeCellBuilder<std::remove_cvref_t<CoordinateSystem>>,
              core::Dimension_t dimension = coordinates::dimension<CoordinateSystem>()>
    inline constexpr auto computeOrthogonalCells(
        CoordinateSystem&& coordinateSystem,
        OrthogonalGriddingStrategy&& orthogonalGriddingStrategy,
        const core::IndexTuple<dimension>& start, const core::IndexTuple<dimension>& end,
        CellBuilder&& cellBuilder = CellBuilder{})
    {
      using namespace core::array;
      const auto size = core::computeSize(end - start);
      using Cell_t = typename std::remove_cvref_t<CellBuilder>::Cell_t;

      std::vector<Cell_t> cells{};
      cells.reserve(size);

      CellIdentifier cellId = 0;
      core::for_each_index(start, end,
                           [&cells, &orthogonalGriddingStrategy, &cellBuilder, &cellId,
                            &coordinateSystem](const auto idx) {
                             auto coordinateCell = orthogonalGriddingStrategy.getCell(idx);
                             auto cell = cellBuilder(coordinateSystem, std::move(coordinateCell));
                             cell.id_ = cellId++;
                             cells.push_back(std::move(cell));
                           });

      return cells;
    }

    //    template <core::Dimension_t dimension>
    //    struct IndexTuplePair {
    //      core::IndexTuple<dimension> left_, right_;
    //      IndexTuplePair(core::IndexTuple<dimension> left, core::IndexTuple<dimension> right)
    //      {
    //        if (left < right) {
    //          left_ = std::move(left);
    //          right_ = std::move(right);
    //        } else {
    //          left_ = std::move(right);
    //          right_ = std::move(left);
    //        }
    //      }
    //      inline constexpr bool operator==(const IndexTuplePair& other) const {
    //        return std::tie(left_, right_) == std::tie(other.left_, other.right_);
    //      }
    //    };

    template <core::Dimension_t dimension>
    using IndexTuplePair = std::pair<core::IndexTuple<dimension>, core::IndexTuple<dimension>>;

    /**
     * Compute the pairs of neighbouring cells (interfaces with O in the sketch)
     * The inner pairs are counted twice
     * The outer pairs are counted once
     *  -------------------------------
     *  |         |         |         |
     *  |         |         |   end   |
     *  |         |         |         |
     *  -----O---------O---------------
     *  |         |         |         |
     *  O         O         O         |
     *  |         |         |         |
     *  -----O---------O---------------
     *  |         |         |         |
     *  O  start  O         O         |
     *  |         |         |         |
     *  -----O---------O---------------
     *
     * @tparam dimension
     * @param start IndexTuple of the leftmost corner
     * @param end IndexTuple of the rightmost corner
     * @return List of IndexTuple Pairs
     */
    template <core::Dimension_t dimension>
    inline constexpr std::vector<IndexTuplePair<dimension>> computeOrthogonalNeighbouringPairs(
        const core::IndexTuple<dimension>& start, const core::IndexTuple<dimension>& end)
    {
      std::vector<IndexTuplePair<dimension>> interfaces{};
      core::for_each_index(start, end, [&interfaces](const auto& idxTuple) {
        for (std::size_t dim = 0; dim < dimension; ++dim) {
          auto idxTupleNeighbour = idxTuple;
          idxTupleNeighbour[dim] += 1;
          interfaces.push_back({idxTuple, idxTupleNeighbour});
          idxTupleNeighbour[dim] -= 2;
          interfaces.push_back({idxTuple, idxTupleNeighbour});
        }
      });

      return interfaces;
    }

    template <core::Rim_t rim, coordinates::CoordinateSystem CoordinateSystem,
              fv::OrthogonalGriddingStrategy<coordinates::dimension<CoordinateSystem>()>
                  OrthogonalGriddingStrategy,
              OrthogonalFiniteVolumeCellBuilder<CoordinateSystem> CellBuilder
              = DefaultOrthogonalFiniteVolumeCellBuilder<std::remove_cvref_t<CoordinateSystem>>>
    struct OrthogonalFiniteVolumeGrid {
      constexpr static core::Dimension_t dimension = coordinates::dimension<CoordinateSystem>();
      using IndexTuple = core::IndexTuple<dimension>;
      using Extent = core::Extent<dimension>;
      using Cell = typename CellBuilder::Cell_t;

      Extent physicalExtent_{};
      IndexTuple startTotalIndex_{};
      IndexTuple endTotalIndex_{};

      std::size_t numberOfPhysicalCells_{};
      std::size_t numberOfTotalCells_{};

      std::vector<Cell> cells_{};
      std::vector<CellInterface> cellInterfaces_{};

      OrthogonalGriddingStrategy orthogonalGriddingStrategy_;
      CellBuilder cellBuilder_;

      OrthogonalFiniteVolumeGrid(CoordinateSystem& coordinateSystem,
                                 OrthogonalGriddingStrategy orthogonalGriddingStrategy,
                                 Extent physicalExtent, CellBuilder cellBuilder = CellBuilder{})
          : physicalExtent_(std::move(physicalExtent)),
            orthogonalGriddingStrategy_(std::move(orthogonalGriddingStrategy)),
            cellBuilder_(std::move(cellBuilder))
      {
        auto totalExtent = computeTotalExtent(physicalExtent, rim);
        numberOfPhysicalCells_ = core::computeSize(physicalExtent_);
        numberOfTotalCells_ = core::computeSize(totalExtent);

        std::fill(std::begin(startTotalIndex_), std::end(startTotalIndex_), -rim);
        std::transform(std::begin(physicalExtent_), std::end(physicalExtent_),
                       std::begin(endTotalIndex_), [](const auto size) { return size + rim; });

        cells_ = computeOrthogonalCells(coordinateSystem, orthogonalGriddingStrategy_,
                                        startTotalIndex_, endTotalIndex_, cellBuilder_);
        cellInterfaces_ = computeCellInterfaces();
      }

      template <CellInterfaceInvocable Functor>
      inline constexpr void forEachCellInterface(Functor&& fun) const
      {
        std::for_each(std::begin(cellInterfaces_), std::end(cellInterfaces_),
                      std::forward<Functor>(fun));
      }

      inline constexpr CellIdentifier getCellId(const IndexTuple& idx) const
      {
        CellIdentifier id{idx[0] + rim};
        for (std::size_t dim = 1; dim < dimension; ++dim) {
          id *= physicalExtent_[dim];
          id += idx[dim] + rim;
        }
        assert(id < numberOfTotalCells_);
        assert(id > 0);
        return id;
      }

      std::vector<CellInterface> computeCellInterfaces() const
      {
        IndexTuple startInterfaces, endInterfaces;
        for (std::size_t dim = 0; dim < dimension; ++dim) {
          startInterfaces[dim] = -rim + 1;
          endInterfaces[dim] = physicalExtent_[dim] + rim - 1;
        }

        const auto neighbouringPairs
            = computeOrthogonalNeighbouringPairs(startInterfaces, endInterfaces);
        std::vector<CellInterface> cellInterfaces{};
        cellInterfaces.reserve(neighbouringPairs.size());

        std::transform(std::begin(neighbouringPairs), std::end(neighbouringPairs),
                       std::back_inserter(cellInterfaces), [this](const auto& pair) {
                         return CellInterface{getCellId(pair.first), getCellId(pair.second)};
                       });

        // Remove duplicates
        const auto last = std::unique(std::begin(cellInterfaces), std::end(cellInterfaces));
        cellInterfaces.erase(last, std::end(cellInterfaces));
        cellInterfaces.shrink_to_fit();

        return cellInterfaces;
      }
    };

  }  // namespace fv
}  // namespace nami

#endif  // NAMI_FINITEVOLUMEGRID_H
