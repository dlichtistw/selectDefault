// Copyright David Lichti 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <type_traits>

#include "select.h"
#include "select_util.h"

namespace select_n
{
  namespace detail_n
  {
    //! Meta programming helper to deduce a suitable return type for \c select_or_default .
    template< typename MAP, typename KEY, typename DEFAULT >
    struct find_mapped_value_or_default_c
    {
      //! Value type if taken from the map.
      using mapped_value = find_mapped_value_t< MAP, KEY >;

      //! Value type if falling back to the default.
      using default_value = std::remove_reference_t< DEFAULT >;

      //! If both sources are lvalue references, the result can be returned by lvalue reference, too.
      static constexpr bool return_by_reference{ std::is_lvalue_reference_v< MAP > && std::is_lvalue_reference_v< DEFAULT > };

      //! If any of the sources is const, the result has to be const, too.
      static constexpr bool return_const{ std::is_const_v< mapped_value > || std::is_const_v< default_value > };

      //! Final return type, deduced according to the rules above.
      using type = std::conditional_t< return_by_reference, conditional_const_t< return_const, mapped_value > &, mapped_value >;
    };

    //! Deduce a suitable return type for \c select_or_default : reference if possible, const if necessary.
    template< typename MAP, typename KEY, typename DEFAULT >
    using find_mapped_value_or_default_t = typename find_mapped_value_or_default_c< MAP, KEY, DEFAULT >::type;
  }

  /*!
    \brief Select an entry from a map, or return a default value.

    If \c map returns an entry for \c key , it is returned. Otherwise, \c def is returned as default.
    The result is returned as reference if both \c map and \c def are references, adding a const if any of \c map or \c def is const.
    If \c map or \c def are moved in (i.e. passed as rvalue), and the result is taken from the rvalue input, then the result is moved out.
  */
  template< typename DEFAULT, typename MAP, typename KEY >
  detail_n::find_mapped_value_or_default_t< MAP, KEY, DEFAULT >
  select_or_default( MAP &&map, KEY &&key, DEFAULT &&def )
  {
    if ( auto existing{ select( map, std::forward< KEY >( key ) ) } )
    {
      // Key exists -> return value.
      if constexpr ( std::is_same_v< MAP, std::remove_reference_t< MAP > > )
        // Temporary map -> move entry.
        return std::move( *existing );
      else
        // Persistent map -> copy or reference entry.
        return *existing;
    }
    else
      // Key missing -> return the default value.
      return std::forward< DEFAULT >( def );
  }

  /*!
    \brief Select an entry from a map, or return a defaul value.

    This is a convenience variant that does not need an explicit default value.
    Instead, a static default value is provided.

    \note Note that the default value is provided as const reference.
          Hence, the result is always returned as a const reference if \c map is passed by reference.
  */
  template< typename MAP, typename KEY >
  decltype( auto ) select_or_default( MAP &&map, KEY &&key )
  {
    // Add the static default value and forward the arguments.
    return select_or_default( std::forward< MAP >( map ), std::forward< KEY >( key ), detail_n::static_default< detail_n::find_mapped_value_t< MAP, KEY > >() );
  }
}
