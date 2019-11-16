#pragma once

#include "world.hpp"
#include "shapes.hpp"
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
        intersection( fpnum t, const shape::ptr& s ) :
            time_( t ), object_( s )
        { }

        const fpnum time() const noexcept
        {
            return time_;
        }

        const shape::ptr& object() const noexcept
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
        shape::ptr object_;

    };

    using intersections = std::vector<intersection>;

    intersections intersect( const shape::ptr& s, const ray& r );
    intersections intersect( const world::ptr& s, const ray& r );
    intersection hit( const intersections& itrs );
}