#include "world.hpp"

namespace ls {
    bool world::contains( const shape::ptr& s ) const
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

     world::ptr world::create_default() noexcept
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
}