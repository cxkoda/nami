#include <doctest/doctest.h>
#include <nami/state.h>

#include <string>

TEST_CASE("BurgerStateArithmetic") {
  using namespace nami;

  burger::State<double> state{2};
  const double value = 10;
  auto state2 = value * state;

  CHECK(state2.quantity_ == 20);
}