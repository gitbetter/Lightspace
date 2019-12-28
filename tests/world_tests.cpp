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
        auto mat1 = phong_material::create( f_color( 0.8f, 1.f, 0.6f ), 0.1f, 0.7f, 0.2f );

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
        outer->material()->ambient = 1;
        auto inner = w->objects()[1];
        inner->material()->ambient = 1;
        auto r = ray( f_point( 0, 0, 0.75f ), f_vector( 0, 0, -1 ) );
        auto c = w->color_at( r );

        REQUIRE( c == inner->material()->surface_pattern->color_at( f_point( 0, 0, 0 ) ) );
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
        auto r = ray( f_point( 0, 0, -5 ), f_vector( 0, 0, 1 ) );
        auto s = sphere::create();
        s->set_transform( transform::translation( 0.f, 0.f, 1.f ) );
        auto i = intersection( 5, s );
        auto state = prepare_intersection_state( i, r );
        
        REQUIRE( state.shifted_point.z < -( epsilon * 0.5f ) );
        REQUIRE( state.point.z > state.shifted_point.z );
    }
    
    SECTION( "The reflected color for a non-reflective material" )
    {
        auto w = world::create_default();
        auto r = ray( f_point( 0, 0, 0 ), f_vector( 0, 0, 1 ) );
        auto sh = w->objects()[1];
        sh->material()->ambient = 1.f;
        auto i = intersection( 1, sh );
        auto state = prepare_intersection_state( i, r );
        auto colr = w->reflected_color( state );
        
        REQUIRE( colr == f_color( 0, 0, 0 ) );
    }
    
    SECTION( "The reflected color for a reflective material" )
    {
        auto w = world::create_default();
        auto pl = plane::create();
        pl->material()->reflectivity = 0.5f;
        pl->set_transform( transform::translation( 0.f, -1.f, 0.f ) );
        w->add_object( pl );
        auto r = ray( f_point( 0, 0, -3 ), f_vector( 0, -0.7071067f, 0.7071067f ) );
        auto i = intersection( 1.414214f, pl );
        auto state = prepare_intersection_state( i, r );
        auto colr = w->reflected_color( state );
        
        REQUIRE( colr == f_color( 0.19033f, 0.23791f, 0.14274f ) );
    }
    
    SECTION( "shade_hit with a reflective material" )
    {
        auto w = world::create_default();
        auto pl = plane::create();
        pl->material()->reflectivity = 0.5f;
        pl->set_transform( transform::translation( 0.f, -1.f, 0.f ) );
        w->add_object( pl );
        auto r = ray( f_point( 0, 0, -3 ), f_vector( 0, -0.7071067f, 0.7071067f ) );
        auto i = intersection( 1.414214f, pl );
        auto state = prepare_intersection_state( i, r );
        auto colr = w->shade_hit( state );
        
        REQUIRE( colr == f_color( 0.87675f, 0.92434f, 0.82917f ) );
    }
    
    SECTION( "color_at with mutually reflective surfaces" )
    {
        auto w = world::create();
        w->set_light( point_light::create( f_color( 1, 1, 1 ), f_point( 0, 0, 0 ) ) );
        auto pl_lower = plane::create();
        pl_lower->material()->reflectivity = 1.f;
        pl_lower->set_transform( transform::translation( 0.f, -1.f, 0.f ) );
        w->add_object( pl_lower );
        auto pl_upper = plane::create();
        pl_upper->material()->reflectivity = 1.f;
        pl_upper->set_transform( transform::translation( 0.f, 1.f, 0.f ) );
        w->add_object( pl_upper );
        auto r = ray( f_point( 0, 0, 0 ), f_vector( 0, 1, 0 ) );
        auto colr = w->color_at( r );
        
        SUCCEED( );
    }
    
    SECTION( "The reflective color at the maximum recursive depth" )
    {
        auto w = world::create_default();
        auto pl = plane::create();
        pl->material()->reflectivity = 0.5f;
        pl->set_transform( transform::translation( 0.f, -1.f, 0.f ) );
        w->add_object( pl );
        auto r = ray( f_point( 0, 0, -3 ), f_vector( 0, -0.7071067f, 0.7071067f ) );
        auto i = intersection( 1.414214f, pl );
        auto state = prepare_intersection_state( i, r );
        auto colr = w->reflected_color( state, 0 );
        
        REQUIRE( colr == f_color( 0, 0, 0 ) );
    }
};
