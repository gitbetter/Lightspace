#include "catch.hpp"
#include "ray.hpp"

using namespace ls;

TEST_CASE( "Ray processing",  "[rays]") 
{
	SECTION( "Creating and querying a ray" )
	{
		auto o = f_point( 1, 2, 3 );
		auto d = f_vector( 4, 5, 6 );
		auto r = ray( o, d );
		
		REQUIRE( r.origin() == o );
		REQUIRE( r.direction() == d );
	}

	SECTION( "Computing a point from a distance" ) 
	{
		auto r = ray( f_point( 2, 3, 4 ), f_vector( 1, 0, 0 ) );

		REQUIRE( r.position( 0.f ) == f_point( 2, 3, 4 ) );
		REQUIRE( r.position( 1.f ) == f_point( 3, 3, 4 ) );
		REQUIRE( r.position( -1.f ) == f_point( 1, 3, 4 ) );
		REQUIRE( r.position( 2.5f ) == f_point( 4.5f, 3, 4 ) );
	}
};