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
        s->set_material( phong_material( f_color( 0.8f, 1.f, 0.6f ), 0.1f, 0.7f, 0.2f ) );
        w->_objects.push_back( s );
        s = sphere::create();
        s->set_transform( transform::scale( 0.5f, 0.5f, 0.5f ) );
        w->_objects.push_back( s );
        return w;
    }

    f_color world::shade_hit( const intersection_state& state )
    {
        return phong_lighting( state.object->material(), _light, state.point, state.eye, state.normal );
    }

    f_color world::color_at( const ray& r )
    {
        auto itrs = intersect( shared_from_this(), r );
        auto h = hit( itrs );
        if ( h == intersection::none )
        {
            return f_color( 0, 0, 0 );
        }
        auto state = prepare_intersection_state( h, r );
        return shade_hit( state );
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