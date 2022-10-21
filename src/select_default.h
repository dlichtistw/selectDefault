#pragma once

#include <type_traits>
#include <utility>

// Copyright David Lichti 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

namespace select_n
{
  namespace detail_n
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

    //! Deduce the value type of selecting from a \c MAP type using the \c KEY type.
    template< typename MAP, typename KEY >
    using select_map_value_t = decltype( std::declval< MAP >().find( std::declval< KEY >() )->second );

    //! Define a suitable return type for \c select(map,key) . It is a pointer, const if necessary.
    template< typename MAP, typename KEY >
    using select_map_t = copy_const_t< MAP, select_map_value_t< MAP, KEY > > *;
  }

  /*!
    \brief Select an entry from a map.

    If \c map has an entry associated to \c key , it is returned by pointer.
    If \c key is missing from \c map, a \c nullptr is returned, instead.
    The pointer is const if \c map is const. Otherwise, it is a non-const pointer.
  */
  template< typename MAP, typename KEY >
  detail_n::select_map_t< MAP, KEY >
  select( MAP &map, KEY &&key )
  {
    if ( const auto entry{ map.find( std::forward< KEY >( key ) ) }; entry != map.end() )
      // Key exists -> return pointer to mapped value.
      return &entry->second;
    else
      // Key missing -> return nullptr.
      return nullptr;
  }

  namespace detail_n
  {
    //! Define a suitable return type for \c select_default(map,key,def) . It is a reference if possible and const if necessary.
    template< typename MAP, typename KEY, typename DEFAULT = const select_map_value_t< MAP, KEY > & >
    using select_map_default_t = std::conditional_t< std::is_lvalue_reference_v< MAP > && std::is_lvalue_reference_v< DEFAULT >,
                                                     conditional_const_t< is_consty_v< MAP > || is_consty_v< DEFAULT >, select_map_value_t< MAP, KEY> > &,
                                                     select_map_value_t< MAP, KEY > >;

    //! Provide a single, static default value for \c select_default(map,key,def) .
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
  }

  /*!
    \brief Select an entry from a map, or return a default value.

    If \c map returns an entry for \c key , it is returned. Otherwise, \c def is returned as default.
    The result is returned as reference if both \c map and \c def are references, adding a const if any of \c map or \c def is const.
    If \c map or \c def are moved in (i.e. passed as rvalue), and the result is taken from the rvalue input, then the result is moved out.
  */
  template< typename DEFAULT, typename MAP, typename KEY >
  detail_n::select_map_default_t< MAP, KEY, DEFAULT >
  select_default( MAP &&map, KEY &&key, DEFAULT &&def )
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
  decltype( auto ) select_default( MAP &&map, KEY &&key )
  {
    // Add the static default value and forward the arguments.
    return select_default( std::forward< MAP >( map ), std::forward< KEY >( key ), detail_n::static_default< detail_n::select_map_value_t< MAP, KEY > >() );
  }
}
