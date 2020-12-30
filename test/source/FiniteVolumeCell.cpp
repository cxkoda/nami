//
// Created by dave on 04.12.20.
//

#include "nami/FiniteVolumeCell.h"

#include <doctest/doctest.h>

#include "nami/Cartesian.h"

using namespace nami;
using namespace nami::fv;

TEST_CASE("Default Cartesian FV Cell Builder")
{
  constexpr core::Dimension_t dimension = 2;
  using CoordinateSystem = coordinates::Cartesian<dimension>;
  constexpr CoordinateSystem cartesian{};

  DefaultOrthogonalFiniteVolumeCellBuilder<CoordinateSystem> builder{};
  OrthogonalCoordinateCell<dimension> coordinateCell{{1., 2.}, {3., 5.}};

  auto cell = builder(cartesian, coordinateCell);
  SUBCASE("computes the cell centers as arithmetic means")
  {
    CHECK(cell.center_[0] == 2);
    CHECK(cell.center_[1] == 3.5);
  }
  SUBCASE("Integrates the volume") { CHECK(cell.volume_ == 6.0); }
  SUBCASE("transfers the remaining fields")
  {
    CHECK(cell.from_[0] == 1);
    CHECK(cell.from_[1] == 2);
    CHECK(cell.to_[0] == 3);
    CHECK(cell.to_[1] == 5);
  }
}
