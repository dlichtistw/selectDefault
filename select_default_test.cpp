#include "select_default.h"
#include "test_util.h"

#include <cassert>
#include <iostream>

using namespace select_n;
using test_n::Tracer;

namespace
{
  using entry_t = test_n::test_map_entry;

  void testSelectImplicitDefaultFromConstantMap()
  {
    const auto map{ test_n::make_test_map() };
    const auto &def{ detail_n::static_default< Tracer >() };
    Tracer::clear_log();

    auto &existing{ select_default( map, entry_t::EXISTING ) };
    auto &missing{ select_default( map, entry_t::MISSING ) };

    assert( ( std::is_same_v< decltype( existing ), const Tracer & > ) );
    assert( &existing == &map.at( entry_t::EXISTING ) );
    assert( &missing == &def );
    assert( Tracer::log().empty() );
  }

  void testSelectImplicitDefaultFromMutableMap()
  {
    auto map{ test_n::make_test_map() };
    const auto &def{ detail_n::static_default< Tracer >() };
    Tracer::clear_log();

    auto &existing{ select_default( map, entry_t::EXISTING ) };
    auto &missing{ select_default( map, entry_t::MISSING ) };

    assert( ( std::is_same_v< decltype( existing ), const Tracer & > ) );
    assert( &existing == &map.at( entry_t::EXISTING ) );
    assert( &missing == &def );
    assert( Tracer::log().empty() );
  }

  void testSelectImplicitDefaultFromTemporaryMap()
  {
    const auto &def{ detail_n::static_default< Tracer >() };
    Tracer::clear_log();

    auto existing{ select_default( test_n::make_test_map(), entry_t::EXISTING ) };
    auto missing{ select_default( test_n::make_test_map(), entry_t::MISSING ) };

    assert( ( std::is_same_v< decltype( existing ), Tracer > ) );
    assert( missing == def );
    assert( Tracer::log().size() == 4 );
    assert( ( Tracer::log( 0 ) == Tracer::log_entry_t{ Tracer::operation::COPY_CONSTRUCTION, existing.id() } ) );
    assert( ( Tracer::log( 1 ) == Tracer::log_entry_t{ Tracer::operation::DESTRUCTION, existing.id() } ) );
    assert( ( Tracer::log( 2 ) == Tracer::log_entry_t{ Tracer::operation::COPY_CONSTRUCTION, def.id() } ) );
    assert( Tracer::log( 3 ).first == Tracer::operation::DESTRUCTION );
  }

  void testSelectExplicitDefaultFromConstantMap()
  {
    const auto map{ test_n::make_test_map() };
    const auto def{ detail_n::static_default< Tracer >() };
    Tracer::clear_log();

    auto &existing{ select_n::select_default( map, entry_t::EXISTING, def ) };
    auto &missing{ select_n::select_default( map, entry_t::MISSING, def ) };

    assert( ( std::is_same_v< decltype( existing ), const Tracer & > ) );
    assert( &existing == &map.at( entry_t::EXISTING ) );
    assert( &missing == &def );
    assert( Tracer::log().empty() );
  }

  void testSelectExplicitDefaultFromMutableMap()
  {
    auto map{ test_n::make_test_map() };
    auto def{ detail_n::static_default< Tracer >() };
    Tracer::clear_log();

    auto &existing{ select_n::select_default( map, entry_t::EXISTING, def ) };
    auto &missing{ select_n::select_default( map, entry_t::MISSING, def ) };

    assert( ( std::is_same_v< decltype( existing ), Tracer & > ) );
    assert( &existing == &map.at( entry_t::EXISTING ) );
    assert( &missing == &def );
    assert( Tracer::log().empty() );
  }

  void testSelectExplicitDefaultFromTemporaryMap()
  {
    const auto &def{ detail_n::static_default< Tracer >() };
    Tracer::clear_log();

    auto existing{ select_default( test_n::make_test_map(), entry_t::EXISTING, def ) };
    auto missing{ select_default( test_n::make_test_map(), entry_t::MISSING, def ) };

    assert( ( std::is_same_v< decltype( existing ), Tracer > ) );
    assert( missing == def );
    assert( Tracer::log().size() == 4 );
    assert( ( Tracer::log( 0 ) == Tracer::log_entry_t{ Tracer::operation::COPY_CONSTRUCTION, existing.id() } ) );
    assert( ( Tracer::log( 1 ) == Tracer::log_entry_t{ Tracer::operation::DESTRUCTION, existing.id() } ) );
    assert( ( Tracer::log( 2 ) == Tracer::log_entry_t{ Tracer::operation::COPY_CONSTRUCTION, def.id() } ) );
    assert( Tracer::log( 3 ).first == Tracer::operation::DESTRUCTION );
  }
}

int main()
{
  testSelectImplicitDefaultFromConstantMap();
  testSelectImplicitDefaultFromMutableMap();
  testSelectImplicitDefaultFromTemporaryMap();

  testSelectExplicitDefaultFromConstantMap();
  testSelectExplicitDefaultFromMutableMap();
  testSelectExplicitDefaultFromTemporaryMap();
}
