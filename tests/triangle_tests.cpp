#include "catch.hpp"
#include "shapes.hpp"

using namespace ls;

TEST_CASE( "Triangle processing", "[triangles]" )
{
    SECTION( "Constructing a triangle" )
    {
        auto p1 = f_point( 0, 1, 0 );
        auto p2 = f_point( -1, 0, 0 );
        auto p3 = f_point( 1, 0, 0 );
        auto t = triangle::create( p1, p2, p3 );

        REQUIRE( t->p1() == p1 );
        REQUIRE( t->p2() == p2 );
        REQUIRE( t->p3() == p3 );
        REQUIRE( t->e1() == f_vector( -1, -1, 0 ) );
        REQUIRE( t->e2() == f_vector( 1, -1, 0 ) );
        REQUIRE( t->normal() == f_vector( 0, 0, -1 ) );
    }

    SECTION( "Finding the normal on a triangle" )
    {
        auto t = triangle::create( f_point( 0, 1, 0 ), f_point( -1, 0, 0 ), f_point( 1, 0, 0 ) );
        auto n1 = t->normal( 0, 0.5f, 0 );
        auto n2 = t->normal( -0.5f, 0.75f, 0 );
        auto n3 = t->normal( 0.5f, 0.25f, 0 );

        REQUIRE( n1 == t->normal() );
        REQUIRE( n2 == t->normal() );
        REQUIRE( n3 == t->normal() );
    }

    SECTION( "Intersecting a ray parallel to the triangle" )
    {
        auto t = triangle::create( f_point( 0, 1, 0 ), f_point( -1, 0, 0 ), f_point( 1, 0, 0 ) );
        auto r = ray( f_point( 0, -1, -2 ), f_vector( 0, 1, 0 ) );
        auto itrs = intersect( t, r );

        REQUIRE( itrs.empty() );
    }

    SECTION( "A ray misses the p1-p3 edge" )
    {
        auto t = triangle::create( f_point( 0, 1, 0 ), f_point( -1, 0, 0 ), f_point( 1, 0, 0 ) );
        auto r = ray( f_point( 1, 1, -2 ), f_vector( 0, 0, 1 ) );
        auto itrs = intersect( t, r );

        REQUIRE( itrs.empty() );
    }

    SECTION( "A ray misses the p1-p2 edge" )
    {
        auto t = triangle::create( f_point( 0, 1, 0 ), f_point( -1, 0, 0 ), f_point( 1, 0, 0 ) );
        auto r = ray( f_point( -1, 1, -2 ), f_vector( 0, 0, 1 ) );
        auto itrs = intersect( t, r );

        REQUIRE( itrs.empty() );
    }

    SECTION( "A ray misses the p2-p3 edge" )
    {
        auto t = triangle::create( f_point( 0, 1, 0 ), f_point( -1, 0, 0 ), f_point( 1, 0, 0 ) );
        auto r = ray( f_point( 0, -1, -2 ), f_vector( 0, 0, 1 ) );
        auto itrs = intersect( t, r );

        REQUIRE( itrs.empty() );
    }

    SECTION( "A ray strikes a triangle" )
    {
        auto t = triangle::create( f_point( 0, 1, 0 ), f_point( -1, 0, 0 ), f_point( 1, 0, 0 ) );
        auto r = ray( f_point( 0, 0.5f, -2 ), f_vector( 0, 0, 1 ) );
        auto itrs = intersect( t, r );

        REQUIRE( itrs.size() == 1 );
        REQUIRE( approx( itrs[0].time(), 2.f ) );
    }
};