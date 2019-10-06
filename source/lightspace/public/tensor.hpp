#pragma once

#include "common.hpp"

namespace ls 
{
	enum class tensor_t 
	{
		point, vector
	};

	template<typename T>
	class tensor 
	{
	public:

		T x{ 0 }, y{ 0 }, z{ 0 }, w{ 0 };

	public:

		tensor( T x, T y, T z, T w ) : 
			x( x ), y( y ), z( z ), w( w ) { }

		tensor_t is() const noexcept
		{
			return w == 0 ? tensor_t::vector : tensor_t::point;
		}

		T length() const
		{
			return std::sqrt( x * x + y * y + z * z + w * w );
		}

		static tensor point( T x, T y, T z )
		{
			return tensor( x, y, z, 1 );
		}

		static tensor vector( T x, T y, T z )
		{
			return tensor( x, y, z, 0 );
		}

		bool operator==( const tensor<T>& rhs ) const noexcept
		{
			return approx( x, rhs.x ) && approx( y, rhs.y ) &&
				approx( z, rhs.z ) && approx( w, rhs.w );
		}

		const tensor<T> operator+( const tensor<T>& rhs ) const noexcept
		{
			return tensor<T>( x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w );
		}

		const tensor<T> operator-( const tensor<T>& rhs ) const noexcept
		{
			return tensor<T>( x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w );
		}

		const tensor<T> operator-() const noexcept
		{
			return tensor<T>( -x, -y, -z, -w );
		}

		const tensor<T> operator*( T s ) const noexcept
		{
			return tensor<T>( x * s, y * s, z * s, w * s );
		}

		const tensor<T> operator/( const T& s ) const
		{
			return tensor<T>( x / s, y / s, z / s, w / s );
		}
	};

	template<typename T>
	inline tensor<T> operator*( T lhs, const tensor<T>& rhs )
	{
		return rhs * lhs;
	}

	using f_tensor = tensor<fpnum>;
	using i_tensor = tensor<int>;
	using c_tensor = tensor<char>;
}