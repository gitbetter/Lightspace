#include "catch.hpp"
#include "shapes.hpp"

using namespace ls;

TEST_CASE( "Cube processing", "[cubes]" )
{
    SECTION( "A ray intersects a cube" )
    {
    	auto c = cube::create();
        auto r = ray( f_point( 0, 0.5, 0 ), f_vector( 0, 0, 1 ) );
        auto xs = intersect( c, r );

        REQUIRE( xs.size() == 2 );
        REQUIRE( xs[0].time() == -1.f );
        REQUIRE( xs[1].time() == 1.f );
    }
    
    SECTION( "A ray misses a cube" )
    {
        auto c = cube::create();
        auto r = ray( f_point( 2, 2, 0 ), f_vector( -1, 0, 0 ) );
        auto xs = intersect( c, r );

        REQUIRE( xs.size() == 0 );
    }
    
    SECTION( "The normal on the surface of a cube" )
    {
        auto c = cube::create();
        auto n = c->normal( -1, -1, -1 );

        REQUIRE( n == f_vector( -1, 0, 0 ) );
    }
};
