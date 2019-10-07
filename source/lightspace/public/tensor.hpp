#pragma once

#include <iostream>
#include "common.hpp"

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

		tensor( T x, T y, T z, T w ) : 
			x( x ), y( y ), z( z ), w( w ) 
		{ }

		virtual ~tensor() 
		{ }

		virtual tensor_t is() const noexcept
		{
			return tensor_t::tensor;
		}

		const tensor<T> normalized()
		{
			T lenInv = 1 / length();
			return tensor<T>( x * lenInv, y * lenInv, z * lenInv, w * lenInv );
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

		const T dot( const tensor<T>& other )
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

		const point<T> normalized()
		{
			return point<T>( tensor<T>::normalized() );
		}

		const point<T> operator+( const tensor<T>& rhs ) const noexcept
		{
			return point<T>( add( rhs ) );
		}

		const point<T> operator-( const tensor<T>& rhs ) const noexcept
		{
			return point<T>( subtract( rhs ) );
		}

		const point<T> operator-() const noexcept
		{
			return point<T>( negate() );
		}

		const point<T> operator*( T s ) const noexcept
		{
			return point<T>( multiply( s ) );
		}

		const point<T> operator/( const T& s ) const
		{
			return point<T>( divide( s ) );
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

		const color<T> normalized()
		{
			return color<T>( tensor<T>::normalized() );
		}

		const color<T> operator+( const tensor<T>& rhs ) const noexcept
		{
			return color<T>( add( rhs ) );
		}

		const color<T> operator-( const tensor<T>& rhs ) const noexcept
		{
			return color<T>( subtract( rhs ) );
		}

		const color<T> operator-() const noexcept
		{
			return color<T>( negate() );
		}

		const color<T> operator*( T s ) const noexcept
		{
			return color<T>( multiply( s ) );
		}

		const color<T> operator*( const color<T>& rhs ) const noexcept
		{
			return color<T>( hadamard_product( rhs ) );
		}

		const color<T> operator/( const T& s ) const
		{
			return color<T>( divide( s ) );
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

		const T dot( const vector<T>& other )
		{
			return tensor<T>::dot( other );
		}

		const vector<T> cross( const vector<T>& other )
		{
			return vector<T>( y * other.z - z * other.y,
							  z * other.x - x * other.z,
							  x * other.y - y * other.x );
		}

		const vector<T> normalized()
		{
			return vector<T>( tensor<T>::normalized() );
		}

		const vector<T> operator+( const tensor<T>& rhs ) const noexcept
		{
			return vector<T>( add( rhs ) );
		}

		const vector<T> operator-( const tensor<T>& rhs ) const noexcept
		{
			return vector<T>( subtract( rhs ) );
		}

		const vector<T> operator-() const noexcept
		{
			return vector<T>( negate() );
		}

		const vector<T> operator*( T s ) const noexcept
		{
			return vector<T>( multiply( s ) );
		}

		const vector<T> operator/( const T& s ) const
		{
			return vector<T>( divide( s ) );
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
}