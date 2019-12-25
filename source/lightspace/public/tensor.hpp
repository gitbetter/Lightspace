#pragma once

#include "common.hpp"
#include <iostream>

namespace ls
{
    /**
     * Available tensor types
     */

    enum class tensor_t
    {
        tensor, point, vector, color
    };

    /**
     * Tensor base class and definitions
     */

    template<typename T>
    class tensor
    {
    public:

        union
        {
            T x{ 0 };
            T r;
        };

        union
        {
            T y{ 0 };
            T g;
        };

        union
        {
            T z{ 0 };
            T b;
        };

        union
        {
            T w{ 0 };
            T a;
        };

    public:

        tensor() :
            x( 0 ), y( 0 ), z( 0 ), w( 0 )
        { }

        tensor( T x, T y, T z, T w ) :
            x( x ), y( y ), z( z ), w( w )
        { }

        virtual ~tensor()
        { }

        virtual tensor_t is() const noexcept
        {
            return tensor_t::tensor;
        }

        const tensor<T> normalized() const
        {
            T lenInv = 1 / length();
            return tensor<T>( x * lenInv, y * lenInv, z * lenInv, w * lenInv );
        }

        T operator()( uint8_t idx ) const
        {
            if ( idx < 0 || idx > 3 )
            {
                throw std::out_of_range( "Tensor index " + std::to_string( idx ) + " is out of range" );
            }
            if ( idx == 0 ) return x;
            if ( idx == 1 ) return y;
            if ( idx == 2 ) return z;
            return w;
        }

        T& operator()( uint8_t idx )
        {
            if ( idx < 0 || idx > 3 )
            {
                throw std::out_of_range( "Tensor index " + std::to_string( idx ) + " is out of range" );
            }
            if ( idx == 0 ) return x;
            if ( idx == 1 ) return y;
            if ( idx == 2 ) return z;
            return w;
        }

        bool operator==( const tensor<T>& rhs ) const noexcept
        {
            return equals( rhs );
        }

        const tensor<T> operator+( const tensor<T>& rhs ) const noexcept
        {
            return add( rhs );
        }

        const tensor<T> operator-( const tensor<T>& rhs ) const noexcept
        {
            return subtract( rhs );
        }

        const tensor<T> operator-() const noexcept
        {
            return negate();
        }

        const tensor<T> operator*( T s ) const noexcept
        {
            return multiply( s );
        }

        const tensor<T> operator/( const T& s ) const
        {
            return divide( s );
        }

    protected:

        T length() const
        {
            return std::sqrt( x * x + y * y + z * z + w * w );
        }

        const T dot( const tensor<T>& other ) const noexcept
        {
            return x * other.x +
                y * other.y +
                z * other.z +
                w * other.w;
        }

        bool equals( const tensor<T>& rhs ) const noexcept
        {
            return approx( x, rhs.x ) && approx( y, rhs.y ) &&
                approx( z, rhs.z ) && approx( w, rhs.w );
        }

        const tensor<T> add( const tensor<T>& rhs ) const noexcept
        {
            return tensor<T>( x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w );
        }

        const tensor<T> subtract( const tensor<T>& rhs ) const noexcept
        {
            return tensor<T>( x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w );
        }

        const tensor<T> negate() const noexcept
        {
            return tensor<T>( -x, -y, -z, -w );
        }

        const tensor<T> multiply( T s ) const noexcept
        {
            return tensor<T>( x * s, y * s, z * s, w * s );
        }

        const tensor<T> hadamard_product( const tensor<T>& rhs ) const noexcept
        {
            return tensor<T>( x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w );
        }

        const tensor<T> divide( const T& s ) const
        {
            // TODO: Handle division by zero
            return tensor<T>( x / s, y / s, z / s, w / s );
        }

    };

    template<typename T>
    inline tensor<T> operator*( T lhs, const tensor<T>& rhs )
    {
        return rhs * lhs;
    }

    template<typename T>
    std::ostream& operator<<( std::ostream& out, const tensor<T>& t )
    {
        out << "(x: " << t.x << ",y: " << t.y << ",z: " << t.z << ",w: " << t.w << ")";
        return out;
    }

    using f_tensor = tensor<fpnum>;
    using i_tensor = tensor<int>;
    using c_tensor = tensor<char>;

    /**
 * Vector class and definitions
 */

    template<typename T>
    class vector : public tensor<T>
    {
    public:

        vector( T x, T y, T z ) :
            tensor<T>( x, y, z, 0 )
        { }

        vector( const tensor<T>& t ) :
            tensor<T>( t.x, t.y, t.z, t.w )
        { }

        tensor_t is() const noexcept override
        {
            return tensor_t::vector;
        }

        T length() const
        {
            return tensor<T>::length();
        }

        const T dot( const vector<T>& other ) const
        {
            return tensor<T>::dot( other );
        }

        const vector<T> cross( const vector<T>& other ) const
        {
            return vector<T>( this->y * other.z - this->z * other.y,
                              this->z * other.x - this->x * other.z,
                              this->x * other.y - this->y * other.x );
        }

        const vector<T> reflect( const vector<T>& normal ) const
        {
            return *this - normal * 2 * this->dot( normal );
        }

        const vector<T> normalized() const
        {
            return vector<T>( tensor<T>::normalized() );
        }

        const vector<T> operator+( const tensor<T>& rhs ) const noexcept
        {
            return vector<T>( this->add( rhs ) );
        }

        const vector<T> operator-( const tensor<T>& rhs ) const noexcept
        {
            return vector<T>( this->subtract( rhs ) );
        }

        const vector<T> operator-() const noexcept
        {
            return vector<T>( this->negate() );
        }

        const vector<T> operator*( T s ) const noexcept
        {
            return vector<T>( this->multiply( s ) );
        }

        const vector<T> operator/( const T& s ) const
        {
            return vector<T>( this->divide( s ) );
        }

    };

    template<typename T>
    inline vector<T> operator*( T lhs, const vector<T>& rhs )
    {
        return rhs * lhs;
    }

    using f_vector = vector<fpnum>;
    using i_vector = vector<int>;
    using c_vector = vector<char>;

    /**
     * Point class and definitions
     */

    template<typename T>
    class point : public tensor<T>
    {
    public:

        point( T x, T y, T z ) :
            tensor<T>( x, y, z, 1 )
        { }

        point( const tensor<T>& t ) :
            tensor<T>( t.x, t.y, t.z, t.w )
        { }

        tensor_t is() const noexcept override
        {
            return tensor_t::point;
        }

        const point<T> normalized() const
        {
            return point<T>( tensor<T>::normalized() );
        }

        const point<T> operator+( const tensor<T>& rhs ) const noexcept
        {
            return point<T>( this->add( rhs ) );
        }

        const vector<T> operator-( const tensor<T>& rhs ) const noexcept
        {
            return point<T>( this->subtract( rhs ) );
        }

        const point<T> operator-() const noexcept
        {
            return point<T>( this->negate() );
        }

        const point<T> operator*( T s ) const noexcept
        {
            return point<T>( this->multiply( s ) );
        }

        const point<T> operator/( const T& s ) const
        {
            return point<T>( this->divide( s ) );
        }

    };

    template<typename T>
    inline point<T> operator*( T lhs, const point<T>& rhs )
    {
        return rhs * lhs;
    }

    using f_point = point<fpnum>;
    using i_point = point<int>;
    using c_point = point<char>;

    /**
     * Color class and definitions
     */

    template<typename T>
    class color : public tensor<T>
    {
    public:

        color() :
            tensor<T>( 0, 0, 0, 1 )
        { }

        color( T r, T g, T b, T a ) :
            tensor<T>( r, g, b, a )
        { }

        color( T r, T g, T b ) :
            tensor<T>( r, g, b, 1 )
        { }

        color( const tensor<T>& t ) :
            tensor<T>( t.r, t.g, t.b, t.a )
        { }

        tensor_t is() const noexcept override
        {
            return tensor_t::color;
        }

        const color<T> normalized() const
        {
            return color<T>( tensor<T>::normalized() );
        }

        const color<T> operator+( const tensor<T>& rhs ) const noexcept
        {
            auto col = this->add( rhs );
            col.a = 1.f;
            return color<T>( col );
        }

        const color<T> operator-( const tensor<T>& rhs ) const noexcept
        {
            auto col = this->subtract( rhs );
            col.a = 1.f;
            return color<T>( col );
        }

        const color<T> operator-() const noexcept
        {
            auto col = this->negate();
            col.a = 1.f;
            return color<T>( col );
        }

        const color<T> operator*( T s ) const noexcept
        {
            auto col = this->multiply( s );
            col.a = 1.f;
            return color<T>( col );
        }

        const color<T> operator*( const color<T>& rhs ) const noexcept
        {
            auto col = this->hadamard_product( rhs );
            col.a = 1.f;
            return color<T>( col );
        }

        const color<T> operator/( const T& s ) const
        {
            auto col = this->divide( s );
            col.a = 1.f;
            return color<T>( col );
        }

    };

    template<typename T>
    inline color<T> operator*( T lhs, const color<T>& rhs )
    {
        return rhs * lhs;
    }

    using f_color = color<fpnum>;
    using i_color = color<int>;
    using c_color = color<char>;
}
