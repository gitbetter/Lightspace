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

	SECTION( "Constructing and inspecting a 3x3 matrix" )
	{
		auto m = f3_matrix{
			-3, 5, 0,
			1, -2, 7,
			0, 1, 1
		};

		REQUIRE( m( 0, 0 ) == -3 );
		REQUIRE( m( 1, 1 ) == -2 );
		REQUIRE( m( 2, 2 ) == 1 );
	}

	SECTION( "Constructing and inspecting a 2x2 matrix" )
	{
		auto m = f2_matrix{
			2, 0.5f,
			1.5f, 1
		};

		REQUIRE( m( 0, 0 ) == 2 );
		REQUIRE( m( 0, 1 ) == 0.5f );
		REQUIRE( m( 1, 0 ) == 1.5f );
		REQUIRE( m( 1, 1 ) == 1 );
	}

	SECTION( "Matrix equality with identical matrices" )
	{
		auto m1 = i4_matrix{
			1, 2, 3, 4,
			5, 6, 7, 8,
			9, 8, 7, 6,
			5, 4, 3, 2
		};
		auto m2 = i4_matrix{
			1, 2, 3, 4,
			5, 6, 7, 8,
			9, 8, 7, 6,
			5, 4, 3, 2
		};

		REQUIRE( m1 == m2 );
	}

	SECTION( "Matrix equality with different matrices" )
	{
		auto m1 = i4_matrix{
			1, 2, 3, 4,
			5, 6, 7, 8,
			9, 8, 7, 6,
			5, 4, 3, 2
		};
		auto m2 = i4_matrix{
			2, 3, 4, 5,
			6, 7, 8, 9,
			8, 7, 6, 5,
			4, 3, 2, 1
		};

		REQUIRE( m1 != m2 );
	}

	SECTION( "Multiplying two 4x4 matrices" )
	{
		auto m1 = i4_matrix{
			1, 2, 3, 4,
			5, 6, 7, 8,
			9, 8, 7, 6,
			5, 4, 3, 2
		};
		auto m2 = i4_matrix{
			-2, 1, 2, 3,
			3, 2, 1, -1,
			4, 3, 6, 5,
			1, 2, 7, 8
		};
		auto result = i4_matrix{
			20, 22, 50, 48,
			44, 54, 114, 108,
			40, 58, 110, 102,
			16, 26, 46, 42
		};

		REQUIRE( m1 * m2 == result );
	}

	SECTION( "Multiplying a matrix by a tensor" )
	{
		auto m = i4_matrix{
			1, 2, 3, 4,
			2, 4, 4, 2,
			8, 6, 4, 1,
			0, 0, 0, 1
		};
		auto t = i_tensor( 1, 2, 3, 1 );
		auto result = i_tensor( 18, 24, 33, 1 );

		REQUIRE( m * t == result );
	}

	SECTION( "Multiplying a matrix by the identity matrix" )
	{
		auto m = i4_matrix{
			0, 1, 2, 4,
			1, 2, 4, 8,
			2, 4, 8, 16,
			4, 8, 16, 32
		};

		REQUIRE( m * i4_matrix::identity() == m );
	}

	SECTION( "Multiplying the identity matrix by a tuple" )
	{
		auto t = i_tensor( 1, 2, 3, 4 );

		REQUIRE( i4_matrix::identity() * t == t );
	}

	SECTION( "Transposing a matrix" )
	{
		auto m = i4_matrix{
			1, 9, 7, 5,
			0, 3, 3, 2,
			1, 1, 2, 5,
			8, 4, 6, 7
		};
		auto result = i4_matrix{
			1, 0, 1, 8,
			9, 3, 1, 4,
			7, 3, 2, 6,
			5, 2, 5, 7
		};

		REQUIRE( m.transpose() == result );
	}

	SECTION( "Transposing the identity matrix" )
	{
		REQUIRE( i4_matrix::identity().transpose() == i4_matrix::identity() );
	}
};