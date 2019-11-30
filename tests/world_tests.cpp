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

    SECTION( "The color when a ray misses" )
    {
        auto w = world::create_default();
        auto r = ray( f_point( 0, 0, -5 ), f_vector( 0, 1, 0 ) );
        auto c = w->color_at( r );

        REQUIRE( c == f_color( 0, 0, 0 ) );
    }

    SECTION( "The color when a ray hits" )
    {
        auto w = world::create_default();
        auto r = ray( f_point( 0, 0, -5 ), f_vector( 0, 0, 1 ) );
        auto c = w->color_at( r );

        REQUIRE( c == f_color( 0.38066f, 0.47583f, 0.2855f ) );
    }

    SECTION( "The color with an intersection behind the ray" )
    {
        auto w = world::create_default();
        auto outer = w->objects()[0];
        auto m = outer->material();
        m.ambient = 1;
        outer->set_material( m );
        auto inner = w->objects()[1];
        m = inner->material();
        m.ambient = 1;
        inner->set_material( m );
        auto r = ray( f_point( 0, 0, 0.75f ), f_vector( 0, 0, -1 ) );
        auto c = w->color_at( r );

        REQUIRE( c == inner->material().surface_color );
    }

    SECTION( "There is no shadow when nothing is collinear with the the point and the light" )
    {
        auto w = world::create_default();
        auto p = f_point( 0, 10, 0 );

        REQUIRE( !w->in_shadow( p ) );
    }

    SECTION( "The shadow when an object is between the point and the light" )
    {
        auto w = world::create_default();
        auto p = f_point( 10, -10, 10 );

        REQUIRE( w->in_shadow( p ) );
    }
    
    SECTION( "There is no shadow when an object is behind the light" )
    {
        auto w = world::create_default();
        auto p = f_point( -20, 20, -20 );

        REQUIRE( !w->in_shadow( p ) );
    }

    SECTION( "There is no shadow when an object is behind the point" )
    {
        auto w = world::create_default();
        auto p = f_point( -2, 2, -2 );

        REQUIRE( !w->in_shadow( p ) );
    }

    SECTION( "shade_hit() is given an intersection in shadow" )
    {
        auto w = world::create();
        auto l = point_light::create( f_color( 1, 1, 1 ), f_point( 0, 0, -10 ) );
        w->set_light( l );
        auto s1 = sphere::create();
        w->add_object( s1 );
        auto s2 = sphere::create();
        s2->set_transform( transform::translation( 0.f, 0.f, 10.f ) );
        w->add_object( s2 );

        auto r = ray( f_point( 0, 0, 5 ), f_vector( 0, 0, 1 ) );
        auto i = intersection( 4, s2 );
        auto state = prepare_intersection_state( i, r );
        auto c = w->shade_hit( state );

        REQUIRE( c == f_color( 0.1f, 0.1f, 0.1f ) );
    }

    SECTION( "The hit should offset the point" )
    {
        auto r = ray( f_point( 0, 0, -5 ), f_point( 0, 0, 1 ) );
        auto s = sphere::create();
        s->set_transform( transform::translation( 0.f, 0.f, 1.f ) );
        auto i = intersection( 5, s );
        auto state = prepare_intersection_state( i, r );
        
        REQUIRE( state.shifted_point.z < -( epsilon * 0.5f ) );
        REQUIRE( state.point.z > state.shifted_point.z );
    }
};