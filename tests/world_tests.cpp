#include "catch.hpp"
#include "world.hpp"
#include "lights.hpp"
#include "shapes.hpp"
#include "transform.hpp"
#include "intersection.hpp"

using namespace ls;

TEST_CASE( "World processing", "[world]" )
{
    SECTION( "Creating a world" )
    {
        auto w = world();

        REQUIRE( w.objects().size() == 0 );
        REQUIRE( w.light() == nullptr );
    }

    SECTION( "The default world" )
    {
        auto w = world::create_default();
        auto light = point_light::create( f_color( 1, 1, 1 ), f_point( -10, 10, -10 ) );
        auto s1 = sphere::create(), s2 = sphere::create();
        auto mat1 = phong_material( f_color( 0.8f, 1.f, 0.6f ), 0.1f, 0.7f, 0.2f );

        s1->set_material( mat1 );
        s2->set_transform( transform::scale( 0.5f, 0.5f, 0.5f ) );

        REQUIRE( *( w->light() ) == *light );
        REQUIRE( w->contains( s1 ) );
        REQUIRE( w->contains( s2 ) );
    }

    SECTION( "Intersect a world with a ray" )
    {
        auto w = world::create_default();
        auto r = ray( f_point( 0, 0, -5 ), f_vector( 0, 0, 1 ) );
        auto xs = intersect( w, r );

        REQUIRE( xs.size() == 4 );
        REQUIRE( xs[0].time() == 4.f );
        REQUIRE( xs[1].time() == 4.5f );
        REQUIRE( xs[2].time() == 5.5f );
        REQUIRE( xs[3].time() == 6.f );
    }

    SECTION( "Shading an intersection" )
    {
        auto w = world::create_default();
        auto r = ray( f_point( 0, 0, -5 ), f_vector( 0, 0, 1 ) );
        auto s = w->objects()[0];
        auto i = intersection( 4, s );

        auto state = prepare_intersection_state( i, r );
        auto c = w->shade_hit( state );

        REQUIRE( c == f_color( 0.38066f, 0.47583f, 0.2855f ) );
    }

    SECTION( "Shading an intersection from the inside" )
    {
        auto w = world::create_default();
        w->set_light( point_light::create( f_color( 1, 1, 1 ), f_point( 0, 0.25f, 0 ) ) );
        auto r = ray( f_point( 0, 0, 0 ), f_vector( 0, 0, 1 ) );
        auto s = w->objects()[1];
        auto i = intersection( 0.5f, s );

        auto state = prepare_intersection_state( i, r );
        auto c = w->shade_hit( state );

        REQUIRE( c == f_color( 0.90498f, 0.90498f, 0.90498f ) );
    }
};