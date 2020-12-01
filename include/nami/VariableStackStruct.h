//
// Created by dave on 01.12.20.
//

#ifndef NAMI_VARIABLESTACKSTRUCT_H
#define NAMI_VARIABLESTACKSTRUCT_H

#include <tuple>
#include <utility>

namespace nami {

  namespace attempt2 {

    //    template <typename StackType> struct VariableStack {};

    template <std::size_t idx, typename VariableStack>
    inline constexpr auto&& get(VariableStack&& stack) noexcept {
      //      return std::get<idx>(stack.getAll());
      //          return std::get<idx>(stack.all);
      return std::forward<VariableStack>(stack).template get<idx>();
    }

    namespace internal {
      template <typename InputType1, typename InputType2, typename OutputType, std::size_t... Is,
                typename BinaryOperation>
      inline constexpr void transform(const InputType1& input1, const InputType2& input2,
                                      OutputType& output, BinaryOperation binary_op,
                                      std::integer_sequence<std::size_t, Is...>) {
        (..., (get<Is>(output) = binary_op(get<Is>(input1), get<Is>(input2))));
      }
    }  // namespace internal

    template <typename StackType>
    inline constexpr void operator+=(StackType& lhs, const StackType& rhs) {
      internal::transform(
          lhs, rhs, lhs, [](auto& a, const auto& b) { return a + b; },
          std::make_integer_sequence<std::size_t,
                                     // std::tuple_size<decltype(lhs.getAll())>::value>{});
                                     StackType::size>{});
      // std::tuple_size<decltype(StackType::all)>::value>{});
      //      return lhs;
    }

    template <typename StackType>
    inline constexpr StackType operator+(StackType lhs, const StackType& rhs) {
      lhs += rhs;
      return lhs;
    }

  }  // namespace attempt2

}  // namespace nami

#endif  // NAMI_VARIABLESTACKSTRUCT_H
