#include "catch.hpp"
#include "shapes.hpp"
#include "intersection.hpp"

using namespace ls;

TEST_CASE( "Intersection processing", "[intersections]" )
{
    SECTION( "An intersection encapsulates t and an object" )
    {
        auto s = sphere::create();
        auto i = intersection( 3.5f, s );

        REQUIRE( i.time() == 3.5f );
        REQUIRE( i.object() == s );
    }

    SECTION( "Aggregating intersections" )
    {
        auto s = sphere::create();
        auto i1 = intersection( 1, s );
        auto i2 = intersection( 2, s );
        auto xs = intersections{ i1, i2 };

        REQUIRE( xs.size() == 2 );
        REQUIRE( xs[0].time() == 1 );
        REQUIRE( xs[1].time() == 2 );
    }

    SECTION( "The hit, when all intersections have positive t" )
    {
        auto s = sphere::create();
        auto i1 = intersection( 1, s );
        auto i2 = intersection( 2, s );
        auto xs = intersections{ i2, i1 };

        REQUIRE( hit( xs ) == i1 );
    }

    SECTION( "The hit, when some intersections have negative t" )
    {
        auto s = sphere::create();
        auto i1 = intersection( -1, s );
        auto i2 = intersection( 1, s );
        auto xs = intersections{ i2, i1 };

        REQUIRE( hit( xs ) == i2 );
    }

    SECTION( "The hit, when all intersections have negative t" )
    {
        auto s = sphere::create();
        auto i1 = intersection( -2, s );
        auto i2 = intersection( -1, s );
        auto xs = intersections{ i2, i1 };

        REQUIRE( hit( xs ) == intersection::none );
    }

    SECTION( "The hit is always the lowest nonnegative intersection" )
    {
        auto s = sphere::create();
        auto i1 = intersection( 5, s );
        auto i2 = intersection( 7, s );
        auto i3 = intersection( -3, s );
        auto i4 = intersection( 2, s );
        auto xs = intersections{ i1, i2, i3, i4 };

        REQUIRE( hit( xs ) == i4 );
    }

    SECTION( "Precomputing the state of an intersection" )
    {
        auto r = ray( f_point( 0, 0, -5 ), f_vector( 0, 0, 1 ) );
        auto s = sphere::create();
        auto i = intersection( 4.f, s );
        auto istate = prepare_intersection_state( i, r );

        REQUIRE( istate.time == i.time() );
        REQUIRE( istate.object == i.object() );
        REQUIRE( istate.point == f_point( 0, 0, -1 ) );
        REQUIRE( istate.eye == f_vector( 0, 0, -1 ) );
        REQUIRE( istate.normal == f_vector( 0, 0, -1 ) );
    }

    SECTION( "The hit, when an intersection occurs on the outside" )
    {
        auto r = ray( f_point( 0, 0, -5 ), f_vector( 0, 0, 1 ) );
        auto s = sphere::create();
        auto i = intersection( 4.f, s );
        auto istate = prepare_intersection_state( i, r );

        REQUIRE( istate.inside == false );
    }

    SECTION( "The hit, when an intersection occurs on the inside" )
    {
        auto r = ray( f_point( 0, 0, 0 ), f_vector( 0, 0, 1 ) );
        auto s = sphere::create();
        auto i = intersection( 1.f, s );
        auto istate = prepare_intersection_state( i, r );

        REQUIRE( istate.point == f_point( 0, 0, 1 ) );
        REQUIRE( istate.eye == f_vector( 0, 0, -1 ) );
        REQUIRE( istate.normal == f_vector( 0, 0, -1 ) );
        REQUIRE( istate.inside == true );
    }
    
    SECTION( "Precomputing the reflection vector" )
    {
        auto pl = plane::create();
        auto r = ray( f_point( 0, 1, -1 ), f_vector( 0, -0.707106f, 0.707106f ) );
        auto i = intersection( 1.414214f, pl );
        auto state = prepare_intersection_state( i, r );
        
        REQUIRE( state.reflection == f_vector( 0, 0.707106f, 0.707106f ) );
    }
};
