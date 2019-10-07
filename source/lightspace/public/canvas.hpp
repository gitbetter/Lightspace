#include <vector>
#include "tensor.hpp"

namespace ls {
	class canvas
	{
	public:

		using pixel_space = std::vector<std::vector<f_color>>;

		canvas( uint8_t width, uint8_t height );

		const uint8_t width() const noexcept
		{
			return _width;
		}

		const uint8_t height() const noexcept
		{
			return _height;
		}

		const pixel_space& pixels() const noexcept
		{
			return _pixels;
		}

		void draw_pixel( uint8_t x, uint8_t y, const f_color& color );

		const f_color pixel_at( uint8_t x, uint8_t y ) const;

	private:

		uint8_t _width{ 0 };
		uint8_t _height{ 0 };
		pixel_space _pixels;

	};
}