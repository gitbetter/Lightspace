#include "catch.hpp"
#include "ray.hpp"
#include "transform.hpp"

using namespace ls;

TEST_CASE( "Ray processing", "[rays]" )
{
    SECTION( "Creating and querying a ray" )
    {
        auto o = f_point( 1, 2, 3 );
        auto d = f_vector( 4, 5, 6 );
        auto r = ray( o, d );

        REQUIRE( r.origin() == o );
        REQUIRE( r.direction() == d );
    }

    SECTION( "Computing a point from a distance" )
    {
        auto r = ray( f_point( 2, 3, 4 ), f_vector( 1, 0, 0 ) );

        REQUIRE( r.position( 0.f ) == f_point( 2, 3, 4 ) );
        REQUIRE( r.position( 1.f ) == f_point( 3, 3, 4 ) );
        REQUIRE( r.position( -1.f ) == f_point( 1, 3, 4 ) );
        REQUIRE( r.position( 2.5f ) == f_point( 4.5f, 3, 4 ) );
    }

    SECTION( "Translating a ray" )
    {
        auto r = ray( f_point( 1, 2, 3 ), f_vector( 0, 1, 0 ) );
        auto m = transform::translation( 3.f, 4.f, 5.f );

        auto r2 = m * r;

        REQUIRE( r2.origin() == f_point( 4, 6, 8 ) );
        REQUIRE( r2.direction() == f_vector( 0, 1, 0 ) );
    }

    SECTION( "Scaling a ray" )
    {
        auto r = ray( f_point( 1, 2, 3 ), f_vector( 0, 1, 0 ) );
        auto m = transform::scale( 2.f, 3.f, 4.f );

        auto r2 = m * r;

        REQUIRE( r2.origin() == f_point( 2, 6, 12 ) );
        REQUIRE( r2.direction() == f_vector( 0, 3, 0 ) );
    }
};