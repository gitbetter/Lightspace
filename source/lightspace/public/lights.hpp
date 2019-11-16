#pragma once

#include "common.hpp"
#include "tensor.hpp"
#include "materials.hpp"

namespace ls {
    class light
    {
    public:

        using ptr = std::shared_ptr<light>;

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

        template<typename... Ts>
        static ptr create( Ts&&... args ) noexcept
        {
            return ptr( new shape( std::forward<Ts>( args ) ) );
        }

    private:

        f_color _intensity;
        f_point _position;

    };

    class point_light : public light
    {
    public:

        using ptr = std::shared_ptr<point_light>;

    public:

        point_light( const f_color& intensity, const f_point& position ) :
            light( intensity, position )
        { }

        template<typename... Ts>
        static ptr create( Ts&&... args ) noexcept
        {
            return ptr( new point_light( std::forward<Ts>( args )... ) );
        }

    };

    f_color phong_lighting( const phong_material& mat, const light& l, const f_point& position, const f_vector& eye, const f_vector& normal );
}