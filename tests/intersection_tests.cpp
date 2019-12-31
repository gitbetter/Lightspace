#include "catch.hpp"
#include "shapes.hpp"
#include "intersection.hpp"
#include "transform.hpp"
#include "lights.hpp"

using namespace ls;

TEST_CASE( "Intersection processing", "[intersections]" )
{
    SECTION( "An intersection encapsulates t and an object" )
    {
        auto s = sphere::create();
        auto i = intersection( 3.5f, s );

        REQUIRE( i.time() == 3.5f );
        REQUIRE( i.object() == s );
    }

    SECTION( "Aggregating intersections" )
    {
        auto s = sphere::create();
        auto i1 = intersection( 1, s );
        auto i2 = intersection( 2, s );
        auto xs = intersections{ i1, i2 };

        REQUIRE( xs.size() == 2 );
        REQUIRE( xs[0].time() == 1 );
        REQUIRE( xs[1].time() == 2 );
    }

    SECTION( "The hit, when all intersections have positive t" )
    {
        auto s = sphere::create();
        auto i1 = intersection( 1, s );
        auto i2 = intersection( 2, s );
        auto xs = intersections{ i2, i1 };

        REQUIRE( hit( xs ) == i1 );
    }

    SECTION( "The hit, when some intersections have negative t" )
    {
        auto s = sphere::create();
        auto i1 = intersection( -1, s );
        auto i2 = intersection( 1, s );
        auto xs = intersections{ i2, i1 };

        REQUIRE( hit( xs ) == i2 );
    }

    SECTION( "The hit, when all intersections have negative t" )
    {
        auto s = sphere::create();
        auto i1 = intersection( -2, s );
        auto i2 = intersection( -1, s );
        auto xs = intersections{ i2, i1 };

        REQUIRE( hit( xs ) == intersection::none );
    }

    SECTION( "The hit is always the lowest nonnegative intersection" )
    {
        auto s = sphere::create();
        auto i1 = intersection( 5, s );
        auto i2 = intersection( 7, s );
        auto i3 = intersection( -3, s );
        auto i4 = intersection( 2, s );
        auto xs = intersections{ i1, i2, i3, i4 };

        REQUIRE( hit( xs ) == i4 );
    }

    SECTION( "Precomputing the state of an intersection" )
    {
        auto r = ray( f_point( 0, 0, -5 ), f_vector( 0, 0, 1 ) );
        auto s = sphere::create();
        auto i = intersection( 4.f, s );
        auto istate = prepare_intersection_state( i, r );

        REQUIRE( istate.time == i.time() );
        REQUIRE( istate.object == i.object() );
        REQUIRE( istate.point == f_point( 0, 0, -1 ) );
        REQUIRE( istate.eye == f_vector( 0, 0, -1 ) );
        REQUIRE( istate.normal == f_vector( 0, 0, -1 ) );
    }

    SECTION( "The hit, when an intersection occurs on the outside" )
    {
        auto r = ray( f_point( 0, 0, -5 ), f_vector( 0, 0, 1 ) );
        auto s = sphere::create();
        auto i = intersection( 4.f, s );
        auto istate = prepare_intersection_state( i, r );

        REQUIRE( istate.inside == false );
    }

    SECTION( "The hit, when an intersection occurs on the inside" )
    {
        auto r = ray( f_point( 0, 0, 0 ), f_vector( 0, 0, 1 ) );
        auto s = sphere::create();
        auto i = intersection( 1.f, s );
        auto istate = prepare_intersection_state( i, r );

        REQUIRE( istate.point == f_point( 0, 0, 1 ) );
        REQUIRE( istate.eye == f_vector( 0, 0, -1 ) );
        REQUIRE( istate.normal == f_vector( 0, 0, -1 ) );
        REQUIRE( istate.inside == true );
    }
    
    SECTION( "Precomputing the reflection vector" )
    {
        auto pl = plane::create();
        auto r = ray( f_point( 0, 1, -1 ), f_vector( 0, -0.707106f, 0.707106f ) );
        auto i = intersection( 1.414214f, pl );
        auto state = prepare_intersection_state( i, r );
        
        REQUIRE( state.reflection == f_vector( 0, 0.707106f, 0.707106f ) );
    }
    
    SECTION( "Finding n1 and n2 at various intersections" )
    {
        auto a = sphere::create_glassy();
        a->set_transform( transform::scale( 2.f, 2.f, 2.f ) );
        a->material()->refractive_index = 1.5f;
        auto b = sphere::create_glassy();
        b->set_transform( transform::translation( 0.f, 0.f, -0.25f ) );
        b->material()->refractive_index = 2.f;
        auto c = sphere::create_glassy();
        c->set_transform( transform::translation( 0.f, 0.f, 0.25f ) );
        c->material()->refractive_index = 2.5f;
        auto r = ray( f_point( 0, 0, -4 ), f_vector( 0, 0, 1 ) );
        auto xs = intersections{
            intersection( 2, a ),
            intersection( 2.75f, b ),
            intersection( 3.25f, c ),
            intersection( 4.75f, b ),
            intersection( 5.25f, c ),
            intersection( 6, a )
        };
        auto state = prepare_intersection_state( xs[0], r, xs );
        
        REQUIRE( approx( state.ridx_from, 1.f ) );
        REQUIRE( approx( state.ridx_to, 1.5f ) );
    }
    
    SECTION( "The under point is offset below the surface" )
    {
        auto r = ray( f_point( 0, 0, -5 ), f_vector( 0, 0, 1 ) );
        auto sh = sphere::create_glassy();
        sh->set_transform( transform::translation( 0.f, 0.f, 1.f ) );
        auto i = intersection( 5, sh );
        auto itrs = intersections{ i };
        auto state = prepare_intersection_state( i, r, itrs );
        
        REQUIRE( state.shifted_under_point.z > epsilon / 2 );
        REQUIRE( state.point.z < state.shifted_under_point.z );
    }
    
    SECTION( "The Shlick approximation under total internal reflection" )
    {
        auto sh = sphere::create_glassy();
        auto r = ray( f_point( 0, 0, 0.7071067f ), f_vector( 0, 1, 0 ) );
        auto itrs = intersections{
            intersection( -0.7071067f, sh ),
            intersection( 0.7071067f, sh ),
        };
        auto state = prepare_intersection_state( itrs[1], r, itrs );
        auto reflectance = schlick( state );
        
        REQUIRE( reflectance == 1.f );
    }
    
    SECTION( "The Shlick approximation with a perpendicular viewing angle" )
    {
        auto sh = sphere::create_glassy();
        auto r = ray( f_point( 0, 0, 0 ), f_vector( 0, 1, 0 ) );
        auto itrs = intersections{
            intersection( -1, sh ),
            intersection( 1, sh ),
        };
        auto state = prepare_intersection_state( itrs[1], r, itrs );
        auto reflectance = schlick( state );
        
        REQUIRE( approx( reflectance, 0.04f ) );
    }
    
    SECTION( "The Shlick approximation with a small angle and ridx_to > ridx_from" )
    {
        auto sh = sphere::create_glassy();
        auto r = ray( f_point( 0, 0.99f, -2 ), f_vector( 0, 0, 1 ) );
        auto itrs = intersections{
            intersection( 1.8589f, sh )
        };
        auto state = prepare_intersection_state( itrs[0], r, itrs );
        auto reflectance = schlick( state );
        
        REQUIRE( approx( reflectance, 0.48873f ) );
    }
};
