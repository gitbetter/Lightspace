#include "shapes.hpp"

namespace ls {
    intersections intersect( const shape_ptr& s, const ray& r )
    {
        auto sph = std::dynamic_pointer_cast<sphere>( s );
        if ( sph )
        {
            return intersect( sph, r );
        }

        auto pln = std::dynamic_pointer_cast<plane>( s );
        if ( pln )
        {
            return intersect( pln, r );
        }

        return intersections();
    }

    sphere_ptr sphere::create_glassy()
    {
        auto sph = sphere::create();
        sph->material()->transparency = 1.f;
        sph->material()->refractive_index = 1.5f;
        return sph;
    }

    intersections intersect( const sphere_ptr& s, const ray& r )
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

    intersections intersect( const plane_ptr& p, const ray& r )
    {
        const ray transformed_ray = p->transform().inverse() * r;
        
        if ( abs( transformed_ray.direction().y ) < epsilon )
        {
            return intersections();
        }

        return intersections{
            intersection( -transformed_ray.origin().y / transformed_ray.direction().y, p )
        };
    }
}
