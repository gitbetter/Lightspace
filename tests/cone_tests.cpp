#include "catch.hpp"
#include "shapes.hpp"

using namespace ls;

TEST_CASE( "Cone processing", "[cones]" )
{
    SECTION( "Intersecting a cone with a ray" )
    {
        auto sh = cone::create();
        auto r = ray( f_point( 1, 1, -5 ), f_vector( -0.5f, -1, 1 ).normalized() );
        auto itrs = intersect( sh, r );

        REQUIRE( itrs.size() == 2 );
        REQUIRE( approx( itrs[0].time(), 4.55006f ) );
        REQUIRE( approx( itrs[1].time(), 49.44996f ) );
    }

    SECTION( "Intersecting a cone with a ray parallel to one of its halves" )
    {
        auto sh = cone::create();
        auto r = ray( f_point( 0, 0, -1 ), f_vector( 0, 1, 1 ).normalized() );
        auto itrs = intersect( sh, r );

        REQUIRE( itrs.size() == 1 );
        REQUIRE( approx( itrs[0].time(), 0.35355f ) );
    }

    SECTION( "Intersecting a cone's end caps" )
    {
        auto sh = cone::create();
        sh->set_min_extent( -0.5f );
        sh->set_max_extent( 0.5f );
        sh->set_closed( true );
        auto r = ray( f_point( 0, 0, -0.25f ), f_vector( 0, 1, 0 ).normalized() );
        auto itrs = intersect( sh, r );

        REQUIRE( itrs.size() == 4 );
    }

    SECTION( "Computing the normal vector on a cone" )
    {
        auto sh = cone::create();
        auto n = sh->normal( 0, 0, 0 );

        REQUIRE( n == f_vector( 0, 0, 0 ) );
    }
};