#include "intersection.hpp"

namespace ls {
	intersections intersect( const sphere& s, const ray& r )
	{
		f_vector sphere_to_ray = r.origin() - s.origin();
		f_vector ray_direction = r.direction();

		fpnum a = ray_direction.dot( ray_direction );
		fpnum b = 2 * ray_direction.dot( sphere_to_ray );
		fpnum c = sphere_to_ray.dot( sphere_to_ray ) - 1;

		fpnum discriminant = ( b * b ) - 4 * a * c;

		// If the discrimant is zero, the ray did not hit the sphere
		if ( discriminant < 0 )
		{
			return intersections();
		}

		// Precompute these values since divisions and square roots are
		// expensive operations
		fpnum discriminant_sqrt = std::sqrt( discriminant );
		fpnum denom = 1 / ( 2 * a );

		return intersections{
			intersection( ( -b - discriminant_sqrt ) * denom, s ),
			intersection( ( -b + discriminant_sqrt ) * denom, s )
		};
	}
}