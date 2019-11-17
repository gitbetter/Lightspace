#include "catch.hpp"
#include "materials.hpp"
#include "lights.hpp"

using namespace ls;

TEST_CASE( "Material processing", "[materials]" )
{
    SECTION( "The default material" )
    {
        auto m = phong_material();

        REQUIRE( m.surface_color == f_color( 1, 1, 1 ) );
        REQUIRE( m.ambient == 0.1f );
        REQUIRE( m.diffuse == 0.9f );
        REQUIRE( m.specular == 0.9f );
        REQUIRE( m.shininess == 200.f );
    }

    SECTION( "Shading with the eye between the light and the surface" )
    {
        auto m = phong_material();
        auto p = f_point( 0, 0, 0 );
        auto eye_v = f_vector( 0, 0, -1 );
        auto normal_v = f_vector( 0, 0, -1 );
        auto light = point_light::create( f_color( 1, 1, 1 ), f_point( 0, 0, -10 ) );

        REQUIRE( phong_lighting(m, light, p, eye_v, normal_v) == f_color( 1.9f, 1.9f, 1.9f ) );
    }

    SECTION( "Shading with the eye between the light and the surface with eye offset by 45 degrees" )
    {
        auto m = phong_material();
        auto p = f_point( 0, 0, 0 );
        auto eye_v = f_vector( 0, 0.70710678f, -0.70710678f );
        auto normal_v = f_vector( 0, 0, -1 );
        auto light = point_light::create( f_color( 1, 1, 1 ), f_point( 0, 0, -10 ) );

        REQUIRE( phong_lighting( m, light, p, eye_v, normal_v ) == f_color( 1.f, 1.f, 1.f ) );
    }

    SECTION( "Shading with the eye opposite the surface and the light offset by 45 degrees" )
    {
        auto m = phong_material();
        auto p = f_point( 0, 0, 0 );
        auto eye_v = f_vector( 0, 0, -1 );
        auto normal_v = f_vector( 0, 0, -1 );
        auto light = point_light::create( f_color( 1, 1, 1 ), f_point( 0, 10, -10 ) );

        REQUIRE( phong_lighting( m, light, p, eye_v, normal_v ) == f_color( 0.7364f, 0.7364f, 0.7364f ) );
    }

    SECTION( "Shading with the eye in the path of the reflection vector" )
    {
        auto m = phong_material();
        auto p = f_point( 0, 0, 0 );
        auto eye_v = f_vector( 0, -0.70710678f, -0.70710678f );
        auto normal_v = f_vector( 0, 0, -1 );
        auto light = point_light::create( f_color( 1, 1, 1 ), f_point( 0, 10, -10 ) );

        REQUIRE( phong_lighting( m, light, p, eye_v, normal_v ) == f_color( 1.6364f, 1.6364f, 1.6364f ) );
    }

    SECTION( "Shading with the light behind the surface" )
    {
        auto m = phong_material();
        auto p = f_point( 0, 0, 0 );
        auto eye_v = f_vector( 0, 0, -1 );
        auto normal_v = f_vector( 0, 0, -1 );
        auto light = point_light::create( f_color( 1, 1, 1 ), f_point( 0, 0, 10 ) );

        REQUIRE( phong_lighting( m, light, p, eye_v, normal_v ) == f_color( 0.1f, 0.1f, 0.1f ) );
    }
};