#include "catch.hpp"
#include "ray.hpp"
#include "shapes.hpp"
#include "intersection.hpp"
#include "transform.hpp"
#include "materials.hpp"

using namespace ls;

TEST_CASE( "Sphere processing", "[spheres]" )
{
    SECTION( "A ray intersects a sphere at two points" )
    {
        auto r = ray( f_point( 0, 0, -5 ), f_vector( 0, 0, 1 ) );
        auto s = sphere::create();
        auto xs = intersect( s, r );

        REQUIRE( xs.size() == 2 );
        REQUIRE( xs[0].time() == 4.f );
        REQUIRE( xs[1].time() == 6.f );
    }

    SECTION( "A ray intersects a sphere at a tangent" )
    {
        auto r = ray( f_point( 0, 1, -5 ), f_vector( 0, 0, 1 ) );
        auto s = sphere::create();
        auto xs = intersect( s, r );

        REQUIRE( xs.size() == 2 );
        REQUIRE( xs[0].time() == 5.f );
        REQUIRE( xs[1].time() == 5.f );
    }

    SECTION( "A ray misses a sphere" )
    {
        auto r = ray( f_point( 0, 2, -5 ), f_vector( 0, 0, 1 ) );
        auto s = sphere::create();
        auto xs = intersect( s, r );

        REQUIRE( xs.size() == 0 );
    }

    SECTION( "A ray originates inside a sphere" )
    {
        auto r = ray( f_point( 0, 0, 0 ), f_vector( 0, 0, 1 ) );
        auto s = sphere::create();
        auto xs = intersect( s, r );

        REQUIRE( xs.size() == 2 );
        REQUIRE( xs[0].time() == -1.f );
        REQUIRE( xs[1].time() == 1.f );
    }

    SECTION( "A sphere is behind a ray" )
    {
        auto r = ray( f_point( 0, 0, 5 ), f_vector( 0, 0, 1 ) );
        auto s = sphere::create();
        auto xs = intersect( s, r );

        REQUIRE( xs.size() == 2 );
        REQUIRE( xs[0].time() == -6.f );
        REQUIRE( xs[1].time() == -4.f );
    }

    SECTION( "Intersect sets the intersected object" )
    {
        auto r = ray( f_point( 0, 0, 5 ), f_vector( 0, 0, 1 ) );
        auto s = sphere::create();
        auto xs = intersect( s, r );

        REQUIRE( xs.size() == 2 );
        REQUIRE( xs[0].object() == s );
        REQUIRE( xs[1].object() == s );
    }

    SECTION( "A sphere's default transformation" )
    {
        auto s = sphere::create();

        REQUIRE( s->transform() == f4_matrix::identity() );
    }

    SECTION( "Changing a sphere's transformation" )
    {
        auto s = sphere::create();
        auto t = transform::translation( 2.f, 3.f, 4.f );

        s->set_transform( t );

        REQUIRE( s->transform() == t );
    }

    SECTION( "Intersecting a scaled sphere with a ray" )
    {
        auto r = ray( f_point( 0, 0, -5 ), f_vector( 0, 0, 1 ) );
        auto s = sphere::create();

        s->set_transform( transform::scale( 2.f, 2.f, 2.f ) );

        auto xs = intersect( s, r );

        REQUIRE( xs.size() == 2 );
        REQUIRE( xs[0].time() == 3 );
        REQUIRE( xs[1].time() == 7 );
    }

    SECTION( "Intersecting a translated sphere with a ray" )
    {
        auto r = ray( f_point( 0, 0, -5 ), f_vector( 0, 0, 1 ) );
        auto s = sphere::create();

        s->set_transform( transform::translation( 5.f, 0.f, 0.f ) );

        auto xs = intersect( s, r );

        REQUIRE( xs.size() == 0 );
    }

    SECTION( "The normal on a sphere at a point on the x axis" )
    {
        auto s = sphere::create();
        auto n = s->normal( 1, 0, 0 );

        REQUIRE( n == f_vector( 1, 0, 0 ) );
    }

    SECTION( "The normal on a sphere at a point on the y axis" )
    {
        auto s = sphere::create();
        auto n = s->normal( 0, 1, 0 );

        REQUIRE( n == f_vector( 0, 1, 0 ) );
    }

    SECTION( "The normal on a sphere at a point on the z axis" )
    {
        auto s = sphere::create();
        auto n = s->normal( 0, 0, 1 );

        REQUIRE( n == f_vector( 0, 0, 1 ) );
    }

    SECTION( "The normal on a sphere at a nonaxial point" )
    {
        auto s = sphere::create();
        auto n = s->normal( 0.577350269f, 0.577350269f, 0.577350269f );

        REQUIRE( n == f_vector( 0.577350269f, 0.577350269f, 0.577350269f ) );
    }

    SECTION( "The normal is a normalized vector" )
    {
        auto s = sphere::create();
        auto n = s->normal( 0.577350269f, 0.577350269f, 0.577350269f );

        REQUIRE( n == n.normalized() );
    }

    SECTION( "Computing the normal on a translated sphere" )
    {
        auto s = sphere::create();
        s->set_transform( transform::translation( 0.f, 1.f, 0.f ) );
        auto n = s->normal( 0, 1.70711f, -0.70711f );

        REQUIRE( n == f_vector( 0, 0.70711f, -0.70711f ) );
    }

    SECTION( "Computing the normal on a transformed sphere" )
    {
        auto s = sphere::create();
        auto m = transform::scale( 1.f, 0.5f, 1.f ) * transform::rotation_z( pi_over_5 );
        s->set_transform( m );
        auto n = s->normal( 0, 0.7071067f, -0.7071067f );

        REQUIRE( n == f_vector( 0, 0.97014f, -0.24254f ) );
    }

    SECTION( "A sphere has a default material" )
    {
        auto s = sphere::create();

        REQUIRE( *( s->material() ) == phong_material() );
    }

    SECTION( "A sphere may be assigned a material" )
    {
        auto s = sphere::create();
        auto m = phong_material::create();
        m->ambient = 1.f;
        s->set_material( m );

        REQUIRE( *( s->material() ) == *m );
    }
    
    SECTION( "A helper function for producing a sphere with a glassy material" )
    {
        auto s = sphere::create_glassy();
        
        REQUIRE( s->transform() == f4_matrix::identity() );
        REQUIRE( approx( s->material()->transparency, 1.f ) );
        REQUIRE( approx( s->material()->refractive_index, 1.5f ) );
    }
};
