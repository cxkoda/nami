//
// Created by dave on 16.12.20.
//

#ifndef NAMI_INDEXTUPLE_H
#define NAMI_INDEXTUPLE_H

#include <array>
#include <cassert>
#include <numeric>

namespace nami::core {
  using Dimension_t = std::size_t;

  template <Dimension_t dimension> using IndexTuple = std::array<long, dimension>;

  namespace internal {
    template <std::size_t iterDimension, Dimension_t dimension, typename Functor>
    inline constexpr void for_each_impl(IndexTuple<dimension>& idx,
                                        const IndexTuple<dimension>& start,
                                        const IndexTuple<dimension>& end, Functor&& fun) {
      if constexpr (iterDimension != dimension) {
        for (idx[iterDimension] = start[iterDimension]; idx[iterDimension] < end[iterDimension];
             ++idx[iterDimension]) {
          for_each_impl<iterDimension + 1>(idx, start, end, std::forward<Functor>(fun));
        }
      } else {
        fun(idx);
      }
    }
  }  // namespace internal

  template <Dimension_t dimension, typename Functor>
  inline constexpr auto for_each_index(const IndexTuple<dimension>& start,
                                       const IndexTuple<dimension>& end, Functor&& fun) {
    IndexTuple<dimension> idx{};
    return internal::for_each_impl<0>(idx, start, end, std::forward<Functor>(fun));
  }

  template <Dimension_t dimension, typename T>
  inline constexpr T computeProduct(const std::array<T, dimension>& data) {
    return std::accumulate(std::begin(data), std::end(data), T{1}, std::multiplies<T>{});
  }

  using Rim_t = std::size_t;
  template <core::Dimension_t dimension> using Extent = std::array<std::size_t, dimension>;

  template <core::Dimension_t dimension, typename T>
  inline constexpr std::size_t computeSize(const std::array<T, dimension>& extent) {
    for (const auto& val : extent) {
      assert(val >= 0);
    }
    return computeProduct(extent);
  }

}  // namespace nami::core
#endif  // NAMI_INDEXTUPLE_H
