#pragma once

#include <type_traits>
#include <utility>

namespace select_n
{
  namespace detail_n
  {
    template< typename T >
    constexpr bool is_consty_v{ std::is_const_v< std::remove_reference_t< T > > };

    template< bool cond, typename T >
    using conditional_const_t = std::conditional_t< cond, std::add_const_t< T >, std::remove_const_t< T > >;

    template< typename S, typename T >
    using copy_const_t = conditional_const_t< is_consty_v< S >, T >;

    template< typename MAP, typename KEY >
    using select_map_value_t = decltype( std::declval< MAP >().find( std::declval< KEY >() )->second );

    template< typename MAP, typename KEY >
    using select_map_t = copy_const_t< MAP, select_map_value_t< MAP, KEY > > *;
  }

  template< typename MAP, typename KEY >
  detail_n::select_map_t< MAP, KEY >
  select( MAP &map, KEY &&key )
  {
    if ( const auto entry{ map.find( std::forward< KEY >( key ) ) }; entry != map.end() )
      return &entry->second;
    else
      return nullptr;
  }

  namespace detail_n
  {
    template< typename MAP, typename KEY, typename DEFAULT = const select_map_value_t< MAP, KEY > & >
    using select_map_default_t = std::conditional_t< std::is_lvalue_reference_v< MAP > && std::is_lvalue_reference_v< DEFAULT >,
                                                     conditional_const_t< is_consty_v< MAP > || is_consty_v< DEFAULT >, select_map_value_t< MAP, KEY> > &,
                                                     select_map_value_t< MAP, KEY > >;

    template< typename T >
    const T &static_default()
    {
      if constexpr ( !std::is_same_v< T, std::decay_t< T > > )
        return static_default< std::decay_t< T > >();
      else
      {
        static const T def{};
        return def;
      }
    }
  }

  template< typename DEFAULT, typename MAP, typename KEY >
  detail_n::select_map_default_t< MAP, KEY, DEFAULT >
  select_default( MAP &&map, KEY &&key, DEFAULT &&def )
  {
    if ( auto existing{ select( map, std::forward< KEY >( key ) ) } )
      return *existing;
    else
      return std::forward< DEFAULT >( def );
  }

  template< typename MAP, typename KEY >
  decltype( auto ) select_default( MAP &&map, KEY &&key )
  {
    return select_default( std::forward< MAP >( map ), std::forward< KEY >( key ), detail_n::static_default< detail_n::select_map_value_t< MAP, KEY > >() );
  }
}
