#pragma once

#include "shapes.hpp"
#include "ray.hpp"
#include <vector>

namespace ls {
	class intersection
	{
	public:

		intersection( fpnum t, const shape& s ) :
			time_( t ), object_( s )
		{ }

		const fpnum time() const noexcept
		{
			return time_;
		}

		const shape& object() const noexcept
		{
			return object_;
		}

	protected:

		fpnum time_;
		shape object_;

	};

	using intersections = std::vector<intersection>;

	intersections intersect( const sphere& s, const ray& r );
}