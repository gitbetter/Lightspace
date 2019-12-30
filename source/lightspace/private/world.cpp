#include "world.hpp"
#include "lights.hpp"
#include "ray.hpp"

namespace ls {
    void world::add_object( shape_ptr obj )
    {
        bool present = false;
        for ( auto object : _objects )
        {
            if ( object == obj )
            {
                present = true;
                break;
            }
        }
        if ( !present )
        {
            _objects.push_back( obj );
        }
    }

    void world::remove_object( shape_ptr obj )
    {
        auto it = _objects.begin();
        for ( ; it != _objects.end(); it++ )
        {
            if ( *it == obj )
            {
                break;
            }
        }
        if ( it != _objects.end() )
        {
            _objects.erase( it );
        }
    }

    bool world::contains( const shape_ptr& s ) const
    {
        for ( const auto& object : _objects )
        {
            if ( s->identical_to( object ) )
            {
                return true;
            }
        }
        return false;
    }

    world_ptr world::create_default() noexcept
    {
        auto w = world::create();
        w->_light = point_light::create( f_color( 1, 1, 1 ), f_point( -10, 10, -10 ) );
        auto s = sphere::create();
        s->set_material( phong_material::create( f_color( 0.8f, 1.f, 0.6f ), 0.1f, 0.7f, 0.2f ) );
        w->_objects.push_back( s );
        s = sphere::create();
        s->set_transform( transform::scale( 0.5f, 0.5f, 0.5f ) );
        w->_objects.push_back( s );
        return w;
    }

    f_color world::shade_hit( const intersection_state& state, uint8_t depth )
    {
        auto shadowed = in_shadow( state.shifted_point );
        auto surface = phong_lighting( state.object, state.object->material(), _light, state.shifted_point, state.eye, state.normal, shadowed );
        auto reflected = reflected_color( state, depth );
        auto refracted = refracted_color( state, depth );
        return surface + reflected + refracted;
    }

    f_color world::reflected_color( const intersection_state& state, uint8_t depth ) {
        if ( approx( state.object->material()->reflectivity, 0.f ) || depth <= 0 )
        {
            return f_color( 0, 0, 0 );
        }
        auto reflected_ray = ray( state.shifted_point, state.reflection );
        auto col = color_at( reflected_ray, depth - 1 );
        return col * state.object->material()->reflectivity;
    }

    f_color world::refracted_color( const intersection_state& state, uint8_t depth ) {
        if ( approx( state.object->material()->transparency, 0.f ) || depth <= 0 )
        {
            return f_color( 0, 0, 0 );
        }
        // Check for total internal reflection using Snell's Law and a trig identity
        auto ratio = state.ridx_from / state.ridx_to;
        auto cos_i = state.eye.dot( state.normal );
        auto sin2_t = (ratio * ratio) * ( 1 - cos_i * cos_i );
        if ( sin2_t > 1 )
        {
            return f_color( 0, 0, 0 );
        }
        // Get the new color using the refracted ray
        auto cos_t = sqrt( 1.f - sin2_t );
        auto direction = state.normal * ( ratio * cos_i - cos_t ) - state.eye * ratio;
        auto refracted_ray = ray( state.shifted_under_point, direction );
        auto col = color_at( refracted_ray, depth - 1 );
        return col * state.object->material()->transparency;
    }

    f_color world::color_at( const ray& r, uint8_t depth )
    {
        auto itrs = intersect( shared_from_this(), r );
        auto h = hit( itrs );
        if ( h == intersection::none )
        {
            return f_color( 0, 0, 0 );
        }
        auto state = prepare_intersection_state( h, r );
        return shade_hit( state, depth );
    }

    bool world::in_shadow( const f_point& p )
    {
        auto to_light = _light->position() - p;
        auto dist = to_light.length();
        auto dir = to_light.normalized();

        auto r = ray( p, dir );
        auto itrs = intersect( shared_from_this(), r );
        auto h = hit( itrs );

        return h != intersection::none && h.time() < dist;
    }

    intersections intersect( const world_ptr& w, const ray& r )
    {
        intersections itrs;
        for ( const shape_ptr& object : w->objects() )
        {
            auto object_itrs = intersect( object, r );
            itrs.insert( itrs.end(), object_itrs.begin(), object_itrs.end() );
        }
        std::sort( itrs.begin(), itrs.end(), [] ( intersection i1, intersection i2 ) {
            return i1.time() < i2.time();
        } );
        return itrs;
    }
}
