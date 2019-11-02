#pragma once

#include "tensor.hpp"
#include "matrix.hpp"

namespace ls {
    class ray
    {
    public:

        ray( const f_point& o, const f_vector& d ) :
            origin_( o ), direction_( d )
        { }

        const f_point position( fpnum t ) const noexcept
        {
            return origin_ + direction_ * t;
        }

        const f_point& origin() const noexcept
        {
            return origin_;
        }

        const f_vector& direction() const noexcept
        {
            return direction_;
        }

    private:

        f_point origin_;
        f_vector direction_;

    };

    inline const ray operator*( const f4_matrix& mat, const ray& r ) noexcept
    {
        return ray( mat * r.origin(), mat * r.direction() );
    }
}