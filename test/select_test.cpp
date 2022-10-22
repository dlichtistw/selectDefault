#include <cassert>

#include "select.h"
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
}

int main()
{
  testSelectFromConstantMap();
  testSelectFromMutableMap();
}
