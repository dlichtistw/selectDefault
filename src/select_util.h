// Copyright David Lichti 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <utility>

namespace select_n::detail_n
{
  //! Provide a single, static default constructed value.
  template< typename T >
  const T &static_default()
  {
    if constexpr ( !std::is_same_v< T, std::decay_t< T > > )
      // Do not distinguish on type modifiers.
      return static_default< std::decay_t< T > >();
    else
    {
      // The one and only default value of type T.
      static const T def{};
      return def;
    }
  }

  //! Make a const or non-const version of \c T depending on \c condition .
  template< bool condition, typename T >
  using conditional_const_t = std::conditional_t< condition, std::add_const_t< T >, std::remove_const_t< T > >;
}
