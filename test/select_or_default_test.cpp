#include "select_or_default.h"
#include "test_util.h"

#include <cassert>
#include <iostream>
#include <utility>

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

    auto &existing{ select_or_default( map, entry_t::EXISTING ) };
    auto &missing{ select_or_default( map, entry_t::MISSING ) };

    std::cout << Tracer::log() << std::endl;

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

    auto &existing{ select_or_default( map, entry_t::EXISTING ) };
    auto &missing{ select_or_default( map, entry_t::MISSING ) };

    std::cout << Tracer::log() << std::endl;

    assert( ( std::is_same_v< decltype( existing ), const Tracer & > ) );
    assert( &existing == &map.at( entry_t::EXISTING ) );
    assert( &missing == &def );
    assert( Tracer::log().empty() );
  }

  void testSelectImplicitDefaultFromTemporaryMap()
  {
    auto map{ test_n::make_test_map() };
    Tracer::clear_log();

    auto existing{ select_or_default( std::move( map ), entry_t::EXISTING ) };

    std::cout << Tracer::log() << std::endl;

    assert( ( std::is_same_v< decltype( existing ), Tracer > ) );
    assert( existing == map.at( entry_t::EXISTING ) );
    assert( ( Tracer::log() == std::vector{ Tracer::log_entry_t{ Tracer::operation::MOVE_CONSTRUCTION, existing.id() } } ) );
  }

  void testSelectExplicitDefaultFromConstantMap()
  {
    const auto map{ test_n::make_test_map() };
    const auto def{ detail_n::static_default< Tracer >() };
    Tracer::clear_log();

    auto &existing{ select_n::select_or_default( map, entry_t::EXISTING, def ) };
    auto &missing{ select_n::select_or_default( map, entry_t::MISSING, def ) };

    std::cout << Tracer::log() << std::endl;

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

    auto &existing{ select_n::select_or_default( map, entry_t::EXISTING, def ) };
    auto &missing{ select_n::select_or_default( map, entry_t::MISSING, def ) };

    std::cout << Tracer::log() << std::endl;

    assert( ( std::is_same_v< decltype( existing ), Tracer & > ) );
    assert( &existing == &map.at( entry_t::EXISTING ) );
    assert( &missing == &def );
    assert( Tracer::log().empty() );
  }

  void testSelectExplicitDefaultFromTemporaryMap()
  {
    auto map{ test_n::make_test_map() };
    auto def{ test_n::make_test_tracer() };
    Tracer::clear_log();

    auto existing{ select_or_default( std::move( map ), entry_t::EXISTING, def ) };

    std::cout << Tracer::log() << std::endl;

    assert( ( std::is_same_v< decltype( existing ), Tracer > ) );
    assert( existing == map.at( entry_t::EXISTING ) );
    assert( ( Tracer::log() == std::vector{ Tracer::log_entry_t{ Tracer::operation::MOVE_CONSTRUCTION, existing.id() } } ) );
  }

  void testSelectExplicitDefaultFromTemporaryDefault()
  {
    auto map{ test_n::make_test_map() };
    auto def{ test_n::make_test_tracer() };
    Tracer::clear_log();

    auto missing{ select_or_default( map, entry_t::MISSING, std::move( def ) ) };

    std::cout << Tracer::log() << std::endl;

    assert( ( std::is_same_v< decltype( missing ), Tracer > ) );
    assert( missing == def );
    assert( ( Tracer::log() == std::vector{ Tracer::log_entry_t{ Tracer::operation::MOVE_CONSTRUCTION, def.id() } } ) );
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
  testSelectExplicitDefaultFromTemporaryDefault();
}
