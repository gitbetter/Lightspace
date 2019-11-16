#include "intersection.hpp"
#include <algorithm>

namespace ls {
    const intersection intersection::none = intersection();

    intersections intersect( const shape::ptr& s, const ray& r )
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

    intersections intersect( const world::ptr& w, const ray& r )
    {
        intersections itrs;
        for ( const shape::ptr& object : w->objects() )
        {
            auto object_itrs = intersect( object, r );
            itrs.insert( itrs.end(), object_itrs.begin(), object_itrs.end() );
        }
        std::sort( itrs.begin(), itrs.end(), [] ( intersection i1, intersection i2 ) {
            return i1.time() < i2.time();
        } );
        return itrs;
    }

    intersection hit( const intersections& itrs )
    {
        auto itrs_sorted = itrs;
        std::sort( itrs_sorted.begin(), itrs_sorted.end(), [] ( intersection i1, intersection i2 ) {
            return i1.time() < i2.time();
        } );
        auto it = itrs_sorted.cbegin();
        while ( it != itrs_sorted.cend() && it->time() < 0 ) ++it;
        return it == itrs_sorted.cend() ? intersection::none : *it;
    }
}