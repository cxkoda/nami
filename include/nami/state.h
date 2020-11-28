//
// Created by dave on 28.11.20.
//

#ifndef NAMI_STATE_H
#define NAMI_STATE_H

#include <string>

namespace nami {

  template <typename T> struct State {};
  template <typename T> struct Flux {};

  template <typename... T, typename U> State<T...> operator*(const U& u, State<T...> state) {
    state.quantity_ *= u;
    return state;
  }

  namespace numerics {
    //    template <typename T> struct NumericalFlux {};
    //    template <typename T> using NumericalFlux = Flux<T>;

    template <typename T> auto computeHllFlux(const T& leftState, const T& rightState) {
      auto leftSpeed = computeSignalSpeed(leftState);
      auto rightSpeed = computeSignalSpeed(rightState);
      auto leftFlux = computeFlux(leftState);
      auto rightFlux = computeFlux(rightState);
      using fluxT = decltype(leftFlux);
      fluxT result = (rightSpeed * rightState - leftSpeed * leftState + leftFlux + rightFlux)
                     / (rightSpeed - leftSpeed);
      return result;
    }

  }  // namespace numerics

  namespace burger {
    enum VariableType { quantity };

    template <typename T> struct State : nami::State<T> {
      constexpr static VariableType allVariables[] = {quantity};
      T quantity_{};
    };
    template <typename T> struct Flux : nami::Flux<T> { T flux_{}; };

    template <typename T> T computeSignalSpeed(const State<T>& state) { return state.quantity_; }

    template <typename T> Flux<T> computeFlux(const State<T>& state) {
      Flux<T> flux{0.5 * state.quantity_ * state.quantity_};
      return flux;
    }

  }  // namespace burger

}  // namespace nami

#endif  // NAMI_STATE_H
