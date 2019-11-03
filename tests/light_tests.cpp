#include "catch.hpp"
#include "lights.hpp"

using namespace ls;

TEST_CASE( "Light processing", "[lights]" )
{
    SECTION( "A point light has a position and intensity" )
    {
        auto intensity = f_color( 1, 1, 1 );
        auto position = f_point( 0, 0, 0 );
        auto light = point_light( intensity, position );

        REQUIRE( light.intensity() == intensity );
        REQUIRE( light.position() == position );
    }
};