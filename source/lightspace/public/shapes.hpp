#pragma once

#include "tensor.hpp"
#include "matrix.hpp"
#include "materials.hpp"
#include "intersection.hpp"

namespace ls {
    class shape
    {
    public:

        shape() :
            _id( get_uid() ), _origin( f_point( 0, 0, 0 ) ), _transform( f4_matrix::identity() ), _mat( phong_material::create() )
        { }
        explicit shape( const f_point& o ) :
            _id( get_uid() ), _origin( o ), _transform( f4_matrix::identity() ), _mat( phong_material::create() )
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

        const phong_material_ptr& material() const noexcept
        {
            return _mat;
        }

        void set_material( const phong_material_ptr& mat ) noexcept
        {
            _mat = mat;
        }

        virtual f_vector normal( fpnum x, fpnum y, fpnum z ) const noexcept
        {
            return f_vector( 0, 0, 0 );
        }

        virtual bool identical_to( const shape_ptr other ) const noexcept
        {
            return other && _origin == other->_origin && _transform == other->_transform && *_mat == *( other->_mat );
        }

        bool operator==( const shape& rhs ) const noexcept
        {
            return _id == rhs._id;
        }

        PTR_FACTORY( shape )

    protected:

        uint32_t _id;
        f_point _origin;
        f4_matrix _transform;
        phong_material_ptr _mat;

    };

    intersections intersect( const shape_ptr& s, const ray& r );

    class sphere : public shape
    {
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

        bool identical_to( const shape_ptr other ) const noexcept override
        {
            auto other_sphere = std::dynamic_pointer_cast<sphere>( other );
            if ( !other_sphere )
            {
                return false;
            }
            return _radius == other_sphere->_radius && shape::identical_to( other );
        }
        
        static sphere_ptr create_glassy();

        PTR_FACTORY( sphere )

    private:

        fpnum _radius;

    };

    intersections intersect( const sphere_ptr& s, const ray& r );

    class plane : public shape
    {
    public:

        plane() : shape()
        { }

        f_vector normal( fpnum x, fpnum y, fpnum z ) const noexcept override
        {
            auto ws_normal = _transform.inverse().transpose() * f_vector( 0, 1, 0 );
            return f_vector( ws_normal.x, ws_normal.y, ws_normal.z ).normalized();
        }

        PTR_FACTORY( plane )
    };

    intersections intersect( const plane_ptr& p, const ray& r );

    class cube : public shape
    {
    public:

        cube() : shape()
        { }

        f_vector normal( fpnum x, fpnum y, fpnum z ) const noexcept override;
        
        static std::array<fpnum, 2> check_axis( fpnum origin, fpnum direction );

        PTR_FACTORY( cube )
    };

    intersections intersect( const cube_ptr& c, const ray& r );
}
