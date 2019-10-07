#include <stdexcept>
#include "canvas.hpp"

namespace ls {
	canvas::canvas( uint8_t width, uint8_t height ) :
		_width( width ), _height( height )
	{
		_pixels = pixel_space( height, std::vector<f_color>( width, f_color( 0, 0, 0 ) ) );
	}

	void canvas::draw_pixel( uint8_t x, uint8_t y, const f_color& color )
	{
		try
		{
			_pixels[x][y] = color;
		}
		catch ( const std::out_of_range& e )
		{
			std::cout << e.what() << ". Could not draw pixel at (" << x << ", " << y << ")." << std::endl;
		}
	}

	const f_color canvas::pixel_at( uint8_t x, uint8_t y ) const
	{
		try
		{
			return _pixels[x][y];
		}
		catch ( const std::out_of_range& e )
		{
			std::cout << e.what() << ". No such pixel at (" << x << ", " << y << ")." << std::endl;
			return f_color( 0, 0, 0 );
		}
	}
}