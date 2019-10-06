#include "catch.hpp"
#include "tensor.hpp"

using namespace ls;

TEST_CASE( "Tensor processing",  "[tensors]") 
{
	SECTION( "Tensor is a point" ) 
	{
		f_tensor t( 3.f, 4.f, -2.f, 1.f );

		REQUIRE( approx(t.x, 3.f) );
		REQUIRE( approx(t.y, 4.f) );
		REQUIRE( approx(t.z, -2.f) );
		REQUIRE( approx(t.w, 1.f) );
		REQUIRE( t.is() == tensor_t::point );
	}

	SECTION( "Tensor is a vector" ) 
	{
		f_tensor t( 3.f, 4.f, -2.f, 0.f );

		REQUIRE( approx(t.x, 3.f) );
		REQUIRE( approx(t.y, 4.f) );
		REQUIRE( approx(t.z, -2.f) );
		REQUIRE( approx(t.w, 0.f) );
		REQUIRE( t.is() == tensor_t::vector );
	}

	SECTION( "Factory creating a point" ) 
	{
		auto t = f_tensor::point(4.f, 3.f, 2.f);

		REQUIRE( t == f_tensor(4.f, 3.f, 2.f, 1.f));
	}

	SECTION( "Factory creating a vector" ) 
	{
		auto t = f_tensor::vector(5.f, 2.f, 1.f);

		REQUIRE( t == f_tensor(5.f, 2.f, 1.f, 0));
	}

	SECTION( "Adding two tensors" )
	{
		auto vec = f_tensor::vector( 3.f, 1.4f, 9.f );
		auto pt = f_tensor::point( 1.f, 3.f, 1.f );

		REQUIRE( vec + pt == f_tensor( 4.f, 4.4f, 10.f, 1.f ) );
	}

	SECTION( "Subtracting two points" )
	{
		auto pt1 = f_tensor::point( 3.f, 2.f, 5.f );
		auto pt2 = f_tensor::point( 1.f, 1.f, 1.f );

		REQUIRE( pt1 - pt2 == f_tensor( 2.f, 1.f, 4.f, 0.f ) );
	}

	SECTION( "Subtracting a vector from a point" )
	{
		auto pt = f_tensor::point( 1.f, 2.f, 14.f );
		auto vec = f_tensor::vector( 1.f, 3.f, 5.f );

		REQUIRE( pt - vec == f_tensor( 0.f, -1.f, 9.f, 1.f ) );
	}

	SECTION( "Subtracting a vector from a vector" )
	{
		auto vec1 = f_tensor::vector( 3.f, 11.f, 9.f );
		auto vec2 = f_tensor::vector( 5.f, 2.f, 92.f );

		REQUIRE( vec1 - vec2 == f_tensor( -2.f, 9.f, -83.f, 0.f ) );
	}

	SECTION( "Subtracting a vector from the zero vector" )
	{
		auto zero = f_tensor::vector( 0.f, 0.f, 0.f );
		auto vec = f_tensor::vector( 10.f, 3.f, 1.f );

		REQUIRE( zero - vec == f_tensor( -10.f, -3.f, -1.f, 0.f ) );
	}

	SECTION( "Negating a vector" )
	{
		auto vec = f_tensor::vector( 2.f, 4.f, 0.5f );

		REQUIRE( -vec == f_tensor( -2.f, -4.f, -0.5f, 0.f ) );
	}

	SECTION( "Multiplying a tensor by a scalar" )
	{
		auto t = f_tensor( 5.f, 2.f, 10.f, 3.f );

		REQUIRE( 1.5f * t == f_tensor( 7.5f, 3.f, 15.f, 4.5f ) );
	}

	SECTION( "Multiplying a tensor by a fraction" )
	{
		auto t = f_tensor( 10.f, 6.f, 4.f, -5.f );

		REQUIRE( t * 0.5f == f_tensor( 5.f, 3.f, 2.f, -2.5f ) );
	}

	SECTION( "Dividing a tensor by a scalar" )
	{
		auto t = f_tensor( 12.f, 9.f, 90.f, 8.f );

		REQUIRE( t / 2.f == f_tensor( 6.f, 4.5f, 45.f, 4.f ) );
	}

	SECTION( "Computing the length of vector (1, 0, 0)" )
	{
		auto vec = f_tensor::vector( 1, 0, 0 );

		REQUIRE( vec.length() == 1.f );
	}

	SECTION( "Computing the length of vector (0, 1, 0)" )
	{
		auto vec = f_tensor::vector( 0, 1, 0 );

		REQUIRE( vec.length() == 1.f );
	}

	SECTION( "Computing the length of vector (0, 0, 1)" )
	{
		auto vec = f_tensor::vector( 0, 0, 1 );

		REQUIRE( vec.length() == 1.f );
	}

	SECTION( "Computing the length of vector (1, 2, 3) and (-1, -2, -3)" )
	{
		auto vec = f_tensor::vector( 1, 2, 3 );

		REQUIRE( vec.length() == 3.7416573867f );

		auto nvec = -vec;

		REQUIRE( nvec.length() == 3.7416573867f );
	}
}