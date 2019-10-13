#pragma once

#include "common.hpp"
#include "matrix.hpp"
#include <type_traits>

namespace ls {
	namespace transform {
		template<typename T>
		const matrix<T, 4, 4> translation( T x, T y, T z )
		{
			matrix<T, 4, 4> mat{
				1, 0, 0, x,
				0, 1, 0, y,
				0, 0, 1, z,
				0, 0, 0, 1
			};
			return mat;
		}
	}
}