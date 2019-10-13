#pragma once

#include "common.hpp"
#include "tensor.hpp"
#include <array>

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
					if ( !approx( _data[i][j], rhs._data[i][j] ) )
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
					if ( !approx( _data[i][j], rhs._data[i][j] ) )
					{
						return true;
					}
				}
			}
			return false;
		}

		const matrix<T, Rows, Cols> operator*( const matrix<T, Rows, Cols>& rhs ) const noexcept
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

		const tensor<T> operator*( const tensor<T>& rhs ) const noexcept
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

		const matrix<T, Rows, Cols> transpose() const noexcept
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

		const T determinant() const
		{
			T det = 0;
			for ( auto i = 0; i < Cols; i++ )
			{
				det += _data[0][i] * cofactor( 0, i );
			}
			return det;
		}

		const T minor( uint8_t r, uint8_t c ) const
		{
			return submatrix( r, c ).determinant();
		}

		const T cofactor( uint8_t r, uint8_t c ) const
		{
			return minor( r, c ) * ( ( r + c ) % 2 != 0 ? -1 : 1 );
		}

		const matrix<T, Rows - 1, Cols - 1> submatrix( uint8_t row_to_remove, uint8_t col_to_remove ) const
		{
			matrix<T, Rows - 1, Cols - 1> result;
			for ( auto i = 0; i < Rows - 1; i++ )
			{
				for ( auto j = 0; j < Cols - 1; j++ )
				{
					auto otherI = i < row_to_remove ? i : i + 1;
					auto otherJ = j < col_to_remove ? j : j + 1;
					result( i, j ) = _data[otherI][otherJ];
				}
			}
			return result;
		}

		const bool is_invertible() const
		{
			return determinant() != 0;
		}

		const matrix<T, Rows, Cols> inverse() const
		{
			if ( !is_invertible() )
			{
				throw method_not_supported();
			}

			matrix<T, Rows, Cols> inverse;
			for ( auto i = 0; i < Rows; i++ )
			{
				for ( auto j = 0; j < Cols; j++ )
				{
					inverse._data[j][i] = cofactor( i, j ) / determinant();
				}
			}
			return inverse;
		}

		static const matrix<T, Rows, Cols> identity() noexcept
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

		void initialize_with_zeroes() noexcept
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

		void initialize_with_identity() noexcept
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

		void initialize_with_elements( const std::initializer_list<T>& elements ) noexcept
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

	template<>
	inline const fpnum matrix<fpnum, 2, 2>::determinant() const
	{
		return _data[0][0] * _data[1][1] - _data[0][1] * _data[1][0];
	}

	template<>
	inline const int matrix<int, 2, 2>::determinant() const
	{
		return _data[0][0] * _data[1][1] - _data[0][1] * _data[1][0];
	}

	template<>
	inline const fpnum matrix<fpnum, 2, 2>::minor( uint8_t r, uint8_t c ) const
	{
		auto i = r == 0 ? 1 : 0;
		auto j = c == 0 ? 1 : 0;
		return _data[i][j];
	}

	template<>
	inline const int matrix<int, 2, 2>::minor( uint8_t r, uint8_t c ) const
	{
		auto i = r == 0 ? 1 : 0;
		auto j = c == 0 ? 1 : 0;
		return _data[i][j];
	}

	template<typename T, std::size_t Rows, std::size_t Cols>
	std::ostream& operator<<( std::ostream& out, const matrix<T, Rows, Cols>& m )
	{
		out << '[';
		for ( auto i = 0; i < Rows; i++ )
		{
			for ( auto j = 0; j < Cols; j++ )
			{
				out << m( i, j ) << ( j == Cols - 1 ? "" : " " );
			}
			out << ( i == Rows - 1 ? "" : "\n " );
		}
		out << ']';
		return out;
	}

	using f2_matrix = matrix<fpnum, 2, 2>;
	using f3_matrix = matrix<fpnum, 3, 3>;
	using f4_matrix = matrix<fpnum, 4, 4>;
	using i2_matrix = matrix<int, 2, 2>;
	using i3_matrix = matrix<int, 3, 3>;
	using i4_matrix = matrix<int, 4, 4>;
}