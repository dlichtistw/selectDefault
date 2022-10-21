#pragma oce

#include "select_util.h"

// Copyright David Lichti 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

namespace select_n
{
  namespace detail_n
  {
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
}
