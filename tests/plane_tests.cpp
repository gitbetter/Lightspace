#include "catch.hpp"
#include "shapes.hpp"

using namespace ls;

TEST_CASE( "Plane processing", "[planes]" )
{
    SECTION( "The normal of a plane is constant everywhere" )
    {
        auto p = plane::create();
        auto n1 = p->normal( 0, 0, 0 );
        auto n2 = p->normal( 10, 0, -10 );
        auto n3 = p->normal( -5, 0, 150 );

        REQUIRE( n1 == f_vector( 0, 1, 0 ) );
        REQUIRE( n2 == f_vector( 0, 1, 0 ) );
        REQUIRE( n3 == f_vector( 0, 1, 0 ) );
    }

    SECTION( "Intersect with a ray parallel to the plane" )
    {
        auto p = plane::create();
        auto r = ray( f_point( 0, 10, 0 ), f_vector( 0, 0, 1 ) );
        auto xs = intersect( p, r );

        REQUIRE( xs.empty() );
    }

    SECTION( "Intersect with a coplanar ray" )
    {
        auto p = plane::create();
        auto r = ray( f_point( 0, 0, 0 ), f_vector( 0, 0, 1 ) );
        auto xs = intersect( p, r );

        REQUIRE( xs.empty() );
    }

    SECTION( "A ray intersecting a plane from above" )
    {
        auto p = plane::create();
        auto r = ray( f_point( 0, 1, 0 ), f_vector( 0, -1, 0 ) );
        auto xs = intersect( p, r );

        REQUIRE( xs.size() == 1 );
        REQUIRE( xs[0].time() == 1 );
        REQUIRE( xs[0].object() == p );
    }

    SECTION( "A ray intersecting a plane from below" )
    {
        auto p = plane::create();
        auto r = ray( f_point( 0, -1, 0 ), f_vector( 0, 1, 0 ) );
        auto xs = intersect( p, r );

        REQUIRE( xs.size() == 1 );
        REQUIRE( xs[0].time() == 1 );
        REQUIRE( xs[0].object() == p );
    }
};