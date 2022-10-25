// Copyright David Lichti 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <algorithm>
#include <utility>

#include "select_util.h"

namespace select_n
{
  namespace detail_n
  {
    /*!
      \brief Concept of Map Like Types.

      A type \c MAP is like a map over another type \c KEY , if it provides a member function like \c std::map<KEY,T>::find .
      Its return type's values \c it must point to a type \c T with a member \c T::second , just like the standard map's iterators.
    */
    template< typename MAP, typename KEY >
    concept MapLike = requires( MAP map, KEY key ){ { map.find( key )->second }; };

    /*!
      \brief Concept of Set Like Types.

      A type \c SET is like a set of another type \c KEY , if it provides a member function like \c std::set<KEY>::find .
      Its return type's values \c it must support dereferencing, just like the standard set's iterators.
    */
    template< typename SET, typename KEY >
    concept SetLike = requires( SET set, KEY key ){ { *set.find( key ) }; };

    /*!
      \brief Concept of General Searchable Containers.

      A type \c CONTAINER is searchable for another type \c VALUE , if \c std::find can be called on it.
    */
    template< typename CONTAINER, typename VALUE >
    concept Searchable = requires( CONTAINER container, VALUE value ){ { *std::find( container.begin(), container.end(), value ) }; };
  }

  /*!
    \brief Select an entry from a map.

    If \c map has an entry associated to \c key , it is returned by pointer.
    If \c key is missing from \c map, a \c nullptr is returned, instead.
  */
  template< typename MAP, typename KEY > requires detail_n::MapLike< MAP, KEY >
  auto select( MAP &map, KEY &&key ) -> decltype( &map.find( key )->second )
  {
    if ( const auto entry{ map.find( std::forward< KEY >( key ) ) }; entry != map.end() )
      // Key exists -> return pointer to mapped value.
      return &entry->second;
    else
      // Key missing -> return nullptr.
      return nullptr;
  }

  /*!
    \brief Select an entry from a set.

    If \c set has an entry similar to \c key , it is returned by pointer.
    If \c key is missing from \c set, a \c nullptr is returned, instead.
  */
  template< typename SET, typename KEY > requires ( detail_n::SetLike< SET, KEY > && !detail_n::MapLike< SET, KEY > )
  auto select( SET &set, KEY &&key ) -> decltype( &*set.find( key) )
  {
    if ( const auto entry{ set.find( std::forward< KEY >( key ) ) }; entry != set.end() )
      // Key exists -> return pointer to value.
      return &*entry;
    else
      // Key missing -> return nullptr.
      return nullptr;
  }

  /*!
    \brief Select an entry from a generic container.

    If \c container has an entry similar to \c value , it is returned by pointer.
    If \c value is missing from \c container, a \c nullptr is returned, instead.
  */
  template< typename CONTAINER, typename VALUE > requires ( detail_n::Searchable< CONTAINER, VALUE > && !detail_n::SetLike< CONTAINER, VALUE > )
  auto select( CONTAINER &container, VALUE &&value ) -> decltype( &*std::find( container.begin(), container.end(), value ) )
  {
    if ( const auto entry{ std::find( container.begin(), container.end(), std::forward< VALUE >( value ) ) }; entry != container.end() )
      // Value exists -> return by pointer.
      return &*entry;
    else
      // Value missing -> return nullptr.
      return nullptr;
  }
}
