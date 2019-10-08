#include <vector>
#include "tensor.hpp"

namespace ls {
	static constexpr uint16_t max_ppm_line_width = 70;

	class canvas
	{
	public:

		using pixel_space = std::vector<std::vector<f_color>>;

		canvas( uint16_t width, uint16_t height );

		const uint16_t width() const noexcept
		{
			return _width;
		}

		const uint16_t height() const noexcept
		{
			return _height;
		}

		const pixel_space& pixels() const noexcept
		{
			return _pixels;
		}

		void draw_pixel( uint16_t x, uint16_t y, const f_color& color );

		const f_color pixel_at( uint16_t x, uint16_t y ) const;

		const std::string to_ppm() const;

	private:

		uint16_t _width{ 0 };
		uint16_t _height{ 0 };
		pixel_space _pixels;

	private:

		void append_ppm_header( std::stringstream& os ) const;
		
		void append_ppm_pixel_data( std::stringstream& os ) const;

	};
}