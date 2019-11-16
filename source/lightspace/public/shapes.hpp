#pragma once

#include "tensor.hpp"
#include "matrix.hpp"
#include "materials.hpp"

namespace ls {
    class shape
    {
    public:

        using ptr = std::shared_ptr<shape>;

    public:

        shape() :
            _origin( f_point( 0, 0, 0 ) ), _id( get_uid() ), _transform( f4_matrix::identity() )
        { }
        explicit shape( const f_point& o ) :
            _origin( o ), _id( get_uid() ), _transform( f4_matrix::identity() )
        { }
        virtual ~shape() { }

        const f_point& origin() const noexcept
        {
            return _origin;
        }

        const f4_matrix& transform() const noexcept
        {
            return _transform;
        }

        void set_transform( const f4_matrix& t ) noexcept
        {
            _transform = t;
        }

        const phong_material& material() const noexcept
        {
            return _mat;
        }

        void set_material( const phong_material& mat ) noexcept
        {
            _mat = mat;
        }

        virtual f_vector normal( fpnum x, fpnum y, fpnum z ) const noexcept
        {
            return f_vector( 0, 0, 0 );
        }

        virtual bool identical_to( const shape::ptr other ) const noexcept
        {
            return other && _origin == other->_origin && _transform == other->_transform && _mat == other->_mat;
        }

        bool operator==( const shape& rhs ) const noexcept
        {
            return _id == rhs._id;
        }

        template<typename... Ts>
        static ptr create( Ts&&... args ) noexcept
        {
            return ptr( new shape( std::forward<Ts>( args ) ) );
        }

    protected:

        uint32_t _id;
        f_point _origin;
        f4_matrix _transform;
        phong_material _mat;

    };

    class sphere : public shape
    {
    public:

        using ptr = std::shared_ptr<sphere>;

    public:

        sphere() : shape(),
            _radius( 1 )
        { }
        sphere( const f_point& o, fpnum r ) : shape( o ),
            _radius( r )
        { }

        const fpnum radius() const noexcept
        {
            return _radius;
        }

        f_vector normal( fpnum x, fpnum y, fpnum z ) const noexcept override
        {
            auto os_point = _transform.inverse() * f_point( x, y, z );
            auto os_normal = os_point - f_point( 0, 0, 0 );
            auto ws_normal = _transform.inverse().transpose() * os_normal;
            return f_vector( ws_normal.x, ws_normal.y, ws_normal.z ).normalized();
        }

        bool identical_to( const shape::ptr other ) const noexcept override
        {
            auto other_sphere = std::dynamic_pointer_cast<sphere>( other );
            if ( !other_sphere )
            {
                return false;
            }
            return _radius == other_sphere->_radius && shape::identical_to( other );
        }

        template<typename... Ts>
        static ptr create( Ts&&... args ) noexcept
        {
            return ptr( new sphere( std::forward<Ts>( args )... ) );
        }

    private:

        fpnum _radius;

    };
}