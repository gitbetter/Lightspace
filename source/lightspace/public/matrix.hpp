#pragma once

#include "common.hpp"
#include "tensor.hpp"
#include <array>
#include <type_traits>

namespace ls {
	template<
		typename T, std::size_t Rows, std::size_t Cols,
		typename = std::enable_if_t<std::is_arithmetic<T>::value>
	>
	class matrix
	{
	public:

		matrix()
		{
			initialize_with_zeroes();
		}

		matrix( const std::initializer_list<T>& elements )
		{
			initialize_with_elements( elements );
		}

		T operator()( std::size_t r, std::size_t c ) const
		{
			if ( !within_bounds( r, c ) )
			{
				throw std::out_of_range( "Matrix indices are out of range" );
			}
			return _data[r][c];
		}

		T& operator()( std::size_t r, std::size_t c )
		{
			if ( !within_bounds( r, c ) )
			{
				throw std::out_of_range( "Matrix indices are out of range" );
			}
			return _data[r][c];
		}

		bool operator==( const matrix<T, Rows, Cols>& rhs ) const noexcept
		{
			for ( auto i = 0; i < Rows; i++ )
			{
				for ( auto j = 0; j < Cols; j++ )
				{
					if ( _data[i][j] != rhs._data[i][j] )
					{
						return false;
					}
				}
			}
			return true;
		}

		bool operator!=( const matrix<T, Rows, Cols>& rhs ) const noexcept
		{
			for ( auto i = 0; i < Rows; i++ )
			{
				for ( auto j = 0; j < Cols; j++ )
				{
					if ( _data[i][j] != rhs._data[i][j] )
					{
						return true;
					}
				}
			}
			return false;
		}

		const matrix<T, Rows, Cols> operator*( const matrix<T, Rows, Cols>& rhs ) const
		{
			matrix<T, Rows, Cols> result;
			for ( auto i = 0; i < Rows; i++ )
			{
				for ( auto j = 0; j < Cols; j++ )
				{
					for ( auto k = 0; k < Rows; k++ )
					{
						result._data[i][j] += _data[i][k] * rhs._data[k][j];
					}
				}
			}
			return result;
		}

		const tensor<T> operator*( const tensor<T>& rhs ) const
		{
			tensor<T> result;
			for ( auto i = 0; i < Rows; i++ )
			{
				for ( auto j = 0; j < Cols; j++ )
				{
					result( i ) += _data[i][j] * rhs( j );
				}
			}
			return result;
		}

		const matrix<T, Rows, Cols> transpose() const
		{
			matrix<T, Rows, Cols> result;
			for ( auto i = 0; i < Rows; i++ )
			{
				for ( auto j = 0; j < Cols; j++ )
				{
					result._data[j][i] = _data[i][j];
				}
			}
			return result;
		}

		static const matrix<T, Rows, Cols> identity()
		{
			static matrix<T, Rows, Cols> result;
			if ( result._data[0][0] == 0 )
			{
				result.initialize_with_identity();
			}
			return result;
		}
	
	private:

		std::array<std::array<T, Cols>, Rows> _data;

	private:

		void initialize_with_zeroes()
		{
			{
				for ( auto i = 0; i < Rows; i++ )
				{
					for ( auto j = 0; j < Cols; j++ )
					{
						_data[i][j] = 0;
					}
				}
			}
		}

		void initialize_with_identity()
		{
			{
				for ( auto i = 0; i < Rows; i++ )
				{
					for ( auto j = 0; j < Cols; j++ )
					{
						_data[i][j] = i == j ? static_cast<T>( 1 ) : static_cast<T>( 0 );
					}
				}
			}
		}

		void initialize_with_elements( const std::initializer_list<T>& elements )
		{
			auto it = elements.begin();
			for ( auto i = 0; i < Rows; i++ )
			{
				for ( auto j = 0; j < Cols; j++ )
				{
					if ( it == elements.end() )
					{
						_data[i][j] = 0;
					}
					else
					{
						_data[i][j] = *it;
						++it;
					}
				}
			}
		}

		inline bool within_bounds( const std::size_t& r, const std::size_t& c ) const noexcept
		{
			return r >= 0 && r < Rows && c >= 0 && c < Cols;
		}

	};

	using f2_matrix = matrix<fpnum, 2, 2>;
	using f3_matrix = matrix<fpnum, 3, 3>;
	using f4_matrix = matrix<fpnum, 4, 4>;
	using i2_matrix = matrix<int, 2, 2>;
	using i3_matrix = matrix<int, 3, 3>;
	using i4_matrix = matrix<int, 4, 4>;
}