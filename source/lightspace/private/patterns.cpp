#include "patterns.hpp"
#include "shapes.cpp"

namespace ls {
    stripe_pattern stripe_pattern::none = stripe_pattern( f_color( 0, 0, 0 ), f_color( 0, 0, 0 ) );

    f_color stripe_pattern::stripe_at( const f_point& point ) const
    {
        return static_cast<int>( floor( point.x ) ) % 2 == 0 ? first_ : second_;
    }

    f_color stripe_pattern::stripe_at( const shape_ptr obj, const f_point& point ) const
    {
        auto obj_point = obj->transform().inverse() * point;
        auto patt_point = transform_.inverse() * obj_point;
        return stripe_at( patt_point );
    }
}
