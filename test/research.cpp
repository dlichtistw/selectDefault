#include <iostream>
#include <type_traits>

#include "test_util.h"
#include "select.h"

using namespace select_n;
using test_n::Tracer;
using entry_t = test_n::test_map_entry;

namespace
{
}

int main()
{
  std::cout << std::boolalpha;

  const std::map< int, char > map{ { 1, 'a' } };

  std::cout << std::is_same_v< decltype( ( map.find( 1 )->second ) ), const char & > << std::endl;
}