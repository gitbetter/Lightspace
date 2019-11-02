#pragma once

#include "tensor.hpp"
#include "matrix.hpp"

namespace ls {
    class shape
    {
    public:

        using ptr = std::shared_ptr<shape>;

    public:

        shape() :
            origin_( f_point( 0, 0, 0 ) ), id_( get_uid() ), transform_( f4_matrix::identity() )
        { }
        shape( const f_point& o ) :
            origin_( o ), id_( get_uid() ), transform_( f4_matrix::identity() )
        { }
        virtual ~shape() { }

        const f_point& origin() const noexcept
        {
            return origin_;
        }

        const f4_matrix& transform() const noexcept
        {
            return transform_;
        }

        void set_transform( const f4_matrix& t ) noexcept
        {
            transform_ = t;
        }

        virtual f_vector normal( fpnum x, fpnum y, fpnum z ) const noexcept
        {
            return f_vector( 0, 0, 0 );
        }

        bool operator==( const shape& rhs ) const noexcept
        {
            return id_ == rhs.id_;
        }

        template<typename T, typename... Ts>
        static ptr create( Ts&&... args ) noexcept
        {
            return ptr( new shape( std::forward<Ts>( args ) ) );
        }

    protected:

        uint32_t id_;
        f_point origin_;
        f4_matrix transform_;

    };

    class sphere : public shape
    {
    public:

        using ptr = std::shared_ptr<sphere>;

    public:

        sphere() : shape(),
            radius_( 1 )
        { }
        sphere( const f_point& o, fpnum r ) : shape( o ),
            radius_( r )
        { }

        const fpnum radius() const noexcept
        {
            return radius_;
        }

        f_vector normal( fpnum x, fpnum y, fpnum z ) const noexcept override
        {
            auto os_point = transform_.inverse() * f_point( x, y, z );
            auto os_normal = os_point - f_point( 0, 0, 0 );
            auto ws_normal = transform_.inverse().transpose() * os_normal;
            return f_vector( ws_normal.x, ws_normal.y, ws_normal.z ).normalized();
        }

        template<typename... Ts>
        static ptr create( Ts&&... args ) noexcept
        {
            return ptr( new sphere( std::forward<Ts>( args )... ) );
        }

    private:

        fpnum radius_;

    };
}