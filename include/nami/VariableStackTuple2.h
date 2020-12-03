//
// Created by dave on 28.11.20.
//

#ifndef NAMI_VARIABLESTACK_H
#define NAMI_VARIABLESTACK_H

#include <string>
#include <tuple>
#include <type_traits>

namespace nami::attempt3 {

  template <std::size_t idx, typename Iterable> constexpr inline auto&& get(Iterable&& object) {
    return std::get<idx>(std::forward<Iterable>(object).data);
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

  template <typename Iterable>
  inline constexpr Iterable& operator+=(Iterable& lhs, const Iterable& rhs) {
    internal::transform(
        lhs, rhs, lhs, [](auto& a, const auto& b) { return a + b; },
        //        std::index_sequence_for<T...>{});
        std::make_integer_sequence<std::size_t,
                                   std::tuple_size<decltype(Iterable::data)>::value>{});
    //    std::make_integer_sequence<std::size_t, Iterable::size>{});
    return lhs;
  }

  template <typename Iterable>
  inline constexpr Iterable operator+(Iterable lhs, const Iterable& rhs) {
    lhs += rhs;
    return lhs;
  }

  template <typename... Field> using Fields = std::tuple<Field...>;

  //  template<typename T>
  //  struct Generic {
  //    constexpr static char name[] = "Generic";
  //    T data_;
  //  };

}  // namespace nami::attempt3
#endif  // NAMI_VARIABLESTACK_H
