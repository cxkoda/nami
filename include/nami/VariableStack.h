//
// Created by dave on 28.11.20.
//

#ifndef NAMI_VARIABLESTACK_H
#define NAMI_VARIABLESTACK_H

#include <string>
#include <type_traits>
#include <tuple>

namespace nami {

  template <typename... T> struct VariableStack : std::tuple<T...> {
    VariableStack(T&&... args) : std::tuple<T...>{std::forward<T>(args)...} {}
  };

  namespace internal {
    template <typename InputType1, typename InputType2, typename OutputType, std::size_t... Is,
              typename BinaryOperation>
    inline constexpr void transform(const InputType1& input1, const InputType2& input2,
                                    OutputType& output, BinaryOperation binary_op,
                                    std::integer_sequence<std::size_t, Is...>) {
      (..., (std::get<Is>(output) = binary_op(std::get<Is>(input1), std::get<Is>(input2))));
    }
  }  // namespace internal

  template <typename... T>
  inline constexpr VariableStack<T...>& operator+=(VariableStack<T...>& lhs,
                                                   const VariableStack<T...>& rhs) {
    internal::transform(
        lhs, rhs, lhs, [](auto& a, const auto& b) { return a + b; },
        std::index_sequence_for<T...>{});
    return lhs;
  }

  template <typename... T>
  inline constexpr VariableStack<T...> operator+(VariableStack<T...> lhs,
                                                 const VariableStack<T...>& rhs) {
    lhs += rhs;
    return lhs;
  }


  namespace burger {
    enum VariableType { quantity };

    template <typename T> struct BurgerStack: VariableStack<T> {
      constexpr static VariableType allVariables[] = {quantity};
      T quantity_{};
    };
    //    template <typename T> struct Flux : nami::Flux<T> { T flux_{}; };
    //
    //    template <typename T> T computeSignalSpeed(const State<T>& state) { return
    //    state.quantity_; }
    //
    //    template <typename T> Flux<T> computeFlux(const State<T>& state) {
    //      Flux<T> flux{0.5 * state.quantity_ * state.quantity_};
    //      return flux;
    //    }

  }  // namespace burger

}  // namespace nami

#endif  // NAMI_VARIABLESTACK_H
