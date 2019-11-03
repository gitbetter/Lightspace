#include "catch.hpp"
#include "materials.hpp"

using namespace ls;

TEST_CASE( "Material processing", "[materials]" )
{
    SECTION( "The default material" )
    {
        auto m = phong_material();

        REQUIRE( m.color == f_color( 1, 1, 1 ) );
        REQUIRE( m.ambient == 0.1f );
        REQUIRE( m.diffuse == 0.9f );
        REQUIRE( m.specular == 0.9f );
        REQUIRE( m.shininess == 200.f );
    }
};