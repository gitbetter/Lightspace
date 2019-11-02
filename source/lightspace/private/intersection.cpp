#include "intersection.hpp"
#include <algorithm>

namespace ls {
    const intersection intersection::none = intersection();

    intersections intersect( const sphere::ptr& s, const ray& r )
    {
        const ray transformed_ray = s->transform().inverse() * r;

        f_vector sphere_to_ray = transformed_ray.origin() - s->origin();
        f_vector ray_direction = transformed_ray.direction();

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

    intersection hit( const intersections& itrs )
    {
        auto itrsSorted = itrs;
        std::sort( itrsSorted.begin(), itrsSorted.end(), [] ( intersection i1, intersection i2 ) {
            return i1.time() < i2.time();
        } );
        auto it = itrsSorted.cbegin();
        while ( it != itrsSorted.cend() && it->time() < 0 ) ++it;
        return it == itrsSorted.cend() ? intersection::none : *it;
    }
}