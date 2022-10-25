#pragma once

#include <compare>
#include <map>
#include <ostream>
#include <set>
#include <vector>

namespace select_n::test_n
{
  class Tracer
  {
  public:
    Tracer() noexcept;
    Tracer( const Tracer &tracer ) noexcept;
    Tracer( Tracer &&tracer ) noexcept;

    Tracer &operator=( const Tracer &tracer ) noexcept;
    Tracer &operator=( Tracer &&tracer ) noexcept;

    ~Tracer() noexcept;

    friend std::strong_ordering operator<=>( const Tracer &a, const Tracer &b ) noexcept;

    friend bool operator==( const Tracer &, const Tracer & ) noexcept;
    friend bool operator!=( const Tracer &, const Tracer & ) noexcept;

    friend bool operator<( const Tracer &, const Tracer & ) noexcept;
    friend bool operator>( const Tracer &, const Tracer & ) noexcept;

    friend std::ostream &operator<<( std::ostream &stream, const Tracer &tracer );

    struct Silencer
    {
      Silencer() noexcept;
      ~Silencer() noexcept;
    };

    enum class operation
    {
      DEFAULT_CONSTRUCTION,
      COPY_CONSTRUCTION,
      MOVE_CONSTRUCTION,
      COPY_ASSIGNMENT,
      MOVE_ASSIGNMENT,
      DESTRUCTION,
    };

    using id_t = std::size_t;
    using log_entry_t = std::pair< operation, id_t >;
    using log_t = std::vector< log_entry_t >;
    static const log_t &log() noexcept;
    static const log_t::value_type &log( const log_t::size_type index );
    static void clear_log() noexcept;

    const id_t &id() const noexcept;

  private:
    Tracer( const id_t id ) noexcept;

    static bool record_log;
    void record( const operation op ) const noexcept;

    static id_t counter;
    id_t _id;

    static log_t _log;
  };

  enum class test_map_entry
  {
    EXISTING,
    MISSING,
  };

  Tracer make_test_tracer() noexcept;

  using testMap_t = std::map< test_map_entry, Tracer >;
  testMap_t make_test_map() noexcept;

  using testSet_t = std::set< Tracer >;
  testSet_t make_test_set() noexcept;

  using testVec_t = std::vector< Tracer >;
  testVec_t make_test_vector() noexcept;

  std::ostream &operator<<( std::ostream &stream, const Tracer::operation operation );

  template< typename T, typename U >
  std::ostream &operator<<( std::ostream &stream, const std::pair< T, U > &pair )
  {
    return stream << "{ " << pair.first << ", " << pair.second << " }";
  }

  template< typename T >
  std::ostream &operator<<( std::ostream &stream, const std::vector< T > &vector )
  {
    if ( vector.empty() )
      return stream << "{}";
    else
    {
      stream << "{ " << vector.front();
      for ( auto it{ std::next( vector.begin() ) }, end{ vector.end() }; it < end; ++it )
        stream << ", " << *it;
      return stream << " }";
    }
  }
}
