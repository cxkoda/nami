//
// Created by dave on 03.12.20.
//

#include <doctest/doctest.h>
#include <nami/Burger.h>

#include <string>

using namespace nami::Burger;

TEST_CASE("Burger System")
{
  Stack<float> state{3.};

  CHECK(computeSignalSpeed(state) == 3.);
  CHECK(computeFlux(state) == 4.5);
}
