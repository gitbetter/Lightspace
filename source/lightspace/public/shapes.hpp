#pragma once

#include "tensor.hpp"
#include "matrix.hpp"
#include "materials.hpp"
#include "intersection.hpp"

namespace ls {
    class shape : public std::enable_shared_from_this<shape>
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

        group_ptr parent() const noexcept
        {
            if ( !_parent.expired() )
            {
                return _parent.lock();
            }
            return nullptr;
        }

        void set_parent( const group_ptr& p ) noexcept
        {
            _parent.reset();
            _parent = group_ptr_weak( p );
        }

        f_point world_to_object( const f_point& p ) const noexcept;

        f_vector normal_to_world( const f_vector& n ) const noexcept;

        f_vector normal( fpnum x, fpnum y, fpnum z ) const noexcept;

        virtual bool identical_to( const shape_ptr other ) const noexcept
        {
            return other && _origin == other->_origin && _transform == other->_transform && *_mat == *( other->_mat );
        }

        virtual aabb_bounds bounds() const noexcept
        {
            return aabb_bounds( f_point( 0, 0, 0 ), f_point( 0, 0, 0 ) );
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
        group_ptr_weak _parent;

    protected:

        virtual f_vector local_normal( const f_point& p ) const
        {
            return f_vector( 0, 0, 0 );
        }

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

        bool identical_to( const shape_ptr other ) const noexcept override
        {
            auto other_sphere = std::dynamic_pointer_cast<sphere>( other );
            if ( !other_sphere )
            {
                return false;
            }
            return _radius == other_sphere->_radius && shape::identical_to( other );
        }

        aabb_bounds bounds() const noexcept override
        {
            return aabb_bounds( f_point( -_radius, -_radius, -_radius ), f_point( _radius, _radius, _radius ) );
        }
        
        static sphere_ptr create_glassy();

        PTR_FACTORY( sphere )

    private:

        fpnum _radius;

    private:

        f_vector local_normal( const f_point& p ) const override
        {
            return p - f_point( 0, 0, 0 );
        }

    };

    intersections intersect( const sphere_ptr& s, const ray& r );

    class plane : public shape
    {
    public:

        plane() : shape()
        { }

        aabb_bounds bounds() const noexcept override
        {
            return aabb_bounds( f_point( -infinity, 0, -infinity ), f_point( infinity, 0, infinity ) );
        }

        PTR_FACTORY( plane )

    private:

        f_vector local_normal( const f_point& p ) const override
        {
            return f_vector( 0, 1, 0 );
        }

    };

    intersections intersect( const plane_ptr& p, const ray& r );

    class cube : public shape
    {
    public:

        cube() : shape()
        { }

        aabb_bounds bounds() const noexcept override
        {
            return aabb_bounds( f_point( -1, -1, -1 ), f_point( 1, 1, 1 ) );
        }

        PTR_FACTORY( cube )

    private:

        f_vector local_normal( const f_point& p ) const override;

    };

    intersections intersect( const cube_ptr& c, const ray& r );

    class cylinder : public shape
    {
    public:

        cylinder() :
            shape(), min_extent_( -infinity ), max_extent_( infinity ), closed_( false )
        { }
        
        cylinder( fpnum min, fpnum max ) :
            shape(), min_extent_( min ), max_extent_( max ), closed_( false )
        { }
        
        fpnum min_extent() const noexcept
        {
            return min_extent_;
        }
        
        void set_min_extent( fpnum extent ) noexcept
        {
            min_extent_ = extent;
        }
        
        fpnum max_extent() const noexcept
        {
            return max_extent_;
        }
        
        void set_max_extent( fpnum extent ) noexcept
        {
            max_extent_ = extent;
        }
        
        bool closed() const noexcept
        {
            return closed_;
        }
        
        void set_closed( bool closed ) noexcept
        {
            closed_ = closed;
        }

        aabb_bounds bounds() const noexcept override
        {
            return aabb_bounds( f_point( -1, min_extent_, -1 ), f_point( 1, max_extent_, 1 ) );
        }
        
        static bool check_cap( const ray& r, fpnum t ) noexcept;
        
        static void intersect_caps( const cylinder_ptr& cyl, const ray& r, intersections& itrs );

        PTR_FACTORY( cylinder )
        
    private:
        
        fpnum min_extent_;
        fpnum max_extent_;
        bool closed_;

    private:

        f_vector local_normal( const f_point& p ) const override;
        
    };

    intersections intersect( const cylinder_ptr& cyl, const ray& r );

    class cone : public shape
    {
    public:

        cone() :
            shape(), min_extent_( -infinity ), max_extent_( infinity ), closed_( false )
        { }

        cone( fpnum min, fpnum max ) :
            shape(), min_extent_( min ), max_extent_( max ), closed_( false )
        { }

        fpnum min_extent() const noexcept
        {
            return min_extent_;
        }

        void set_min_extent( fpnum extent ) noexcept
        {
            min_extent_ = extent;
        }

        fpnum max_extent() const noexcept
        {
            return max_extent_;
        }

        void set_max_extent( fpnum extent ) noexcept
        {
            max_extent_ = extent;
        }

        bool closed() const noexcept
        {
            return closed_;
        }

        void set_closed( bool closed ) noexcept
        {
            closed_ = closed;
        }

        aabb_bounds bounds() const noexcept override
        {
            return aabb_bounds( f_point( -1, min_extent_, -1 ), f_point( 1, max_extent_, 1 ) );
        }

        static bool check_cap( const ray& r, fpnum radius, fpnum t ) noexcept;

        static void intersect_caps( const cone_ptr& cyl, const ray& r, intersections& itrs );

        PTR_FACTORY( cone )

    private:

        fpnum min_extent_;
        fpnum max_extent_;
        bool closed_;

    private:

        f_vector local_normal( const f_point& p ) const override;

    };

    intersections intersect( const cone_ptr& cyl, const ray& r );

    class triangle : public shape
    {
    public:

        using shape::normal;

        triangle( const f_point& p1, const f_point& p2, const f_point& p3 ) :
            shape(), p1_( p1 ), p2_( p2 ), p3_( p3 )
        { 
            precompute_edges();
            precompute_normal();
        }

        f_point p1() const noexcept
        {
            return p1_;
        }

        f_point p2() const noexcept
        {
            return p2_;
        }

        f_point p3() const noexcept
        {
            return p3_;
        }

        f_vector e1() const noexcept
        {
            return e1_;
        }

        f_vector e2() const noexcept
        {
            return e2_;
        }

        f_vector normal() const noexcept
        {
            return normal_;
        }

        void precompute_edges() noexcept
        {
            e1_ = p2_ - p1_;
            e2_ = p3_ - p1_;
        }

        void precompute_normal() noexcept
        {
            normal_ = e2_.cross( e1_ ).normalized();
        }

        PTR_FACTORY( triangle )

    private:

        f_point p1_;
        f_point p2_;
        f_point p3_;
        f_vector e1_;
        f_vector e2_;
        f_vector normal_;

    private:

        f_vector local_normal( const f_point& p ) const override
        {
            return normal_;
        }

    };

    intersections intersect( const triangle_ptr& tr, const ray& r );

    class group : public shape
    {
    public:

        using children_list = std::vector<shape_ptr>;

        group() :
            shape(), name_( "Default" )
        { }
        group( const std::string& name ) :
            shape(), name_( name )
        { }

        const std::string& name() const noexcept
        {
            return name_;
        }

        const children_list& children() const noexcept
        {
            return children_;
        }
        
        void set_name( const std::string& name ) noexcept
        {
            name_ = name;
        }

        void add_child( const shape_ptr shape ) noexcept;

        aabb_bounds bounds() const noexcept override;

        PTR_FACTORY( group )

    private:

        std::string name_;
        children_list children_;

    private:

        f_vector local_normal( const f_point& p ) const override
        {
            throw method_not_supported();
        }

    };

    intersections intersect( const group_ptr& grp, const ray& r );
}
