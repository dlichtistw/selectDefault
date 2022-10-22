#include <iostream>
#include <type_traits>

#include "test_util.h"
#include "select_or_default.h"

using namespace select_n;
using test_n::Tracer;
using entry_t = test_n::test_map_entry;

namespace
{
}

int main()
{
  std::cout << std::boolalpha;

  const auto &map{ test_n::make_test_map() };

  select_or_default( map, entry_t::EXISTING );
}