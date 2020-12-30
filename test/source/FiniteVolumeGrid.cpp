//
// Created by dave on 04.12.20.
//

#include "nami/FiniteVolumeGrid.h"

#include <doctest/doctest.h>

#include "nami/Cartesian.h"
#include "nami/Gridding.h"

using namespace nami;
using namespace nami::fv;

TEST_CASE("Build Cartesian Gridding in 2 dimensions")
{
  constexpr core::Dimension_t dimension = 2;
  constexpr coordinates::Cartesian<dimension> cartesian{};

  std::array<double, dimension> from{-1, -1};
  std::array<double, dimension> to{1, 1};
  core::Extent<dimension> extent{2, 2};

  LinearGridding gridding{from, to, extent};
  auto cells = computeOrthogonalCells(cartesian, gridding, {0, 0}, {2, 2});

  CHECK(std::size(cells) == 4);
  SUBCASE("All cells should have the same volume")
  {
    for (const auto& cell : cells) {
      CHECK(cell.volume_ == 1);
    }
  }

  SUBCASE("The cell ids have to be in the correct order")
  {
    for (std::size_t idx = 0; idx < cells.size(); ++idx) {
      CHECK(cells[idx].id_ == idx);
    }
  }

  SUBCASE("Checking the cell extents manually")
  {
    using extent_t = decltype(cells[0].from_);
    CHECK(cells[0].from_ == extent_t{-1, -1});
    CHECK(cells[0].to_ == extent_t{0, 0});
    CHECK(cells[1].from_ == extent_t{-1, 0});
    CHECK(cells[1].to_ == extent_t{0, 1});
    CHECK(cells[2].from_ == extent_t{0, -1});
    CHECK(cells[2].to_ == extent_t{1, 0});
    CHECK(cells[3].from_ == extent_t{0, 0});
    CHECK(cells[3].to_ == extent_t{1, 1});
  }
}

TEST_CASE("Neighbouring cells on an orthogonal grid")
{
  const auto neighbours = computeOrthogonalNeighbouringPairs<2>({0, 0}, {3, 2});

  auto findPair = [&neighbours](const decltype(neighbours)::value_type& item) {
    return std::find(std::begin(neighbours), std::end(neighbours), item) != std::end(neighbours);
  };

  CHECK(std::size(neighbours) == 24);

  SUBCASE("Obvious neighbours")
  {
    CHECK(findPair({{0, 0}, {0, 1}}));
    CHECK(findPair({{1, 0}, {0, 0}}));
  }
  SUBCASE("A cell is not its own neighbour") { CHECK(!findPair({{0, 0}, {0, 0}})); }
  SUBCASE("Ghost cells are neighbours")
  {
    CHECK(!findPair({{0, -1}, {0, 0}}));
    CHECK(!findPair({{-1, 1}, {0, 1}}));
  }

  SUBCASE("Not neighbours at larger distances")
  {
    CHECK(!findPair({{0, 0}, {2, 0}}));
    CHECK(!findPair({{0, 0}, {1, 1}}));
  }
}

TEST_CASE("Cartesian Grid")
{
  constexpr core::Dimension_t dimension = 2;
  using CoordinateSystem = nami::coordinates::Cartesian<2>;
  CoordinateSystem cartesian{};
  constexpr core::Rim_t rim = 1;

  std::array<double, dimension> from{-1, -1};
  std::array<double, dimension> to{1, 1};
  core::Extent<dimension> extent{2, 2};

  LinearGridding gridding{from, to, extent};

  //  const OrthogonalFiniteVolumeGrid<rim> grid{extent, std::move(gridding), cartesian};
  //  const OrthogonalFiniteVolumeGrid<rim> grid(cartesian, std::move(gridding), extent);
  //  const OrthogonalFiniteVolumeGrid<rim, CoordinateSystem, decltype(gridding)> grid(cartesian,
  //  std::move(gridding), extent);
  const OrthogonalFiniteVolumeGrid<rim, CoordinateSystem, decltype(gridding)> grid(
      cartesian, std::move(gridding), extent);
  //  const OrthogonalFiniteVolumeGrid<rim, CoordinateSystem> grid(cartesian, gridding, extent);

  SUBCASE("The grid cell id computation has to match the id stored as cell member")
  {
    auto id = grid.getCellId({0, 0});
    CHECK(id == grid.cells_[id].id_);

    id = grid.getCellId({0, 1});
    CHECK(id == grid.cells_[id].id_);
  }
}