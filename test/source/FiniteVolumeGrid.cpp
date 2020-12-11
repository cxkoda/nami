//
// Created by dave on 04.12.20.
//

#include <doctest/doctest.h>
#include <nami/DataContainer.h>

using namespace nami::Burger;

TEST_CASE("Burger System") {
  Stack<float> state{3.};

      CHECK(computeSignalSpeed(state) == 3.);
      CHECK(computeFlux(state) == 4.5);
}
