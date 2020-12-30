//
// Created by dave on 04.12.20.
//

#include "nami/Gridding.h"

#include <doctest/doctest.h>

using namespace nami;
using namespace nami::fv;

TEST_CASE("Linear Gridding dx")
{
  constexpr core::Dimension_t dimension = 2;

  std::array<double, dimension> from{-1, 2};
  std::array<double, dimension> to{2, 7};
  core::Extent<dimension> extent{10, 20};

  LinearGridding gridding{from, to, extent};

  CHECK_EQ(gridding.dx_[0], 3. / 10.);
  CHECK_EQ(gridding.dx_[1], 5. / 20.);
}

TEST_CASE("Linear Gridding Cells")
{
  constexpr core::Dimension_t dimension = 2;

  std::array<double, dimension> from{-1, -1};
  std::array<double, dimension> to{1, 1};
  core::Extent<dimension> extent{2, 2};

  LinearGridding gridding{from, to, extent};

  auto cell = gridding.getCell({0, 0});
  CHECK(cell.from_[0] == -1);
  CHECK(cell.from_[1] == -1);
  CHECK(cell.to_[0] == 0);
  CHECK(cell.to_[1] == 0);

  cell = gridding.getCell({0, 1});
  CHECK(cell.from_[0] == -1);
  CHECK(cell.from_[1] == 0);
  CHECK(cell.to_[0] == 0);
  CHECK(cell.to_[1] == 1);
}
