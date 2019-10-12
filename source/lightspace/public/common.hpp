#pragma once

#include <limits>

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
		return fabs( static_cast<fpnum>( lhs ) -  static_cast<fpnum>( rhs ) ) <= epsilon;
	}

	template<typename T>
	inline T clamp( T x, T a, T b ) noexcept
	{
		return x > b ? b : ( x < a ? a : x );
	}
}