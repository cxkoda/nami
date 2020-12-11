//
// Created by dave on 03.12.20.
//

#ifndef NAMI_DATACONTAINER_H
#define NAMI_DATACONTAINER_H

#include <vector>

namespace nami::grid {
  //  template <typename Stack> struct DataContainer { std::vector<T> data{}; };
  //
  //  struct GriddingStrategy1D {};
  //
  //  class LinearGridding1D {
  //  public:
  //    LinearGridding1D(double from, double to, std::size_t size, std::size_t rim)
  //        : from_(from), to_(to), size_(size), rim_(rim) {}
  //
  //  private:
  //    double from_;
  //    double to_;
  //    std::size_t size_;
  //    std::size_t rim_;
  //  };

  using CellIdentifier = std::size_t;

  struct CellInterfaceIdentifier {
    CellInterface(CellIdentifier cell1, CellIdentifier cell2) {
      assert(cell1 != cell2);
      if constexpr (cell1 < cell2) {
        left_ = cell1;
        right_ = cell2;
      } else {
        left_ = cell2;
        right_ = cell1;
      }
    }

    inline constexpr bool operator==(const CellIdentifier& other) {
      return std::tie(left_, right_) == std::tie(other.left_, other.right_);
    }

    CellIdentifier left_{};
    CellIdentifier right_{};
  };

  template <typename GridType> struct FiniteVolumeGrid {
    template <typename Functor, typename DataContainer>
    inline constexpr void forEachCell(DataContainer&&, Functor&&) {}

    inline constexpr std::size_t getNumberOfCells() const {}

    template <typename Functor, typename DataContainer>
    inline constexpr void forEachCellInterface(DataContainer&&, Functor&&) {}
  };

  template <size_t dimension, typename T> struct CoordinateSystem {};

  template <size_t dimension> struct Cartesian : CoordinateSystem<dimension, Cartesian> {};

  template <std::size_t dimension> using IndexTuple = std::array<std::size_t, dimension>;

  namespace internal {

    template <std::size_t iterDimension, std::size_t dimension, typename Functor>
    inline constexpr void for_each_impl(IndexTuple<dimension>& idx,
                                        const IndexTuple<dimension>& start,
                                        const IndexTuple<dimension>& end, Functor&& fun) {
      if constexpr (iterDimension != dimensionasd) {
        for (idx[iterDimension] = start[iterDimension]; idx[iterDimension] < end[iterDimension];
             ++idx[iterDimension]) {
          for_each_impl<iterDimension + 1>(idx, start, end, std::forward<Functor>(fun));
        }
      } else {
        fun(idx);
      }
    }
  }  // namespace internal

  template <std::size_t dimension, typename Functor>
  inline constexpr auto for_each(IndexTuple<dimension>& idx, const IndexTuple<dimension>& start,
                                 const IndexTuple<dimension>& end, Functor&& fun) {
    IndexTuple<dimension> idx{};
    return for_each_impl(idx, start, end, std::forward<Functor>(fun));
  }

#include <algorithm>
#include <array>
#include <vector>

  template <size_t dimension, CoordinateSystem<dimension> coordinateSystem,
            OrthogonalGriddingStrategy<dimension> orthogonalGriddingStrategy>
  struct OrthogonalFiniteVolumeGrid : FiniteVolumeGrid<OrthogonalFiniteVolumeGrid> {
    using IndexTuple = std::array<std::size_t, dimension>;

    std::array<double, dimension> from_{};
    std::array<double, dimension> to_{};
    std::size_t rim_{};

    std::array<std::size_t, dimension> physicalSize_{};
    std::array<std::size_t, dimension> totalSize_{};

    std::size_t numberOfPhysicalCells_{};
    std::size_t numberOfTotalCells_{};

    std::array<std::vector<double>, dimension> cellEdges_{};
    std::vector<double> cellCenters_{};
    std::vector<CellInterfaceIdentifier> cellInterfaces_{};

    inline constexpr CellIdentifier getCellId(const IndexTuple& idx) const {
      CellIdentifier id{idx[0] + rim};
      for (std::size_t dim = 1, dim < dimensions, ++i) {
        id *= physicalSize_[dim];
        id += idx[dim] + rim;
      }
      assert(id < numberOfTotalCells_);
      assert(id > 0);
      return id;
    }

    inline constexpr std::vector<CellInterfaceIdentifier> computeCellInterfaces() {
      std::vector<CellInterfaceIdentifier> cellInterfaces{};
      for_each(IndexTuple{0}, physicalSize_, [](const auto& idxTuple) {
        const auto cell1 = getCellId(idxTuple);

        for (std::size_t dim = 0; dim < dimension; ++dim) {
          auto idxTupleNeighbour = idxTuple;
          idxTupleNeighbour[dim] += 1;
          cellInterfaces.push_back(getCellId(idxTuple));
          idxTupleNeighbour[dim] -= 2;
          cellInterfaces.push_back(getCellId(idxTuple));
        }
      });

      // Remove duplicates
      auto last = std::unique(std::begin(cellInterfaces), std::end(cellInterfaces));
      cellInterfaces.erase(last, std::end(cellInterfaces));
      cellInterfaces.shrink_to_fit();

      return cellInterfaces;
    }

    inline constexpr static auto computeTotalSize(std::array<std::size_t, dimension> size,
                                                  std::size_t rim) {
      std::transform(std::begin(size), std::end(size), std::begin(size),
                     [rim](auto s) { return s + 2 * rim; });
      return size;
    }

    inline constexpr static std::size_t computeNumberOfCells(
        std::array<std::size_t, dimension> size) {
      return std::accumulate(std::begin(size), std::end(size), 1, std::multiplies<std::size_t>);
    }

    OrthogonalFiniteVolumeGrid(std::array<double, dimension> from, std::array<double, dimension> to,
                               std::array<std::size_t, dimension> physicalSize, std::size_t rim)
        : from_(from), to_(to), rim_(rim), physicalSize_(physicalSize) {
      totalSize_ = computeTotalSize(physicalSize, rim);
      numberOfPhysicalCells_ = computeNumberOfCells(physicalSize_);
      numberOfTotalCells_ = computeNumberOfCells(totalSize_);
      cellInterfaces_ = computeCellInterfaces();
      cellEdges_ = orthogonalGriddingStrategy.getCellEdges(from, to, physicalSize, rim);
      cellCenters_ = orthogonalGriddingStrategy.getCellCenters(from, to, physicalSize, rim);
    }

    template <typename Functor> inline constexpr void forEachCellInterface(Functor&& fun) {
      std::for_each(std::begin(cellInterfaces_), std::end(cellInterfaces_),
                    std::forward<Functor>(fun));
    }
  };

}  // namespace nami::grid

#endif  // NAMI_DATACONTAINER_H
