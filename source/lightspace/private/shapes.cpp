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
            return intersect( cb, r );
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
        auto xt = cube::check_axis( origin.x, direction.x );
        auto yt = cube::check_axis( origin.y, direction.y );
        auto zt = cube::check_axis( origin.z, direction.z );

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

    f_vector cylinder::normal( fpnum x, fpnum y, fpnum z ) const noexcept
    {
        f_vector norm( 0, 0, 0 );
        auto dist = x * x + z * z;
        if ( dist < 1 && y >= max_extent_ - epsilon )
        {
            norm = f_vector( 0, 1, 0 );
        }
        else if ( dist < 1 && y <= min_extent_ + epsilon )
        {
            norm = f_vector( 0, -1, 0 );
        }
        else
        {
            norm = f_vector( x, 0, z );
        }
        auto ws_normal = _transform.inverse().transpose() * norm;
        return f_vector( ws_normal.x, ws_normal.y, ws_normal.z ).normalized();
    }

    bool cylinder::check_cap( const ray& r, fpnum t ) noexcept
    {
        auto x = r.origin().x + t * r.direction().x;
        auto z = r.origin().z + t * r.direction().z;
        auto radius = x * x + z * z;
        return radius < 1.f || approx( radius, 1.f );
    }

    void cylinder::intersect_caps( const cylinder_ptr& cyl, const ray& r, intersections& itrs )
    {
        if ( !cyl->closed() || approx( r.direction().y, 0.f ) )
        {
            return;
        }

        auto t = ( cyl->min_extent() - r.origin().y ) / r.direction().y;
        if ( cylinder::check_cap( r, t ) )
        {
            itrs.push_back( intersection( t, cyl ) );
        }

        t = ( cyl->max_extent() - r.origin().y ) / r.direction().y;
        if ( cylinder::check_cap( r, t ) )
        {
            itrs.push_back( intersection( t, cyl ) );
        }
    }

    intersections intersect( const cylinder_ptr& cyl, const ray& r )
    {
        const ray transformed_ray = cyl->transform().inverse() * r;

        auto direction = transformed_ray.direction();
        auto origin = transformed_ray.origin();
        auto itrs = intersections();

        auto a = direction.x * direction.x + direction.z * direction.z;
        if ( approx( a, 0.f ) )
        {
            cylinder::intersect_caps( cyl, transformed_ray, itrs );
            return itrs;
        }

        auto b = 2 * origin.x * direction.x + 2 * origin.z * direction.z;
        auto c = origin.x * origin.x + origin.z * origin.z - 1;
        auto discriminant = b * b - 4 * a * c;
        if ( approx( discriminant, 0.f ) )
        {
            discriminant = 0.f;
        }

        if ( discriminant < 0 )
        {
            cylinder::intersect_caps( cyl, transformed_ray, itrs );
            return itrs;
        }

        auto denom = 1.f / ( 2.f * a );
        auto t0 = ( -b - sqrt( discriminant ) ) * denom;
        auto t1 = ( -b + sqrt( discriminant ) ) * denom;
        if ( t0 > t1 )
        {
            std::swap( t0, t1 );
        }

        auto y0 = origin.y + t0 * direction.y;
        if ( cyl->min_extent() < y0 && y0 < cyl->max_extent() )
        {
            itrs.push_back( intersection( t0, cyl ) );
        }

        auto y1 = origin.y + t1 * direction.y;
        if ( cyl->min_extent() < y1 && y1 < cyl->max_extent() )
        {
            itrs.push_back( intersection( t1, cyl ) );
        }

        cylinder::intersect_caps( cyl, transformed_ray, itrs );

        return itrs;
    }

    f_vector cone::normal( fpnum x, fpnum y, fpnum z ) const noexcept
    {
        f_vector norm( 0, 0, 0 );
        auto dist = x * x + z * z;
        if ( dist < 1 && y >= max_extent_ - epsilon )
        {
            norm = f_vector( 0, 1, 0 );
        }
        else if ( dist < 1 && y <= min_extent_ + epsilon )
        {
            norm = f_vector( 0, -1, 0 );
        }
        else
        {
            auto newY = std::sqrt( x * x + z * z );
            if ( y > 0 && !approx( y, 0.f ) )
            {
                newY *= -1;
            }
            norm = f_vector( x, newY, z );
        }
        auto ws_normal = _transform.inverse().transpose() * norm;
        return f_vector( ws_normal.x, ws_normal.y, ws_normal.z ).normalized();
    }

    bool cone::check_cap( const ray& r, fpnum radius, fpnum t ) noexcept
    {
        auto x = r.origin().x + t * r.direction().x;
        auto z = r.origin().z + t * r.direction().z;
        auto rad = x * x + z * z;
        return rad < radius || approx( rad, radius );
    }

    void cone::intersect_caps( const cone_ptr& co, const ray& r, intersections& itrs )
    {
        if ( !co->closed() || approx( r.direction().y, 0.f ) )
        {
            return;
        }

        auto t = ( co->min_extent() - r.origin().y ) / r.direction().y;
        if ( cone::check_cap( r, std::abs( co->min_extent() ), t ) )
        {
            itrs.push_back( intersection( t, co ) );
        }

        t = ( co->max_extent() - r.origin().y ) / r.direction().y;
        if ( cone::check_cap( r, std::abs( co->max_extent() ), t ) )
        {
            itrs.push_back( intersection( t, co ) );
        }
    }

    intersections intersect( const cone_ptr& co, const ray& r )
    {
        const ray transformed_ray = co->transform().inverse() * r;

        auto direction = transformed_ray.direction();
        auto origin = transformed_ray.origin();
        auto itrs = intersections();

        auto a = ( direction.x * direction.x ) - ( direction.y * direction.y ) + ( direction.z * direction.z );
        auto b = ( 2.f * origin.x * direction.x ) - ( 2.f * origin.y * direction.y ) + ( 2.f * origin.z * direction.z );
        auto c = ( origin.x * origin.x ) - ( origin.y * origin.y ) + ( origin.z * origin.z );

        if ( approx( a, 0.f ) && approx( b, 0.f ) )
        {
            cone::intersect_caps( co, transformed_ray, itrs );
            return itrs;
        }
        else if ( approx( a, 0.f ) )
        {
            itrs.push_back( intersection( -c / ( 2.f * b ), co ) );
            cone::intersect_caps( co, transformed_ray, itrs );
            return itrs;
        }

        auto discriminant = b * b - 4 * a * c;
        if ( approx( discriminant, 0.f ) )
        {
            discriminant = 0.f;
        }

        if ( discriminant < 0 )
        {
            cone::intersect_caps( co, transformed_ray, itrs );
            return itrs;
        }

        auto denom = 1.f / ( 2.f * a );
        auto disc_sqrt = std::sqrt( discriminant );
        auto t0 = ( -b - disc_sqrt ) * denom;
        auto t1 = ( -b + disc_sqrt ) * denom;
        if ( t0 > t1 )
        {
            std::swap( t0, t1 );
        }

        auto y0 = origin.y + t0 * direction.y;
        if ( co->min_extent() < y0 && y0 < co->max_extent() )
        {
            itrs.push_back( intersection( t0, co ) );
        }

        auto y1 = origin.y + t1 * direction.y;
        if ( co->min_extent() < y1 && y1 < co->max_extent() )
        {
            itrs.push_back( intersection( t1, co ) );
        }

        cone::intersect_caps( co, transformed_ray, itrs );

        return itrs;
    }

    void group::add_child( const shape_ptr shape ) noexcept
    {
        if ( std::find( children_.begin(), children_.end(), shape ) == children_.end() )
        {
            children_.push_back( shape );
        }
        shape->set_parent( std::static_pointer_cast<group>( shared_from_this() ) );
    }
}
