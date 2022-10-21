#include <iostream>
#include <type_traits>

#include "test_util.h"
#include "select_default.h"

using namespace select_n;
using test_n::Tracer;
using entry_t = test_n::test_map_entry;

namespace
{
}

int main()
{
  auto map{ test_n::make_test_map() };
  auto def{ test_n::make_test_tracer() };

  {
    auto existing{ select_default( map, entry_t::EXISTING, def ) };
    std::cout << Tracer::log() << std::endl;
  }
  Tracer::clear_log();
  {
    auto existing{ select_default( std::move( map ), entry_t::EXISTING, def ) };
    std::cout << Tracer::log() << std::endl;
  }
  Tracer::clear_log();
  {
    auto existing{ select_default( map, entry_t::MISSING, def ) };
    std::cout << Tracer::log() << std::endl;
  }
  Tracer::clear_log();
  {
    auto existing{ select_default( map, entry_t::MISSING, std::move( def ) ) };
    std::cout << Tracer::log() << std::endl;
  }
}