#include "catch.hpp"
#include "tensor.hpp"

using namespace ls;

TEST_CASE( "Tensor processing", "[tensors]" )
{
    SECTION( "Tensor is a point" )
    {
        f_point t( 3.f, 4.f, -2.f );

        REQUIRE( approx( t.x, 3.f ) );
        REQUIRE( approx( t.y, 4.f ) );
        REQUIRE( approx( t.z, -2.f ) );
        REQUIRE( approx( t.w, 1.f ) );
        REQUIRE( t.is() == tensor_t::point );
    }

    SECTION( "Tensor is a vector" )
    {
        f_vector t( 3.f, 4.f, -2.f );

        REQUIRE( approx( t.x, 3.f ) );
        REQUIRE( approx( t.y, 4.f ) );
        REQUIRE( approx( t.z, -2.f ) );
        REQUIRE( approx( t.w, 0.f ) );
        REQUIRE( t.is() == tensor_t::vector );
    }

    SECTION( "Adding two tensors" )
    {
        auto vec = f_vector( 3.f, 1.4f, 9.f );
        auto pt = f_point( 1.f, 3.f, 1.f );

        REQUIRE( vec + pt == f_tensor( 4.f, 4.4f, 10.f, 1.f ) );
    }

    SECTION( "Negating a tensor" )
    {
        auto vec = f_tensor( 2.f, 4.f, 0.5f, 0.f );

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
};

TEST_CASE( "Point processing", "[tensors][points]" )
{
    SECTION( "Factory creating a point" )
    {
        auto t = f_point( 4.f, 3.f, 2.f );

        REQUIRE( t == f_tensor( 4.f, 3.f, 2.f, 1.f ) );
    }

    SECTION( "Subtracting two points" )
    {
        auto pt1 = f_point( 3.f, 2.f, 5.f );
        auto pt2 = f_point( 1.f, 1.f, 1.f );

        REQUIRE( pt1 - pt2 == f_tensor( 2.f, 1.f, 4.f, 0.f ) );
    }

    SECTION( "Subtracting a vector from a point" )
    {
        auto pt = f_point( 1.f, 2.f, 14.f );
        auto vec = f_vector( 1.f, 3.f, 5.f );

        REQUIRE( pt - vec == f_tensor( 0.f, -1.f, 9.f, 1.f ) );
    }
};

TEST_CASE( "Vector processing", "[tensors][vectors]" )
{
    SECTION( "Factory creating a vector" )
    {
        auto t = f_vector( 5.f, 2.f, 1.f );

        REQUIRE( t == f_tensor( 5.f, 2.f, 1.f, 0 ) );
    }

    SECTION( "Subtracting two vectors" )
    {
        auto vec1 = f_vector( 3.f, 11.f, 9.f );
        auto vec2 = f_vector( 5.f, 2.f, 92.f );

        REQUIRE( vec1 - vec2 == f_tensor( -2.f, 9.f, -83.f, 0.f ) );
    }

    SECTION( "Subtracting a vector from the zero vector" )
    {
        auto zero = f_vector( 0.f, 0.f, 0.f );
        auto vec = f_vector( 10.f, 3.f, 1.f );

        REQUIRE( zero - vec == f_tensor( -10.f, -3.f, -1.f, 0.f ) );
    }

    SECTION( "Computing the length of vector (1, 0, 0)" )
    {
        auto vec = f_vector( 1, 0, 0 );

        REQUIRE( vec.length() == 1.f );
    }

    SECTION( "Computing the length of vector (0, 1, 0)" )
    {
        auto vec = f_vector( 0, 1, 0 );

        REQUIRE( vec.length() == 1.f );
    }

    SECTION( "Computing the length of vector (0, 0, 1)" )
    {
        auto vec = f_vector( 0, 0, 1 );

        REQUIRE( vec.length() == 1.f );
    }

    SECTION( "Computing the length of vectors (1, 2, 3) and (-1, -2, -3)" )
    {
        auto vec = f_vector( 1, 2, 3 );

        REQUIRE( vec.length() == 3.7416573867f );

        auto nvec = -vec;

        REQUIRE( nvec.length() == 3.7416573867f );
    }

    SECTION( "Normalizing vector (4, 0, 0)" )
    {
        auto vec = f_vector( 4, 0, 0 );
        auto norm = vec.normalized();

        REQUIRE( norm == f_vector( 1, 0, 0 ) );
        REQUIRE( approx( norm.length(), 1.f ) );
    }

    SECTION( "Normalizing vector (1, 2, 3)" )
    {
        auto vec = f_vector( 1, 2, 3 );
        auto norm = vec.normalized();

        REQUIRE( norm == f_vector( 0.26726f, 0.53452f, 0.80178f ) );
        REQUIRE( approx( norm.length(), 1.f ) );
    }

    SECTION( "Computing the dot product of two vectors" )
    {
        auto vec1 = f_vector( 1, 2, 3 );
        auto vec2 = f_vector( 2, 3, 4 );

        REQUIRE( approx( vec1.dot( vec2 ), 20.f ) );
    }

    SECTION( "Computing the cross product of two vectors" )
    {
        auto vec1 = f_vector( 1, 2, 3 );
        auto vec2 = f_vector( 2, 3, 4 );

        REQUIRE( vec1.cross( vec2 ) == f_vector( -1, 2, -1 ) );
        REQUIRE( vec2.cross( vec1 ) == f_vector( 1, -2, 1 ) );
    }

    SECTION( "Reflecting a vector approaching at 45 degrees" )
    {
        auto v = f_vector( 1, -1, 0 );
        auto n = f_vector( 0, 1, 0 );
        auto r = v.reflect( n );

        REQUIRE( r == f_vector( 1, 1, 0 ) );
    }

    SECTION( "Reflecting a vector off a slanted surface" )
    {
        auto v = f_vector( 0, -1, 0 );
        auto n = f_vector( 0.7071067f, 0.7071067f, 0 );
        auto r = v.reflect( n );

        REQUIRE( r == f_vector( 1, 0, 0 ) );
    }
};

TEST_CASE( "Color processing", "[tensors][colors]" )
{
    SECTION( "Factory creating a color" )
    {
        auto t = f_color( 0.98f, 1.f, 0.f );

        REQUIRE( t == f_tensor( 0.98f, 1.f, 0.f, 1.f ) );
    }

    SECTION( "Colors are (r, g, b) tensors" )
    {
        auto color = f_color( 0.98f, 0.23f, 1.2f );

        REQUIRE( color.r == 0.98f );
        REQUIRE( color.g == 0.23f );
        REQUIRE( color.b == 1.2f );
        REQUIRE( color.a == 1.f );
    }

    SECTION( "Adding two colors" )
    {
        auto color1 = f_color( 0.98f, 0.23f, 1.2f );
        auto color2 = f_color( 0.12f, 0.44f, 0.3f );

        REQUIRE( color1 + color2 == f_tensor( 1.1f, 0.67f, 1.5f, 1.f ) );
    }

    SECTION( "Subtracting two colors" )
    {
        auto color1 = f_color( 0.66f, 0.9f, 1.f );
        auto color2 = f_color( 0.3f, 0.12f, 0.3f );

        REQUIRE( color1 - color2 == f_tensor( 0.36f, 0.78f, 0.7f, 1.f ) );
    }

    SECTION( "Multiplying a color by a scalar" )
    {
        auto color = f_color( 0.33f, 0.1f, 0.5f );

        REQUIRE( color * 0.5f == f_tensor( 0.165f, 0.05f, 0.25f, 1.f ) );
        REQUIRE( 0.5f * color == f_tensor( 0.165f, 0.05f, 0.25f, 1.f ) );
    }

    SECTION( "Multiplying colors" )
    {
        auto color1 = f_color( 1, 0.2f, 0.4f );
        auto color2 = f_color( 0.9f, 1, 0.1f );

        REQUIRE( color1 * color2 == f_tensor( 0.9f, 0.2f, 0.04f, 1.f ) );
    }
};