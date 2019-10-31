#include "catch.hpp"
#include "ray.hpp"
#include "shapes.hpp"
#include "intersection.hpp"
#include "transform.hpp"

using namespace ls;

TEST_CASE( "Sphere processing",  "[spheres]") 
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
};