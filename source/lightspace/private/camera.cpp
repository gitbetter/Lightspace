#include "camera.hpp"
#include "world.hpp"

namespace ls {
    camera::camera( uint16_t width, uint16_t height, fpnum fov ) : 
        _width( width ), _height( height ), _field_of_view( fov ), _transform( f4_matrix::identity() )
    { 
        calculate_auxiliary_values();
    }

    void camera::calculate_auxiliary_values()
    {
        fpnum half_view = tanf( _field_of_view * 0.5f );
        _aspect = static_cast<fpnum>( _width ) / static_cast<fpnum>( _height );
        _half_width = _aspect >= 1 ? half_view : half_view * _aspect;
        _half_height = _aspect >= 1 ? half_view / _aspect : half_view;
        _pixel_size = ( static_cast<fpnum>( _half_width ) * 2.f )  / static_cast<fpnum>( _width );
    }

    ray camera::ray_for_pixel( uint16_t x, uint16_t y ) const
    {
        auto x_offset = ( x + 0.5f ) * _pixel_size;
        auto y_offset = ( y + 0.5f ) * _pixel_size;

        auto world_x = _half_width - x_offset;
        auto world_y = _half_height - y_offset;

        auto pixel = _transform.inverse() * f_point( world_x, world_y, -1 );
        auto origin = _transform.inverse() * f_point( 0, 0, 0 );
        auto direction = ( pixel - origin ).normalized();

        return ray( origin, direction );
    }

    canvas camera::render( const world_ptr& w ) const
    {
        auto image = canvas( _width, _height );
        for ( auto i = 0; i < _width; i++ )
        {
            for ( auto j = 0; j < _height; j++ )
            {
                auto ray = ray_for_pixel( i, j );
                auto color = w->color_at( ray );
                image.draw_pixel( i, j, color );
            }
        }
        return image;
    }
}