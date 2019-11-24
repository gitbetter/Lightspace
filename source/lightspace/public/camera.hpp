#pragma once

#include "common.hpp"
#include "canvas.hpp"
#include "transform.hpp"
#include "ray.hpp"

namespace ls {
    class camera
    {
    public:

        camera( uint16_t width, uint16_t height, fpnum fov );

        uint16_t width() const noexcept
        {
            return _width;
        }

        uint16_t height() const noexcept
        {
            return _height;
        }

        fpnum pixel_size() const noexcept
        {
            return _pixel_size;
        }

        fpnum aspect() const noexcept
        {
            return _aspect;
        }

        fpnum field_of_view() const noexcept
        {
            return _field_of_view;
        }

        const f4_matrix& transform() const noexcept
        {
            return _transform;
        }

        void set_transform( const f4_matrix& m ) noexcept
        {
            _transform = m;
        }

        ray ray_for_pixel( uint16_t x, uint16_t y ) const;

        canvas render( const world_ptr& w ) const;

        PTR_FACTORY( camera )

    protected:

        void calculate_auxiliary_values();

    private:

        uint16_t _width;
        uint16_t _height;
        fpnum _half_width;
        fpnum _half_height;
        fpnum _field_of_view;
        fpnum _pixel_size;
        fpnum _aspect;
        f4_matrix _transform;

    };
}