#include "catch.hpp"
#include "transform.hpp"

using namespace ls;

TEST_CASE( "Transform processing",  "[transforms]") 
{
	SECTION( "Multiplying a point by a translation matrix" )
	{
		auto tfm = transform::translation( 5, -3, 2 );
		auto p = i_point( -3, 4, 5 );
		
		REQUIRE( tfm * p == i_point( 2, 1, 7 ) );
	}

	SECTION( "Multiplying a point by an inverse translation matrix" )
	{
		auto tfm_inv = transform::translation( 5, -3, 2 ).inverse();
		auto p = i_point( -3, 4, 5 );

		REQUIRE( tfm_inv * p == i_point( -8, 7, 3 ) );
	}

	SECTION( "Translation does not affect vectors" )
	{
		auto tfm = transform::translation( 5, -3, 2 );
		auto v = i_vector( -3, 4, 5 );

		REQUIRE( tfm * v == v );
	}
};