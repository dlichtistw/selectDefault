#pragma once

#include <algorithm>
#include <type_traits>
#include <utility>

#include "select_util.h"

// Copyright David Lichti 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

namespace select_n
{
  namespace detail_n
  {
    //! Deduce the value type of selecting from a \c CONTAINER type using the \c PREDICATE function.
    template< typename CONTAINER, typename PREDICATE >
    using select_if_value_t = std::remove_reference_t< decltype( *std::find_if( std::declval< CONTAINER >().begin(), std::declval< CONTAINER >().end(), std::declval< PREDICATE >() ) ) >;

    //! Define a suitable return type for \c select_if(map,predicate) . It is a pointer, const if necessary.
    template< typename CONTAINER, typename PREDICATE >
    using select_if_t = copy_const_t< CONTAINER, select_if_value_t< CONTAINER, PREDICATE > > *;
  }

  /*!
    \brief Select an element from a container.

    If \c std::find_if finds an element in \c container using \c predicate , it is returned by pointer.
    Otherwise, a \c nullptr is returned.
    The pointer is const if necessary.
  */
  template< typename CONTAINER, typename PREDICATE >
  detail_n::select_if_t< CONTAINER, PREDICATE >
  select_if( CONTAINER &container, PREDICATE &&predicate )
  {
    if ( const auto entry{ std::find_if( container.begin(), container.end(), std::forward< PREDICATE >( predicate ) ) }; entry != container.end() )
      // Element found -> return by pointer.
      return &*entry;
    else
      // Nothing found -> return nullptr.
      return nullptr;
  }
}
