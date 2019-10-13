#pragma once

#include <limits>
#include <stdexcept>
#include <type_traits>

#if DOUBLE_PRECISION
using fpnum = double;
#else
using fpnum = float;
#endif

namespace ls {
	static constexpr float epsilon = std::numeric_limits<fpnum>::epsilon() * 100;
	static constexpr float infinity = std::numeric_limits<fpnum>::infinity();

	template<
		typename T,
		typename = std::enable_if_t<std::is_arithmetic<T>::value>
	>
	inline bool approx( const T& lhs, const T& rhs ) noexcept {
		return fabs( static_cast<fpnum>( lhs ) - static_cast<fpnum>( rhs ) ) <= epsilon;
	}

	template<typename T>
	inline T clamp( T x, T a, T b ) noexcept
	{
		return x > b ? b : ( x < a ? a : x );
	}

	template<
		typename T,
		typename = std::enable_if_t<std::is_arithmetic<T>::value>
	>
	inline uint8_t sign( const T& num ) noexcept
	{
		return num < 0 ? -1 : 1;
	}

	class method_not_supported : public std::exception
	{
	public:

		const char* what() const throw()
		{ 
			return "This method is not supported for the given class instantiation."; 
		}

	};
}