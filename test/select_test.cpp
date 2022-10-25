#include <cassert>

#include "select.h"
#include "select_util.h"
#include "test_util.h"

using namespace select_n;

namespace
{
  using entry_t = test_n::test_map_entry;

  void testSelectFromConstantMap()
  {
    const auto map{ test_n::make_test_map() };
    test_n::Tracer::clear_log();

    auto existing{ select( map, entry_t::EXISTING ) };
    auto missing{ select( map, entry_t::MISSING ) };

    assert( ( std::is_same_v< decltype( existing ), const test_n::Tracer * > ) );
    assert( existing == &map.at( entry_t::EXISTING ) );
    assert( missing == nullptr );
    assert( test_n::Tracer::log().empty() );
  }

  void testSelectFromMutableMap()
  {
    auto map{ test_n::make_test_map() };
    test_n::Tracer::clear_log();

    auto existing{ select( map, entry_t::EXISTING ) };
    auto missing{ select( map, entry_t::MISSING ) };

    assert( ( std::is_same_v< decltype( existing ), test_n::Tracer * > ) );
    assert( existing == &map.at( entry_t::EXISTING ) );
    assert( missing == nullptr );
    assert( test_n::Tracer::log().empty() );
  }

  void testSelectFromConstantSet()
  {
    const auto set{ test_n::make_test_set() };
    const auto &val{ *set.begin() };
    const auto &def{ detail_n::static_default< test_n::Tracer >() };
    test_n::Tracer::clear_log();

    auto existing{ select( set, val ) };
    auto missing{ select( set, def ) };

    assert( ( std::is_same_v< decltype( existing ), const test_n::Tracer * > ) );
    assert( existing == &val );
    assert( missing == nullptr );
    assert( test_n::Tracer::log().empty() );
  }

  void testSelectFromMutableSet()
  {
    auto set{ test_n::make_test_set() };
    const auto &val{ *set.begin() };
    const auto &def{ detail_n::static_default< test_n::Tracer >() };
    test_n::Tracer::clear_log();

    auto existing{ select( set, val ) };
    auto missing{ select( set, def ) };

    assert( ( std::is_same_v< decltype( existing ), const test_n::Tracer * > ) );
    assert( existing == &val );
    assert( missing == nullptr );
    assert( test_n::Tracer::log().empty() );
  }

  void testSelectFromConstantVector()
  {
    const auto vec{ test_n::make_test_vector() };
    const auto &val{ vec.front() };
    const auto &def{ detail_n::static_default< test_n::Tracer >() };
    test_n::Tracer::clear_log();

    auto existing{ select( vec, val ) };
    auto missing{ select( vec, def ) };

    assert( ( std::is_same_v< decltype( existing ), const test_n::Tracer * > ) );
    assert( existing == &val );
    assert( missing == nullptr );
    assert( test_n::Tracer::log().empty() );
  }

  void testSelectFromMutableVector()
  {
    auto vec{ test_n::make_test_vector() };
    const auto &val{ vec.front() };
    const auto &def{ detail_n::static_default< test_n::Tracer >() };
    test_n::Tracer::clear_log();

    auto existing{ select( vec, val ) };
    auto missing{ select( vec, def ) };

    assert( ( std::is_same_v< decltype( existing ), test_n::Tracer * > ) );
    assert( existing == &val );
    assert( missing == nullptr );
    assert( test_n::Tracer::log().empty() );
  }
}

int main()
{
  testSelectFromConstantMap();
  testSelectFromMutableMap();

  testSelectFromConstantSet();
  testSelectFromMutableSet();

  testSelectFromConstantVector();
  testSelectFromMutableVector();
}
