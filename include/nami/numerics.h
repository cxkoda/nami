//
// Created by dave on 01.12.20.
//

#ifndef NAMI_NUMERICS_H
#define NAMI_NUMERICS_H

namespace nami {
  namespace numerics {
    //    template <typename T> struct NumericalFlux {};
    //    template <typename T> using NumericalFlux = Flux<T>;

    template <typename T> constexpr auto computeHllFlux(const T& leftState, const T& rightState) {
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
}  // namespace nami

#endif  // NAMI_NUMERICS_H
