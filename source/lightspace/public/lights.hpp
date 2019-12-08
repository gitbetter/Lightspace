#pragma once

#include "common.hpp"
#include "tensor.hpp"
#include "materials.hpp"

namespace ls {
    class light
    {
    public:

        light( const f_color& intensity, const f_point& position ) :
            _intensity( intensity ), _position( position )
        { }
        virtual ~light() 
        { }

        const f_color intensity() const noexcept
        {
            return _intensity;
        }

        void set_intensity( const f_color& intensity ) noexcept
        {
            _intensity = intensity;
        }

        const f_point position() const noexcept
        {
            return _position;
        }

        void set_position( const f_point& position ) noexcept
        {
            _position = position;
        }

        bool operator==( const light& rhs ) const noexcept
        {
            return _intensity == rhs._intensity && _position == rhs._position;
        }

        PTR_FACTORY( light )

    private:

        f_color _intensity;
        f_point _position;

    };

    class point_light : public light
    {
    public:

        point_light( const f_color& intensity, const f_point& position ) :
            light( intensity, position )
        { }

        PTR_FACTORY( point_light )

    };

    f_color phong_lighting( const phong_material_ptr& mat, const light_ptr& l, const f_point& position, const f_vector& eye, const f_vector& normal, bool in_shadow = false );
}