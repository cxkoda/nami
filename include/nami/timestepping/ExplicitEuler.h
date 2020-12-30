//
// Created by dave on 29.12.20.
//

#ifndef NAMI_EXPLICITEULER_H
#define NAMI_EXPLICITEULER_H

#include <concepts>
#include <utility>

namespace nami::timestepping {
  using Time_t = double;
  using Timestep_t = double;
  using IntegrationStep_t = std::size_t;

  template <typename T> concept State_t = true;

  template <typename T, typename State> concept Rhs_t = std::invocable<T, Time_t, State>;

  template <typename T, typename State> concept RhsProxy_t
      = std::invocable<T, Time_t, State, IntegrationStep_t>;

  template <typename T, typename State> concept TimeIntegrator_t
      = std::invocable<T, Time_t, Timestep_t, State, IntegrationStep_t>;

  struct ExplicitEuler {
    template <State_t State> inline constexpr void operator()(Time_t time, Timestep_t timestep,
                                                              State&& state,
                                                              RhsProxy_t<State> auto&& rhs) {
      state += timestep * rhs(time, std::forward<State>(state), 0);
    }
  };
}  // namespace nami::timestepping

#endif  // NAMI_EXPLICITEULER_H
