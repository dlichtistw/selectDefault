#pragma once

#include <utility>

// Copyright David Lichti 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

namespace select_n::detail_n
{
  //! Detect const types, including const& types.
  template< typename T >
  constexpr bool is_consty_v{ std::is_const_v< std::remove_reference_t< T > > };

  //! Make a const or non-const version of \c T depending on \c condition .
  template< bool condition, typename T >
  using conditional_const_t = std::conditional_t< condition, std::add_const_t< T >, std::remove_const_t< T > >;

  //! Make a const or non-const version of \c T depending on the constness of \c S .
  template< typename S, typename T >
  using copy_const_t = conditional_const_t< is_consty_v< S >, T >;
}
