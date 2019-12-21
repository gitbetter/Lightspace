#include "patterns.hpp"

namespace ls {
    stripe_pattern stripe_pattern::none = stripe_pattern( f_color( 0, 0, 0 ), f_color( 0, 0, 0 ) );

    f_color stripe_pattern::stripe_at( const f_point& point ) const
    {
        return static_cast<int>( floor( point.x ) ) % 2 == 0 ? first_ : second_;
    }
}