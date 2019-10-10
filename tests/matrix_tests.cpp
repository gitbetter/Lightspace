#include "catch.hpp"
#include "matrix.hpp"

using namespace ls;

TEST_CASE( "Matrix processing",  "[matrices]") 
{
	SECTION( "Constructing and inspecting a 4x4 matrix" )
	{
		auto m = f4_matrix{
			1,    2,    3,    4,
			5.5f, 6.5f, 7.5f, 8.5f,
			9,    10,   11,   12,
			13.5, 14.5, 15.5, 16.5 
		};

		REQUIRE( m( 0, 0 ) == 1 );
		REQUIRE( m( 0, 3 ) == 4 );
		REQUIRE( m( 1, 0 ) == 5.5f );
		REQUIRE( m( 1, 2 ) == 7.5f );
		REQUIRE( m( 2, 2 ) == 11 );
		REQUIRE( m( 3, 0 ) == 13.5f );
		REQUIRE( m( 3, 2 ) == 15.5f );
	}
};