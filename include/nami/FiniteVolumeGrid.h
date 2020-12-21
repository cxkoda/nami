//
// Created by dave on 03.12.20.
//

#ifndef NAMI_FINITEVOLUMEGRID_H
#define NAMI_FINITEVOLUMEGRID_H

#include <algorithm>
#include <array>
#include <cassert>
#include <iterator>
#include <numeric>
#include <tuple>
#include <vector>

#include "Gridding.h"
#include "nami/FiniteVolumeCell.h"
#include "nami/core/ArrayArithmetic.h"
#include "nami/core/IndexTuple.h"

namespace nami {
  namespace fv {
    using CellIdentifier = std::size_t;
    using CellInterfaceIdentifier = std::size_t;

    struct CellInterface {
      CellInterface(CellIdentifier cell1, CellIdentifier cell2) {
        assert(cell1 != cell2);
        if (cell1 < cell2) {
          left_ = cell1;
          right_ = cell2;
        } else {
          left_ = cell2;
          right_ = cell1;
        }
      }

      inline bool operator==(const CellInterface& other) const {
        return std::tie(left_, right_) == std::tie(other.left_, other.right_);
      }

      CellIdentifier left_{};
      CellIdentifier right_{};
    };

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
                                                                const core::Rim_t rim) {
      std::transform(std::begin(extent), std::end(extent), std::begin(extent),
                     [rim](auto s) { return s + 2 * rim; });
      return extent;
    }

    template <core::Rim_t rim, core::Dimension_t dimension, typename OrthogonalGriddingStrategy,
              typename FiniteVolumeCellBuilderType>
    struct OrthogonalFiniteVolumeGrid {
      using IndexTuple = core::IndexTuple<dimension>;
      using Extent = core::Extent<dimension>;
      using Cell = OrthogonalFiniteVolumeCell<dimension>;

      Extent physicalExtent_{};
      IndexTuple startTotalIndex_{};
      IndexTuple endTotalIndex_{};

      std::size_t numberOfPhysicalCells_{};
      std::size_t numberOfTotalCells_{};

      std::vector<Cell> cells_{};
      std::vector<CellInterface> cellInterfaces_{};

      OrthogonalGriddingStrategy orthogonalGriddingStrategy_;
      FiniteVolumeCellBuilderType cellBuilder_;

      OrthogonalFiniteVolumeGrid(Extent physicalExtent,
                                 OrthogonalGriddingStrategy&& orthogonalGriddingStrategy,
                                 FiniteVolumeCellBuilderType&& cellBuilder)
          : physicalExtent_(std::move(physicalExtent)),
            orthogonalGriddingStrategy_(std::move(orthogonalGriddingStrategy)),
            cellBuilder_(std::move(cellBuilder)) {
        auto totalExtent = computeTotalExtent(physicalExtent, rim);
        numberOfPhysicalCells_ = core::computeSize(physicalExtent_);
        numberOfTotalCells_ = core::computeSize(totalExtent);

        std::fill(std::begin(startTotalIndex_), std::end(startTotalIndex_), -rim);
        std::transform(std::begin(physicalExtent_), std::end(physicalExtent_),
                       std::begin(endTotalIndex_), [](const auto size) { return size + rim; });

        cells_ = computeCells(orthogonalGriddingStrategy_, startTotalIndex_, endTotalIndex_,
                              cellBuilder_);
        cellInterfaces_ = computeCellInterfaces();
      }

      inline static std::vector<Cell> computeCells(
          OrthogonalGriddingStrategy& orthogonalGriddingStrategy, const IndexTuple& start,
          const IndexTuple& end, FiniteVolumeCellBuilderType& cellBuilder) {
        using namespace core::array;
        const auto size = core::computeSize(end - start);
        std::vector<Cell> cells{};
        cells.reserve(size);

        core::for_each_index(start, end,
                             [&cells, &orthogonalGriddingStrategy, &cellBuilder](const auto idx) {
                               auto coordinateCell = orthogonalGriddingStrategy.getCell(idx);
                               auto cell = cellBuilder(std::move(coordinateCell));
                               cells.push_back(std::move(cell));
                             });

        return cells;
      }

      template <typename Functor> inline constexpr void forEachCellInterface(Functor&& fun) {
        std::for_each(std::begin(cellInterfaces_), std::end(cellInterfaces_),
                      std::forward<Functor>(fun));
      }

      inline constexpr CellIdentifier getCellId(const IndexTuple& idx) const {
        CellIdentifier id{idx[0] + rim};
        for (std::size_t dim = 1; dim < dimension; ++dim) {
          id *= physicalExtent_[dim];
          id += idx[dim] + rim;
        }
        assert(id < numberOfTotalCells_);
        assert(id > 0);
        return id;
      }

      using IndexTuplePair = std::pair<IndexTuple, IndexTuple>;

      std::vector<CellInterface> computeCellInterfaces() const {
        IndexTuple startInterfaces, endInterfaces;
        for (std::size_t dim = 0; dim < dimension; ++dim) {
          startInterfaces[dim] = -rim + 1;
          endInterfaces[dim] = physicalExtent_[dim] + rim - 1;
        }

        const auto neighbouringPairs = computeNeighbouringPairs(startInterfaces, endInterfaces);
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

      static std::vector<IndexTuplePair> computeNeighbouringPairs(const IndexTuple& start,
                                                                  const IndexTuple& end) {
        std::vector<IndexTuplePair> interfaces{};
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
    };

  }  // namespace fv
}  // namespace nami

#endif  // NAMI_FINITEVOLUMEGRID_H
