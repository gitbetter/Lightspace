#pragma once

#include "ray.hpp"
#include <vector>

namespace ls {
    class intersection
    {
    public:

        static const intersection none;

    public:

        intersection() :
            time_( 0 ), object_( nullptr )
        { }
        intersection( fpnum t, const shape_ptr& s ) :
            time_( t ), object_( s )
        { }

        const fpnum time() const noexcept
        {
            return time_;
        }

        const shape_ptr& object() const noexcept
        {
            return object_;
        }

        bool operator==( const intersection& rhs ) const
        {
            return approx( time_, rhs.time_ ) && object_ == rhs.object_;
        }

        bool operator!=( const intersection& rhs ) const
        {
            return !approx( time_, rhs.time_ ) || object_ != rhs.object_;
        }

        bool operator<( const intersection& rhs ) const
        {
            return time_ < rhs.time_;
        }

    protected:

        fpnum time_;
        shape_ptr object_;

    };

    struct intersection_state
    {
        fpnum time;
        shape_ptr object;
        f_point point;
        f_vector eye;
        f_vector normal;
        bool inside;

        intersection_state() :
            time( 0 ), object( nullptr ), point( f_point(0, 0, 0) ), eye( f_vector( 0, 0, 0 ) ), 
            normal( f_vector( 0, 0, 0 ) ), inside( false )
        { }
    };

    using intersections = std::vector<intersection>;

    intersection_state prepare_intersection_state( const intersection& i, const ray& r );
    intersection hit( const intersections& itrs );
}