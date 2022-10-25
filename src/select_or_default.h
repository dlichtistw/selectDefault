// Copyright David Lichti 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "select.h"
#include "select_util.h"

namespace select_n
{
  namespace detail_n
  {
    //! Meta programming helper to deduce a suitable return type for \c select_or_default .
    template< typename CONTAINER, typename KEY, typename DEFAULT >
    struct find_or_default_result_c
    {
      //! Value type (i.e. ignoring reference) if taken from the container.
      using found_value = std::remove_reference_t< decltype( *select( std::declval< CONTAINER & >(), std::declval< KEY >() ) ) >;

      //! Value type (i.e. ignoring reference) if falling back to the default.
      using default_value = std::remove_reference_t< DEFAULT >;

      //! If any of the sources is const, the result has to be const, too.
      static constexpr bool return_const{ std::is_const_v< found_value > || std::is_const_v< default_value > };

      //! Deduced return value type for \c select_or_default . It already has the suitable const qualifier, but a reference may still be added.
      using return_value = conditional_const_t< return_const, found_value >;

      //! Check that a default value reference can be converted into a return type reference.
      static constexpr bool reference_convertible{ std::is_convertible_v< default_value &, return_value & > };

      //! If both sources are lvalue references, and conversion is possible, the result can be returned by lvalue reference, too.
      static constexpr bool return_by_reference{ std::is_lvalue_reference_v< CONTAINER > && std::is_lvalue_reference_v< DEFAULT > && reference_convertible };

      //! Check that a default value can be conveted into a return type.
      static constexpr bool value_convertible{ std::is_convertible_v< default_value, return_value > };

      // Friendly static assert to diagnose template failures.
      static_assert( value_convertible || return_by_reference, "Cannot use DEFAULT for selecting from CONTAINER with KEY, since it is not convertible." );

      //! Final return type, deduced according to the rules above.
      using type = conditional_reference_t< return_by_reference, return_value >;
    };

    //! Deduce a suitable return type for \c select_or_default : reference if possible, const if necessary.
    template< typename CONTAINER, typename KEY, typename DEFAULT >
    using find_or_default_result_t = typename find_or_default_result_c< CONTAINER, KEY, DEFAULT >::type;
  }

  /*!
    \brief Select an entry from a container, or return a default value.

    If an entry can be selected for \c key from \c container , it is returned. Otherwise, \c def is used as a default.
    The result is returned as reference if both \c container and \c def are references, adding a const if any of \c container or \c def is const.
    If \c container or \c def are moved in (i.e. passed as rvalue), and the result is taken from the rvalue input, then the result is moved out.
  */
  template< typename CONTAINER, typename KEY, typename DEFAULT >
  detail_n::find_or_default_result_t< CONTAINER, KEY, DEFAULT >
  select_or_default( CONTAINER &&container, KEY &&key, DEFAULT &&def )
  {
    if ( auto existing{ select( container, std::forward< KEY >( key ) ) } )
    {
      // Key exists -> return value.
      if constexpr ( std::is_lvalue_reference_v< CONTAINER > )
        // Persistent input -> copy or reference entry.
        return *existing;
      else
        // Temporary input -> move entry.
        return std::move( *existing );
    }
    else
    {
      // Key missing -> return the default value.
      return std::forward< DEFAULT >( def );
    }
  }

  /*!
    \brief Select an entry from a container, or return a defaul value.

    This is a convenience variant that does not need an explicit default value.
    Instead, a static default value is provided.

    \note Note that the default value is provided as const reference.
          Hence, the result is always returned as a const reference if \c container is passed by reference.
  */
  template< typename CONTAINER, typename KEY >
  decltype( auto ) select_or_default( CONTAINER &&container, KEY &&key )
  {
    using default_t = decltype( *select( container, key ) );
    return select_or_default( std::forward< CONTAINER >( container ), std::forward< KEY >( key ), detail_n::static_default< default_t >() );
  }
}
