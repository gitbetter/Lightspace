#include "catch.hpp"
#include "canvas.hpp"

using namespace ls;

TEST_CASE( "Canvas processing",  "[canvas]") 
{
	SECTION( "Creating a canvas" )
	{
		auto canv = canvas( 20, 10 );

		REQUIRE( canv.width() == 20 );
		REQUIRE( canv.height() == 10 );

		const canvas::pixel_space& pixels = canv.pixels();
		for ( auto i = 0; i < canv.height(); i++ )
		{
			for ( auto j = 0; j < canv.width(); j++ )
			{
				REQUIRE( pixels[i][j] == f_color( 0, 0, 0 ) );
			}
		}
	}

	SECTION( "Writing a pixel to a canvas" )
	{
		auto canv = canvas( 20, 10 );
		auto red = f_color( 1, 0, 0 );

		canv.draw_pixel( 2, 3, red );

		REQUIRE( canv.pixel_at( 2, 3 ) == red );
	}
};