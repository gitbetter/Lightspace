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
        
        auto cb = std::dynamic_pointer_cast<cube>( s );
        if ( cb )
        {
            return intersect( cb , r );
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

    f_vector cube::normal( fpnum x, fpnum y, fpnum z ) const noexcept
    {
        auto maxc = std::max( std::max( std::abs( x ), std::abs( y ) ), std::abs( z ) );
        if ( approx( maxc, std::abs( x ) ) )
        {
            auto n = _transform.inverse().transpose() * f_vector( x, 0, 0 );
            return n.normalized();
        }
        else if ( approx( maxc, std::abs( y ) ) )
        {
            auto n = _transform.inverse().transpose() * f_vector( 0, y, 0 );
            return n.normalized();
        }
        auto n = _transform.inverse().transpose() * f_vector( 0, 0, z );
        return n.normalized();
    }

    std::array<fpnum, 2> cube::check_axis( fpnum origin, fpnum direction )
    {
        auto tmin_numerator = -1.f - origin;
        auto tmax_numerator = 1.f - origin;
        
        fpnum tmin, tmax;
        if ( abs( direction ) >= epsilon )
        {
            tmin = tmin_numerator / direction;
            tmax = tmax_numerator / direction;
        }
        else
        {
            tmin = tmin_numerator * infinity;
            tmax = tmax_numerator * infinity;
        }
        
        return tmin > tmax ? std::array<fpnum, 2>{tmax, tmin} : std::array<fpnum, 2>{tmin, tmax};
    }

    intersections intersect( const cube_ptr& c, const ray& r )
    {
        const ray transformed_ray = c->transform().inverse() * r;
        
        auto origin = r.origin();
        auto direction = r.direction();
        auto xt = cube::check_axis( origin.x , direction.x );
        auto yt = cube::check_axis( origin.y , direction.y );
        auto zt = cube::check_axis( origin.z , direction.z );
        
        auto tmin = std::max( std::max( xt[0], yt[0] ), zt[0] );
        auto tmax = std::min( std::min( xt[1], yt[1] ), zt[1] );
        
        if ( tmin > tmax )
        {
            return intersections();
        }
        else
        {
            return intersections{
                intersection( tmin, c ),
                intersection( tmax, c )
            };
        }
    }
}
