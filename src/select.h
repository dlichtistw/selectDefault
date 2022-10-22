#pragma oce

#include "select_util.h"

// Copyright David Lichti 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

namespace select_n
{
  namespace detail_n
  {
    //! Deduce the value type of \c MAP::find with \c KEY .
    template< typename MAP, typename KEY >
    using find_mapped_value_t = std::remove_reference_t< decltype( ( std::declval< MAP >().find( std::declval< KEY >() )->second ) ) >;
  }

  /*!
    \brief Select an entry from a map.

    If \c map has an entry associated to \c key , it is returned by pointer.
    If \c key is missing from \c map, a \c nullptr is returned, instead.
  */
  template< typename MAP, typename KEY >
  detail_n::find_mapped_value_t< MAP, KEY > *
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
