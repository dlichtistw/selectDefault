#include "test_util.h"
#include <compare>

using namespace select_n::test_n;

Tracer::Tracer( const id_t id ) noexcept : _id{ id } {}

Tracer::Tracer() noexcept : Tracer( counter++ )
{
  record( operation::DEFAULT_CONSTRUCTION );
}

Tracer::Tracer( const Tracer &tracer ) noexcept : Tracer( tracer._id )
{
  record( operation::COPY_CONSTRUCTION );
}

Tracer::Tracer( Tracer &&tracer ) noexcept : Tracer( tracer._id )
{
  record( operation::MOVE_CONSTRUCTION );
}

Tracer &Tracer::operator=( const Tracer &tracer ) noexcept
{
  _id = tracer._id;
  record( operation::COPY_ASSIGNMENT );
  return *this;
}

Tracer &Tracer::operator=( Tracer &&tracer ) noexcept
{
  _id = tracer._id;
  record( operation::MOVE_ASSIGNMENT );
  return *this;
}

Tracer::~Tracer() noexcept
{
  record( operation::DESTRUCTION );
}

std::strong_ordering select_n::test_n::operator<=>( const Tracer &a, const Tracer &b ) noexcept { return a._id <=> b._id; }

bool select_n::test_n::operator==( const Tracer &, const Tracer & ) noexcept = default;
bool select_n::test_n::operator!=( const Tracer &, const Tracer & ) noexcept = default;

bool select_n::test_n::operator<( const Tracer &, const Tracer & ) noexcept = default;
bool select_n::test_n::operator>( const Tracer &, const Tracer & ) noexcept = default;

std::ostream &select_n::test_n::operator<<( std::ostream &stream, const Tracer &tracer ) { return stream << "Tracer." << tracer._id; }

void Tracer::record( const operation op ) const noexcept
{
  if ( record_log )
    _log.emplace_back( op, _id );
}

const Tracer::log_t &Tracer::log() noexcept { return _log; }
const Tracer::log_t::value_type &Tracer::log( const Tracer::log_t::size_type index ) { return _log.at( index ); }

void Tracer::clear_log() noexcept { _log.clear(); }

const Tracer::id_t &Tracer::id() const noexcept { return _id; }

bool Tracer::record_log{ true };
Tracer::id_t Tracer::counter{ 0 };
Tracer::log_t Tracer::_log{};

Tracer::Silencer::Silencer() noexcept { record_log = false; }
Tracer::Silencer::~Silencer() noexcept { record_log = true; }

Tracer select_n::test_n::make_test_tracer() noexcept
{
  Tracer::Silencer silencer;
  return {};
}

testMap_t select_n::test_n::make_test_map() noexcept
{
  Tracer::Silencer silencer;
  return { { test_map_entry::EXISTING, {} } };
}

testSet_t select_n::test_n::make_test_set() noexcept
{
  Tracer::Silencer silenver;
  return { {} };
}

testVec_t select_n::test_n::make_test_vector() noexcept
{
  Tracer::Silencer silencer;
  return { {} };
}

std::ostream &
select_n::test_n::operator<<( std::ostream &stream, const Tracer::operation operation )
{
  switch ( operation )
  {
    case Tracer::operation::DEFAULT_CONSTRUCTION: return stream << "DEFAULT_CONSTRUCTION";
    case Tracer::operation::COPY_CONSTRUCTION: return stream << "COPY_CONSTRUCTION";
    case Tracer::operation::MOVE_CONSTRUCTION: return stream << "MOVE_CONSTRUCTION";
    case Tracer::operation::COPY_ASSIGNMENT: return stream << "COPY_ASSIGNMENT";
    case Tracer::operation::MOVE_ASSIGNMENT: return stream << "MOVE_ASSIGNMENT";
    case Tracer::operation::DESTRUCTION: return stream << "DESTRUCTION";
  }
}
