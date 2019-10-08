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

	SECTION( "Constructing the PPM header" )
	{
		auto canv = canvas( 20, 10 );
		auto ppm = canv.to_ppm();

		REQUIRE( ppm.find( "P3\n20 10\n255" ) == 0 );
	}

	SECTION( "Constructing the PPM pixel data" )
	{
		auto canv = canvas( 5, 3 );
		auto color1 = f_color( 1.5f, 0, 0 );
		auto color2 = f_color( 0, 0.5f, 0 );
		auto color3 = f_color( -0.5f, 0, 1 );

		canv.draw_pixel( 0, 0, color1 );
		canv.draw_pixel( 1, 2, color2 );
		canv.draw_pixel( 2, 4, color3 );

		auto ppm = canv.to_ppm();
		std::string expected_ppm{
			"P3\n"
			"5 3\n"
			"255\n"
			"255 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n"
			"0 0 0 0 0 0 0 128 0 0 0 0 0 0 0\n"
			"0 0 0 0 0 0 0 0 0 0 0 0 0 0 255\n" 
		};

		REQUIRE( ppm == expected_ppm );
	}

	SECTION( "Splitting long lines in PPM files" )
	{
		auto canv = canvas( 10, 2 );
		auto pixel_color = f_color( 1.5f, 0.8f, 0.6f );
	
		for ( auto i = 0; i < canv.height(); i++ )
		{
			for ( auto j = 0; j < canv.width(); j++ )
			{
				canv.draw_pixel( i, j, pixel_color );
			}
		}

		auto ppm = canv.to_ppm();

		std::string expected_ppm{
			"P3\n"
			"10 2\n"
			"255\n"
			"255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204\n"
			"153 255 204 153 255 204 153 255 204 153 255 204 153\n"
			"255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204\n"
			"153 255 204 153 255 204 153 255 204 153 255 204 153\n"
		};

		CAPTURE( ppm.size() );
		CAPTURE( expected_ppm.size() );

		REQUIRE( ppm == expected_ppm );
	}

	SECTION( "PPM files are terminated by a newline character" )
	{
		auto canv = canvas( 5, 3 );
		auto ppm = canv.to_ppm();

		REQUIRE( ppm.back() == '\n' );
	}
};