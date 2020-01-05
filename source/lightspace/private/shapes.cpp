#include "shapes.hpp"

namespace ls {
    f_point shape::world_to_object( const f_point& p ) const noexcept
    {
        auto pnt = p;
        if ( !_parent.expired() )
        {
            pnt = _parent.lock()->world_to_object( pnt );
        }
        return _transform.inverse() * pnt;
    }

    f_vector shape::normal_to_world( const f_vector& n ) const noexcept
    {
        auto norm = _transform.inverse().transpose() * n;
        norm.w = 0.f;
        norm = norm.normalized();
        if ( !_parent.expired() )
        {
            norm = _parent.lock()->normal_to_world( norm );
        }
        return norm;
    }

    f_vector shape::normal( fpnum x, fpnum y, fpnum z ) const noexcept
    {
        auto local_point = world_to_object( f_point( x, y, z ) );
        auto local_norm = local_normal( local_point );
        return normal_to_world( local_norm );
    }

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

        auto cyl = std::dynamic_pointer_cast<cylinder>( s );
        if ( cyl )
        {
            return intersect( cyl, r );
        }

        auto cn = std::dynamic_pointer_cast<cone>( s );
        if ( cn )
        {
            return intersect( cn, r );
        }

        auto grp = std::dynamic_pointer_cast<group>( s );
        if ( grp )
        {
            return intersect( grp, r );
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

    f_vector cube::local_normal( const f_point& p ) const
    {
        auto maxc = std::max( std::max( std::abs( p.x ), std::abs( p.y ) ), std::abs( p.z ) );
        if ( approx( maxc, std::abs( p.x ) ) )
        {
            return f_vector( p.x, 0, 0 );
        }
        else if ( approx( maxc, std::abs( p.y ) ) )
        {
            return f_vector( 0, p.y, 0 );
        }
        return f_vector( 0, 0, p.z );
    }

    intersections intersect( const cube_ptr& c, const ray& r )
    {
        const ray transformed_ray = c->transform().inverse() * r;

        auto check_axis = [] ( fpnum origin, fpnum direction ) {
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
        };

        auto origin = r.origin();
        auto direction = r.direction();
        auto xt = check_axis( origin.x, direction.x );
        auto yt = check_axis( origin.y, direction.y );
        auto zt = check_axis( origin.z, direction.z );

        auto tmin = std::max( { xt[0], yt[0], zt[0] } );
        auto tmax = std::min( { xt[1], yt[1], zt[1] } );

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

    f_vector cylinder::local_normal( const f_point& p ) const
    {
        auto dist = p.x * p.x + p.z * p.z;
        if ( dist < 1 && p.y >= max_extent_ - epsilon )
        {
            return f_vector( 0, 1, 0 );
        }
        if ( dist < 1 && p.y <= min_extent_ + epsilon )
        {
            return f_vector( 0, -1, 0 );
        }
        return f_vector( p.x, 0, p.z );
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

    f_vector cone::local_normal( const f_point& p ) const
    {
        auto dist = p.x * p.x + p.z * p.z;
        if ( dist < 1 && p.y >= max_extent_ - epsilon )
        {
            return f_vector( 0, 1, 0 );
        }
        if ( dist < 1 && p.y <= min_extent_ + epsilon )
        {
            return f_vector( 0, -1, 0 );
        }
        auto newY = std::sqrt( p.x * p.x + p.z * p.z );
        if ( p.y > 0 && !approx( p.y, 0.f ) )
        {
            newY *= -1;
        }
        return f_vector( p.x, newY, p.z );
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

    aabb_bounds group::bounds() const noexcept
    {
        aabb_bounds group_bounds( f_point( infinity, infinity, infinity ), f_point( -infinity, -infinity, -infinity ) );
        for ( auto child : children_ )
        {
            auto child_bounds = child->transform() * child->bounds();
            group_bounds.min.x = std::min( child_bounds.min.x, group_bounds.min.x );
            group_bounds.min.y = std::min( child_bounds.min.y, group_bounds.min.y );
            group_bounds.min.z = std::min( child_bounds.min.z, group_bounds.min.z );
            group_bounds.max.x = std::max( child_bounds.max.x, group_bounds.max.x );
            group_bounds.max.y = std::max( child_bounds.max.y, group_bounds.max.y );
            group_bounds.max.z = std::max( child_bounds.max.z, group_bounds.max.z );
        }

        f_point blb = _transform * f_point( group_bounds.min.x, group_bounds.min.y, group_bounds.min.z );
        f_point blf = _transform * f_point( group_bounds.min.x, group_bounds.min.y, group_bounds.max.z );
        f_point tlb = _transform * f_point( group_bounds.min.x, group_bounds.max.y, group_bounds.min.z );
        f_point tlf = _transform * f_point( group_bounds.min.x, group_bounds.max.y, group_bounds.max.z );
        f_point brb = _transform * f_point( group_bounds.max.x, group_bounds.min.y, group_bounds.min.z );
        f_point brf = _transform * f_point( group_bounds.max.x, group_bounds.min.y, group_bounds.max.z );
        f_point trb = _transform * f_point( group_bounds.max.x, group_bounds.max.y, group_bounds.min.z );
        f_point trf = _transform * f_point( group_bounds.max.x, group_bounds.max.y, group_bounds.max.z );

        group_bounds.min.x = std::min( { blb.x, blf.x, tlb.x, tlf.x, brb.x, brf.x, trb.x, trf.x } );
        group_bounds.min.y = std::min( { blb.y, blf.y, tlb.y, tlf.y, brb.y, brf.y, trb.y, trf.y } );
        group_bounds.min.z = std::min( { blb.z, blf.z, tlb.z, tlf.z, brb.z, brf.z, trb.z, trf.z } );
        group_bounds.max.x = std::max( { blb.x, blf.x, tlb.x, tlf.x, brb.x, brf.x, trb.x, trf.x } );
        group_bounds.max.y = std::max( { blb.y, blf.y, tlb.y, tlf.y, brb.y, brf.y, trb.y, trf.y } );
        group_bounds.max.z = std::max( { blb.z, blf.z, tlb.z, tlf.z, brb.z, brf.z, trb.z, trf.z } );

        return group_bounds;
    }

    intersections intersect( const group_ptr& grp, const ray& r )
    {
        const ray transformed_ray = grp->transform().inverse() * r;

        intersections itrs;
        if ( grp->bounds().intersects( r ) )
        {
            for ( auto child : grp->children() )
            {
                auto child_itrs = intersect( child, transformed_ray );
                itrs.insert( itrs.end(), child_itrs.begin(), child_itrs.end() );
            }
            std::sort( itrs.begin(), itrs.end(), [] ( intersection i1, intersection i2 ) {
                return i1.time() < i2.time();
            } );
        }
        return itrs;
    }
}
