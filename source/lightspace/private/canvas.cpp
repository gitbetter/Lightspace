#include <fstream>
#include <sstream>
#include "canvas.hpp"

namespace ls {
	canvas::canvas( uint16_t width, uint16_t height ) :
		_width( width ), _height( height )
	{
		_pixels = pixel_space( height, std::vector<f_color>( width, f_color( 0, 0, 0 ) ) );
	}

	void canvas::draw_pixel( uint16_t x, uint16_t y, const f_color& color )
	{
		if ( !within_bounds( x, y ) )
		{
			std::cout << ". Could not draw pixel at (" << x << ", " << y << ")." << std::endl;
			return;
		}
		_pixels[y][x] = color;
	}

	const f_color canvas::pixel_at( uint16_t x, uint16_t y ) const
	{
		if ( !within_bounds( x, y ) )
		{
			std::cout << ". No such pixel at (" << x << ", " << y << ")." << std::endl;
			return f_color( 0, 0, 0 );
		}
		return _pixels[y][x];
	}

	const std::string canvas::to_ppm() const
	{
		std::stringstream stream;
		append_ppm_header( stream );
		append_ppm_pixel_data( stream );
		return stream.str();
	}

	void canvas::write_to( const std::string& file ) const
	{
		std::ofstream out;
		out.exceptions( std::ios::badbit | std::ios::failbit );
		try
		{
			out.open( file, std::fstream::out | std::fstream::trunc );
			out << to_ppm();
			out.close();
		}
		catch ( const std::exception& e )
		{
			std::cout << "Could not open file at " << file << " for writing: " << e.what() << std::endl;
		}
	}

	void canvas::append_ppm_header( std::stringstream& os ) const
	{
		os << "P3" << std::endl;
		os << static_cast<unsigned int>(_width) << " " << static_cast<unsigned int>(_height) << std::endl;
		os << "255" << std::endl;
	}

	void canvas::append_ppm_pixel_data( std::stringstream& os ) const
	{
		size_t line_width = 0;
		auto print_color_component = [&os, &line_width] ( const std::string& s, const char& delim = ' ' ) {
			line_width += s.size() + 1;
			if ( static_cast<uint16_t>( line_width ) >= max_ppm_line_width )
			{
				if ( os.seekg( -1, std::ios_base::end ).get() == ' ' )
				{
					os.seekp( -1, std::ios_base::end );
				}
				os << std::endl;
				line_width = s.size() + 1;
			}
			os << s << delim;
		};

		for ( auto i = 0; i < _height; i++ )
		{
			for ( auto j = 0; j < _width; j++ )
			{
				const f_color& pixel = _pixels[i][j];
				uint16_t line_width = 0;

				auto r = std::to_string( static_cast<unsigned int>( clamp( pixel.r * 256, 0.f, 255.f ) ) );
				print_color_component( r );
				auto g = std::to_string( static_cast<unsigned int>( clamp( pixel.g * 256, 0.f, 255.f ) ) );
				print_color_component( g );
				auto b = std::to_string( static_cast<unsigned int>( clamp( pixel.b * 256, 0.f, 255.f ) ) );
				print_color_component( b, ( j == _width - 1 ) ? '\n' : ' ' );
			}
			line_width = 0;
		}
	}
}