//
// Created by dave on 29.12.20.
//

#include "nami/timestepping/ExplicitEuler.h"

#include <doctest/doctest.h>

#include <utility>

using namespace nami::timestepping;

struct ExponentialOde {
  double lambda_;
  ExponentialOde(double lambda) : lambda_{std::move(lambda)} {}
  auto operator()([[maybe_unused]] const Time_t time, State_t auto&& state) {
    return lambda_ * state;
  }
};

template <typename Rhs> struct DummyProxy {
  template <typename... Args> DummyProxy(Args&&... args) : rhs_{std::forward<Args>(args)...} {}

  inline constexpr auto operator()(const Time_t time, State_t auto&& state,
                                   [[maybe_unused]] const IntegrationStep_t step) {
    return rhs_(time, std::forward<decltype(state)>(state));
  }

  Rhs rhs_{};
};

TEST_CASE("Make an explicit euler step for an exponential damping ode") {
  DummyProxy<ExponentialOde> rhs{-3.};
  double state{1.};
  double time{0.};
  double timestep{0.1};
  ExplicitEuler euler;

  euler(time, timestep, state, rhs);
  CHECK(state == 0.7);
}
