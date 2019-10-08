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

	inline bool approx( const fpnum& lhs, const fpnum& rhs ) noexcept {
		return fabs( lhs - rhs ) <= epsilon;
	}

	template<typename T>
	inline T clamp( T x, T a, T b ) noexcept
	{
		return x > b ? b : ( x < a ? a : x );
	}
}