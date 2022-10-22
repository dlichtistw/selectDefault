#include <cassert>

#include "select_if.h"
#include "test_util.h"

using namespace select_n;

namespace
{
  using test_n::Tracer;

  template< auto c >
  struct constant_function
  {
    template< typename... T >
    auto operator()( T &&... ) const noexcept { return c; }
  };

  void testSelectFromConstantVector()
  {
    const auto vec{ test_n::make_test_vector() };
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
    auto vec{ test_n::make_test_vector() };
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