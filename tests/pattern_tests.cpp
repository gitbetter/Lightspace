#include "catch.hpp"
#include "patterns.hpp"

using namespace ls;

TEST_CASE( "Pattern processing", "[pattern]" )
{
    SECTION( "Creating a stripe pattern" )
    {
        auto black = f_color(0, 0, 0);
        auto white = f_color(1, 1, 1);
        auto patt = stripe_pattern(black, white);

        REQUIRE(patt.first() == black);
        REQUIRE(patt.second() == white);
    }

    SECTION( "A stripe pattern is constant in Y" )
    {
        auto black = f_color( 0, 0, 0 );
        auto white = f_color( 1, 1, 1 );
        auto patt = stripe_pattern( black, white );

        REQUIRE( patt.stripe_at(f_point(0, 0, 0)) == black );
        REQUIRE( patt.stripe_at( f_point( 0, 1, 0 ) ) == black );
        REQUIRE( patt.stripe_at( f_point( 0, 2, 0 ) ) == black );
    }

    SECTION( "A stripe pattern is constant in Z" )
    {
        auto black = f_color( 0, 0, 0 );
        auto white = f_color( 1, 1, 1 );
        auto patt = stripe_pattern( black, white );

        REQUIRE( patt.stripe_at( f_point( 0, 0, 0 ) ) == black );
        REQUIRE( patt.stripe_at( f_point( 0, 0, 1 ) ) == black );
        REQUIRE( patt.stripe_at( f_point( 0, 0, 2 ) ) == black );
    }

    SECTION( "A stripe pattern alternates in X" )
    {
        auto black = f_color( 0, 0, 0 );
        auto white = f_color( 1, 1, 1 );
        auto patt = stripe_pattern( black, white );

        REQUIRE( patt.stripe_at( f_point( 0, 0, 0 ) ) == black );
        REQUIRE( patt.stripe_at( f_point( 0.9f, 0, 0 ) ) == black );
        REQUIRE( patt.stripe_at( f_point( 1, 0, 0 ) ) == white );
        REQUIRE( patt.stripe_at( f_point( -0.1f, 0, 0 ) ) == white );
        REQUIRE( patt.stripe_at( f_point( -1, 0, 0 ) ) == white );
        REQUIRE( patt.stripe_at( f_point( -1.1f, 0, 0 ) ) == black );
    }
};