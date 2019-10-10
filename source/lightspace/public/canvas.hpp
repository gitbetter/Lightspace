#pragma once

#include <vector>
#include "tensor.hpp"

namespace ls {
	static constexpr uint16_t max_ppm_line_width = 70;

	class canvas
	{
	public:

		using pixel_space = std::vector<std::vector<f_color>>;

		canvas( uint16_t width, uint16_t height );

		inline const uint16_t width() const noexcept
		{
			return _width;
		}

		inline const uint16_t height() const noexcept
		{
			return _height;
		}

		inline const pixel_space& pixels() const noexcept
		{
			return _pixels;
		}

		void draw_pixel( uint16_t x, uint16_t y, const f_color& color );

		const f_color pixel_at( uint16_t x, uint16_t y ) const;

		const std::string to_ppm() const;

		void write_to( const std::string& file ) const;

	private:

		uint16_t _width{ 0 };
		uint16_t _height{ 0 };
		pixel_space _pixels;

	private:

		inline bool within_bounds( const uint16_t& x, const uint16_t& y ) const noexcept
		{
			return x >= 0 && x < _width && y >= 0 && y < _height;
		}

		void append_ppm_header( std::stringstream& os ) const;
		
		void append_ppm_pixel_data( std::stringstream& os ) const;

	};
}