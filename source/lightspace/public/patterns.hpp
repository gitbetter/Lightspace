#pragma once

#include "common.hpp"
#include "tensor.hpp"
#include "matrix.hpp"

namespace ls {
    class pattern { };

    class stripe_pattern : public pattern
    {
    public:

        stripe_pattern()
        { }
        
        stripe_pattern( const f_color& first, const f_color& second ) : 
            first_( first ), second_( second ), transform_( f4_matrix::identity() )
        { }

        const f_color first() const noexcept
        {
            return first_;
        }

        const f_color second() const noexcept
        {
            return second_;
        }
        
        const f4_matrix transform() const noexcept
        {
            return transform_;
        }
        
        void set_transform( const f4_matrix& transform ) noexcept
        {
            transform_ = transform;
        }

        bool operator==( const stripe_pattern& rhs ) const noexcept
        {
            return first_ == rhs.first_ && second_ == rhs.second_;
        }

        bool operator!=( const stripe_pattern& rhs ) const noexcept
        {
            return !( first_ == rhs.first_ && second_ == rhs.second_ );
        }

        f_color stripe_at( const f_point& point ) const;
        
        f_color stripe_at( const shape_ptr obj, const f_point& point ) const;

        static stripe_pattern none;

    private:

        f_color first_;
        f_color second_;
        f4_matrix transform_;

    };
}
