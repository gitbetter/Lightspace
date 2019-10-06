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
}