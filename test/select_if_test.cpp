#include "select_if.h"
#include "test_util.h"

#include <cassert>
#include <vector>
#include <type_traits>

using namespace select_n;

namespace
{
  using entry_t = test_n::test_map_entry;
  using test_n::Tracer;

  std::vector< Tracer > make_test_vector()
  {
    Tracer::Silencer silencer;
    return { {} };
  }

  template< bool b >
  struct constant_function
  {
    template< typename... T >
    bool operator()( T &&... ) noexcept { return b; }
  };

  void testSelectFromConstantVector()
  {
    const auto vec{ make_test_vector() };
    Tracer::clear_log();

    auto existing{ select_if( vec, constant_function< true >{} ) };
    auto missing{ select_if( vec, constant_function< false >{} ) };

    assert( ( std::is_same_v< decltype( existing ), const Tracer * > ) );
    assert( existing == &vec.front() );
    assert( missing == nullptr );
    assert( Tracer::log().empty() );
  }

  void testSelectFromMutableVector()
  {
    auto vec{ make_test_vector() };
    Tracer::clear_log();

    auto existing{ select_if( vec, constant_function< true >{} ) };
    auto missing{ select_if( vec, constant_function< false >{} ) };

    assert( ( std::is_same_v< decltype( existing ), Tracer * > ) );
    assert( existing == &vec.front() );
    assert( missing == nullptr );
    assert( Tracer::log().empty() );
  }
}

int main()
{
  testSelectFromConstantVector();
  testSelectFromMutableVector();
}