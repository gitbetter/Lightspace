#pragma once

#include "common.hpp"
#include "tensor.hpp"
#include "matrix.hpp"

namespace ls {
    class pattern
    {
    public:
        
        pattern() :
            transform_( f4_matrix::identity() )
        { }
        virtual ~pattern()
        { }
        
        const f4_matrix transform() const noexcept
        {
            return transform_;
        }
        
        void set_transform( const f4_matrix& transform ) noexcept
        {
            transform_ = transform;
        }
        
        f_color color_at( const shape_ptr obj, const f_point& point ) const;
        
        virtual f_color color_at( const f_point& point ) const = 0;
        
    protected:
        
        f4_matrix transform_;
        
    };

    class stripe_pattern : public pattern
    {
    public:

        using pattern::color_at;
        
        stripe_pattern() :
            pattern()
        { }
        
        stripe_pattern( const f_color& first, const f_color& second ) :
            pattern(), first_( first ), second_( second )
        { }

        const f_color first() const noexcept
        {
            return first_;
        }

        const f_color second() const noexcept
        {
            return second_;
        }

        bool operator==( const stripe_pattern& rhs ) const noexcept
        {
            return first_ == rhs.first_ && second_ == rhs.second_;
        }

        bool operator!=( const stripe_pattern& rhs ) const noexcept
        {
            return !( first_ == rhs.first_ && second_ == rhs.second_ );
        }

        f_color color_at( const f_point& point ) const override;

        PTR_FACTORY( stripe_pattern )
        
    private:

        f_color first_;
        f_color second_;

    };

    class gradient_pattern : public pattern
    {
    public:
        
        using pattern::color_at;

        gradient_pattern() :
            pattern()
        { }
        
        gradient_pattern( const f_color& first, const f_color& second ) :
            pattern(), first_( first ), second_( second )
        { }

        const f_color first() const noexcept
        {
            return first_;
        }

        const f_color second() const noexcept
        {
            return second_;
        }

        bool operator==( const gradient_pattern& rhs ) const noexcept
        {
            return first_ == rhs.first_ && second_ == rhs.second_;
        }

        bool operator!=( const gradient_pattern& rhs ) const noexcept
        {
            return !( first_ == rhs.first_ && second_ == rhs.second_ );
        }

        f_color color_at( const f_point& point ) const override;

        PTR_FACTORY( gradient_pattern )
        
    private:

        f_color first_;
        f_color second_;

    };

    class ring_pattern : public pattern
    {
    public:
        
        using pattern::color_at;

        ring_pattern() :
            pattern()
        { }
        
        ring_pattern( const f_color& first, const f_color& second ) :
            pattern(), first_( first ), second_( second )
        { }

        const f_color first() const noexcept
        {
            return first_;
        }

        const f_color second() const noexcept
        {
            return second_;
        }

        bool operator==( const ring_pattern& rhs ) const noexcept
        {
            return first_ == rhs.first_ && second_ == rhs.second_;
        }

        bool operator!=( const ring_pattern& rhs ) const noexcept
        {
            return !( first_ == rhs.first_ && second_ == rhs.second_ );
        }

        f_color color_at( const f_point& point ) const override;

        PTR_FACTORY( ring_pattern )
        
    private:

        f_color first_;
        f_color second_;

    };

    class checker_pattern : public pattern
    {
    public:
        
        using pattern::color_at;

        checker_pattern() :
            pattern()
        { }
        
        checker_pattern( const f_color& first, const f_color& second ) :
            pattern(), first_( first ), second_( second )
        { }

        const f_color first() const noexcept
        {
            return first_;
        }

        const f_color second() const noexcept
        {
            return second_;
        }

        bool operator==( const checker_pattern& rhs ) const noexcept
        {
            return first_ == rhs.first_ && second_ == rhs.second_;
        }

        bool operator!=( const checker_pattern& rhs ) const noexcept
        {
            return !( first_ == rhs.first_ && second_ == rhs.second_ );
        }

        f_color color_at( const f_point& point ) const override;

        PTR_FACTORY( checker_pattern )
        
    private:

        f_color first_;
        f_color second_;

    };
}
