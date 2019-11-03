#pragma once

#include "common.hpp"
#include "tensor.hpp"

namespace ls {
    class light
    {
    public:

        using ptr = std::shared_ptr<light>;

    public:

        light( const f_color& intensity, const f_point& position ) :
            intensity_( intensity ), position_( position )
        { }
        virtual ~light() 
        { }

        const f_color intensity() const noexcept
        {
            return intensity_;
        }

        void set_intensity( const f_color& intensity ) noexcept
        {
            intensity_ = intensity;
        }

        const f_point position() const noexcept
        {
            return position_;
        }

        void set_position( const f_point& position ) noexcept
        {
            position_ = position;
        }

    private:

        f_color intensity_;
        f_point position_;

    };

    class point_light : public light
    {
    public:

        using ptr = std::shared_ptr<point_light>;

    public:

        point_light( const f_color& intensity, const f_point& position ) :
            light( intensity, position )
        { }

    };
}