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

	SECTION( "A scaling matrix applied to a point" )
	{
		auto tfm = transform::scale( 2, 3, 4 );
		auto p = i_point( -4, 6, 8 );

		REQUIRE( tfm * p == i_point( -8, 18, 32 ) );
	}

	SECTION( "A scaling matrix applied to a vector" )
	{
		auto tfm = transform::scale( 2, 3, 4 );
		auto v = i_vector( -4, 6, 8 );

		REQUIRE( tfm * v == i_vector( -8, 18, 32 ) );
	}

	SECTION( "Multipliying by the inverse of a scaling matrix" )
	{
		auto tfm_inv = transform::scale( 2.f, 3.f, 4.f ).inverse();
		auto v = f_vector( -4, 6, 8 );

		REQUIRE( tfm_inv * v == f_vector( -2, 2, 2 ) );
	}

	SECTION( "Reflection is scaling by a negative value" )
	{
		auto tfm = transform::scale( -1, 1, 1 );
		auto p = i_point( 2, 3, 4 );

		REQUIRE( tfm * p == i_point( -2, 3, 4 ) );
	}

	SECTION( "Rotating a point around the x axis" )
	{
		auto p = f_point( 0, 1, 0 );
		auto half_quarter = transform::rotation_x( pi_over_4 );
		auto full_quarter = transform::rotation_x( pi_over_2 );

		REQUIRE( half_quarter * p == f_point( 0, 0.7071067811f, 0.7071067811f ) );
		REQUIRE( full_quarter * p == f_point( 0, 0, 1 ) );
	}

	SECTION( "The inverse of an x-rotation rotates in the opposite direction" )
	{
		auto p = f_point( 0, 1, 0 );
		auto half_quarter = transform::rotation_x( pi_over_4 );

		REQUIRE( half_quarter.inverse() * p == f_point( 0, 0.7071067811f, -0.7071067811f ) );
	}

	SECTION( "Rotating a point around the y axis" )
	{
		auto p = f_point( 0, 0, 1 );
		auto half_quarter = transform::rotation_y( pi_over_4 );
		auto full_quarter = transform::rotation_y( pi_over_2 );

		REQUIRE( half_quarter * p == f_point( 0.7071067811f, 0, 0.7071067811f ) );
		REQUIRE( full_quarter * p == f_point( 1, 0, 0 ) );
	}

	SECTION( "Rotating a point around the z axis" )
	{
		auto p = f_point( 0, 1, 0 );
		auto half_quarter = transform::rotation_z( pi_over_4 );
		auto full_quarter = transform::rotation_z( pi_over_2 );

		REQUIRE( half_quarter * p == f_point( -0.7071067811f, 0.7071067811f, 0 ) );
		REQUIRE( full_quarter * p == f_point( -1, 0, 0 ) );
	}

	SECTION( "Shearing x in proportion to y" )
	{
		auto p = i_point( 2, 3, 4 );
		auto tfm = transform::shear( 1, 0, 0, 0, 0, 0 );

		REQUIRE( tfm * p == i_point( 5, 3, 4 ) );
	}

	SECTION( "Shearing x in proportion to z" )
	{
		auto p = i_point( 2, 3, 4 );
		auto tfm = transform::shear( 0, 1, 0, 0, 0, 0 );

		REQUIRE( tfm * p == i_point( 6, 3, 4 ) );
	}

	SECTION( "Shearing y in proportion to x" )
	{
		auto p = i_point( 2, 3, 4 );
		auto tfm = transform::shear( 0, 0, 1, 0, 0, 0 );

		REQUIRE( tfm * p == i_point( 2, 5, 4 ) );
	}

	SECTION( "Shearing y in proportion to z" )
	{
		auto p = i_point( 2, 3, 4 );
		auto tfm = transform::shear( 0, 0, 0, 1, 0, 0 );

		REQUIRE( tfm * p == i_point( 2, 7, 4 ) );
	}

	SECTION( "Shearing z in proportion to x" )
	{
		auto p = i_point( 2, 3, 4 );
		auto tfm = transform::shear( 0, 0, 0, 0, 1, 0 );

		REQUIRE( tfm * p == i_point( 2, 3, 6 ) );
	}

	SECTION( "Shearing z in proportion to y" )
	{
		auto p = i_point( 2, 3, 4 );
		auto tfm = transform::shear( 0, 0, 0, 0, 0, 1 );

		REQUIRE( tfm * p == i_point( 2, 3, 7 ) );
	}

	SECTION( "Individual transformations are applied in sequence" )
	{
		auto p = f_point( 1, 0, 1 );
		auto rot = transform::rotation_x( pi_over_2 );
		auto scl = transform::scale( 5.f, 5.f, 5.f );
		auto tsl = transform::translation( 10.f, 5.f, 7.f );

		auto p2 = rot * p;

		REQUIRE( p2 == f_point( 1, -1, 0 ) );

		p2 = scl * p2;

		REQUIRE( p2 == f_point( 5, -5, 0 ) );

		p2 = tsl * p2;

		REQUIRE( p2 == f_point( 15, 0, 7 ) );
	}

	SECTION( "Chained transforms must be applied in reverse order" )
	{
		auto p = f_point( 1, 0, 1 );
		auto rot = transform::rotation_x( pi_over_2 );
		auto scl = transform::scale( 5.f, 5.f, 5.f );
		auto tsl = transform::translation( 10.f, 5.f, 7.f );

		auto tfm = tsl * scl * rot;

		REQUIRE( tfm * p == f_point( 15, 0, 7 ) );
	}
};