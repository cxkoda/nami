//
// Created by dave on 04.12.20.
//

#include "nami/FiniteVolumeGrid.h"

#include <doctest/doctest.h>

#include "nami/Cartesian.h"
#include "nami/Gridding.h"

using namespace nami;
using namespace nami::fv;

TEST_CASE("FV Grid") {
  constexpr core::Dimension_t dimension = 2;

  FiniteVolumeCellBuilder<coordinates::Cartesian<dimension>> builder{};

  std::array<double, dimension> from{-1, -1};
  std::array<double, dimension> to{1, 1};
  core::Extent<dimension> extent{2, 2};

  LinearGridding gridding{from, to, extent};

  OrthogonalFiniteVolumeGrid<1, 2, decltype(gridding), decltype(builder)> grid{
      extent, std::move(gridding), std::move(builder)};
}