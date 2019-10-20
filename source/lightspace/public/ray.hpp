#pragma once

#include "tensor.hpp"

namespace ls {
	class ray
	{
	public:

		ray( const f_point& o, const f_vector& d ) :
			origin_(o), direction_(d)
		{  }

		const f_point position( fpnum t ) const noexcept
		{
			return origin_ + direction_ * t;
		}

		const f_point& origin() const noexcept
		{
			return origin_;
		}

		const f_vector& direction() const noexcept
		{
			return direction_;
		}

	private:

		f_point origin_;
		f_vector direction_;

	};
}