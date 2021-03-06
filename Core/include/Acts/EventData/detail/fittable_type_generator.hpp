// This file is part of the Acts project.
//
// Copyright (C) 2016-2018 Acts project team
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <variant>
#include "Acts/Utilities/ParameterDefinitions.hpp"

#include <type_traits>

#include <boost/hana/append.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/range.hpp>
#include <boost/hana/remove_at.hpp>
#include <boost/hana/transform.hpp>
#include <boost/hana/tuple.hpp>

namespace Acts {
// forward declaration
template <typename identifier_t, ParID_t... params>
class Measurement;

/// @cond detail
namespace detail {

  namespace hana = boost::hana;

  /**
   * @brief Constexpr function which produces a tuple that contains all unique
   * ordered sublists of the range from 0 to W.
   * This will generate:
   * | W  | output                                             |
   * |:---|:---------------------------------------------------|
   * | 1  | [(0)]                                              |
   * | 2  | [(0), (1), (0, 1)]                                 |
   * | 3  | [(0), (1), (0, 1), (2), (0, 2), (1, 2), (0, 1, 2)] |
   *
   * In python, this algorithm would be roughly equivalent to
   * ```python
   * comb = [()]
   * for i in range(0, 6):
   *     comb = comb + [c + (i,) for c in comb]
   * comb = comb[1:]
   * ```
   * or using `foldl = lambda func, acc, xs: functools.reduce(func, xs, acc)`:
   * ```python
   * def unique_ordered_subset(n):
   *   comb = [()]
   *   comb = foldl(lambda c, i: c+[c_i+(i,) for c_i in c], comb, range(0, n))
   *   return comb[1:]
   * ```
   *
   * @tparam W End of the range (exclusive)
   */
  template <size_t W>
  constexpr auto
  unique_ordered_sublists()
  {
    using namespace hana::literals;
    // generate an empty tuple to start off
    constexpr auto combinations = hana::make_tuple(hana::make_tuple());
    // generate range over which to fold
    constexpr auto w_range
        = hana::to_tuple(hana::make_range(0_c, hana::size_c<W>));
    // main fold expression. This successively adds variations to `state` and
    // then
    // returns the result.
    constexpr auto comb2
        = hana::fold_left(w_range, combinations, [](auto state, auto i) {
            auto mapped = hana::transform(
                state, [i](auto c_i) { return hana::append(c_i, i); });
            return hana::concat(state, mapped);
          });
    // we need to pop off the initial empty tuple, which isn't really valid
    return hana::remove_at(comb2, 0_c);
  }

  /**
   * Generates a tuple of types using `unique_ordered_sublists` from above.
   * @tparam meas_meta Metafunction which will create a type given a parameter
   *                   list
   * @tparam W The size of the parameter pack to generate the sublists over.
   */
  template <template <ParID_t...> class meas_meta, size_t W>
  constexpr auto
  type_generator()
  {
    // generate sublists
    constexpr auto sublists = unique_ordered_sublists<W>();
    // map each sublist (tuple of paramater indices) into a measurement using
    // the provided `meas_meta` metafunction.
    constexpr auto measurements_h = hana::transform(sublists, [](auto s) {
      return hana::unpack(s, [](auto... i) {
        return hana::type_c<
            typename meas_meta<ParID_t(decltype(i)::value)...>::type>;
      });
    });
    // return tuple of measurements
    return measurements_h;
  }

  /**
   * Type alias which unpacks the hana tuple generated in `type_generator`
   * into an `std::variant`.
   * @tparam meas_meta Factory meta function for measurements.
   * @tparam W max number of parameters.
   */
  template <template <ParID_t...> class meas_meta, size_t W>
  using type_generator_t =
      typename decltype(hana::unpack(type_generator<meas_meta, W>(),
                                     hana::template_<std::variant>))::type;

  /// @endcond
}  // namespace detail
/// @endcond
}  // namespace Acts
