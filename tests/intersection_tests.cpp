#include "catch.hpp"
#include "intersection.hpp"

using namespace ls;

TEST_CASE( "Intersection processing",  "[intersections]") 
{
	SECTION( "An intersection encapsulates t and an object" )
	{
		auto s = sphere();
		auto i = intersection( 3.5f, s );

		REQUIRE( i.time() == 3.5f );
		REQUIRE( i.object() == s );
	}

	SECTION( "Aggregating intersections" )
	{
		auto s = sphere();
		auto i1 = intersection( 1, s );
		auto i2 = intersection( 2, s );
		auto xs = intersections{ i1, i2 };

		REQUIRE( xs.size() == 2 );
		REQUIRE( xs[0].time() == 1 );
		REQUIRE( xs[1].time() == 2 );
	}
};