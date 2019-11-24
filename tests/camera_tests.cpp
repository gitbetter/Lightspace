#include "catch.hpp"
#include "camera.hpp"
#include "world.hpp"

using namespace ls;

TEST_CASE( "Camera processing", "[camera]" )
{
    SECTION( "Contructing a camera" )
    {
        auto hsize = 160;
        auto vsize = 120;
        auto fov = pi_over_2;
        auto c = camera::create( hsize, vsize, fov );

        REQUIRE( c->width() == hsize );
        REQUIRE( c->height() == vsize );
        REQUIRE( c->field_of_view() == fov );
        REQUIRE( c->transform() == f4_matrix::identity() );
    }

    SECTION( "The pixel size for a horizontal canvas" )
    {
        auto c = camera::create( 200, 125, pi_over_2 );

        REQUIRE( approx( c->pixel_size(), 0.01f ) );
    }

    SECTION( "The pixel size for a vertical canvas" )
    {
        auto c = camera::create( 125, 200, pi_over_2 );

        REQUIRE( approx( c->pixel_size(), 0.01f ) );
    }

    SECTION( "Constructing a ray through the center of the canvas" )
    {
        auto c = camera::create( 201, 101, pi_over_2 );
        auto r = c->ray_for_pixel( 100, 50 );

        REQUIRE( r.origin() == f_point( 0, 0, 0 ) );
        REQUIRE( r.direction() == f_vector( 0, 0, -1 ) );
    }

    SECTION( "Constructing a ray through a corner of the canvas" )
    {
        auto c = camera::create( 201, 101, pi_over_2 );
        auto r = c->ray_for_pixel( 0, 0 );

        REQUIRE( r.origin() == f_point( 0, 0, 0 ) );
        REQUIRE( r.direction() == f_vector( 0.66519f, 0.33259f, -0.66851f ) );
    }

    SECTION( "Constructing a ray when the camera is transformed" )
    {
        auto c = camera::create( 201, 101, pi_over_2 );
        c->set_transform( transform::rotation_y( pi_over_4 ) * transform::translation( 0.f, -2.f, 5.f ) );
        auto r = c->ray_for_pixel( 100, 50 );

        REQUIRE( r.origin() == f_point( 0, 2, -5 ) );
        REQUIRE( r.direction() == f_vector( 0.7071067f, 0, -0.7071067f ) );
    }

    SECTION( "Rendering a world with a camera" )
    {
        auto w = world::create_default();
        auto c = camera::create( 11, 11, pi_over_2 );
        auto from = f_point( 0, 0, -5 );
        auto to = f_point( 0, 0, 0 );
        auto up = f_vector( 0, 1, 0 );
        c->set_transform( transform::view( from, to, up ) );
        auto img = c->render( w );

        REQUIRE( img.pixel_at( 5, 5 ) == f_color( 0.38066f, 0.47583f, 0.2855f ) );
    }
};