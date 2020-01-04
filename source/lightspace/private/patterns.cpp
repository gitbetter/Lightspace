#include "patterns.hpp"
#include "shapes.cpp"

namespace ls {
    f_color pattern::color_at( const shape_ptr obj, const f_point& point ) const
    {
        auto obj_point = obj->world_to_object( point );
        auto patt_point = transform_.inverse() * obj_point;
        return color_at( patt_point );
    }

    f_color test_pattern::color_at( const f_point& point ) const
    {
        return f_color( point.x, point.y, point.z );
    }

    f_color solid_pattern::color_at( const f_point& point ) const
    {
        return color_;
    }

    f_color stripe_pattern::color_at( const f_point& point ) const
    {
        return static_cast<int>( floor( point.x ) ) % 2 == 0 ? first_ : second_;
    }

    f_color gradient_pattern::color_at( const f_point& point ) const
    {
        auto distance = second_ - first_;
        auto fraction = point.x - floor( point.x );
        return first_ + distance * fraction;
    }

    f_color ring_pattern::color_at( const f_point& point ) const
    {
        return static_cast<int>( floor( sqrt( point.x * point.x + point.z * point.z ) ) ) % 2 == 0 ? first_ : second_;
    }

    f_color checker_pattern::color_at( const f_point& point ) const
    {
        return static_cast<int>( floor( point.x ) + floor( point.y ) + floor( point.z ) ) % 2 == 0 ? first_ : second_;
    }
}
