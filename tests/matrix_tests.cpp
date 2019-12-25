#include "catch.hpp"
#include "matrix.hpp"

using namespace ls;

TEST_CASE( "Matrix processing", "[matrices]" )
{
    SECTION( "Constructing and inspecting a 4x4 matrix" )
    {
        auto m = f4_matrix{
            1,    2,    3,    4,
            5.5f, 6.5f, 7.5f, 8.5f,
            9,    10,   11,   12,
            13.5, 14.5, 15.5, 16.5
        };

        REQUIRE( m( 0, 0 ) == 1 );
        REQUIRE( m( 0, 3 ) == 4 );
        REQUIRE( m( 1, 0 ) == 5.5f );
        REQUIRE( m( 1, 2 ) == 7.5f );
        REQUIRE( m( 2, 2 ) == 11 );
        REQUIRE( m( 3, 0 ) == 13.5f );
        REQUIRE( m( 3, 2 ) == 15.5f );
    }

    SECTION( "Constructing and inspecting a 3x3 matrix" )
    {
        auto m = f3_matrix{
            -3, 5, 0,
            1, -2, 7,
            0, 1, 1
        };

        REQUIRE( m( 0, 0 ) == -3 );
        REQUIRE( m( 1, 1 ) == -2 );
        REQUIRE( m( 2, 2 ) == 1 );
    }

    SECTION( "Constructing and inspecting a 2x2 matrix" )
    {
        auto m = f2_matrix{
            2, 0.5f,
            1.5f, 1
        };

        REQUIRE( m( 0, 0 ) == 2 );
        REQUIRE( m( 0, 1 ) == 0.5f );
        REQUIRE( m( 1, 0 ) == 1.5f );
        REQUIRE( m( 1, 1 ) == 1 );
    }

    SECTION( "Matrix equality with identical matrices" )
    {
        auto m1 = i4_matrix{
            1, 2, 3, 4,
            5, 6, 7, 8,
            9, 8, 7, 6,
            5, 4, 3, 2
        };
        auto m2 = i4_matrix{
            1, 2, 3, 4,
            5, 6, 7, 8,
            9, 8, 7, 6,
            5, 4, 3, 2
        };

        REQUIRE( m1 == m2 );
    }

    SECTION( "Matrix equality with different matrices" )
    {
        auto m1 = i4_matrix{
            1, 2, 3, 4,
            5, 6, 7, 8,
            9, 8, 7, 6,
            5, 4, 3, 2
        };
        auto m2 = i4_matrix{
            2, 3, 4, 5,
            6, 7, 8, 9,
            8, 7, 6, 5,
            4, 3, 2, 1
        };

        REQUIRE( m1 != m2 );
    }

    SECTION( "Multiplying two 4x4 matrices" )
    {
        auto m1 = i4_matrix{
            1, 2, 3, 4,
            5, 6, 7, 8,
            9, 8, 7, 6,
            5, 4, 3, 2
        };
        auto m2 = i4_matrix{
            -2, 1, 2, 3,
            3, 2, 1, -1,
            4, 3, 6, 5,
            1, 2, 7, 8
        };
        auto result = i4_matrix{
            20, 22, 50, 48,
            44, 54, 114, 108,
            40, 58, 110, 102,
            16, 26, 46, 42
        };

        REQUIRE( m1 * m2 == result );
    }

    SECTION( "Multiplying a matrix by a tensor" )
    {
        auto m = i4_matrix{
            1, 2, 3, 4,
            2, 4, 4, 2,
            8, 6, 4, 1,
            0, 0, 0, 1
        };
        auto t = i_tensor( 1, 2, 3, 1 );
        auto result = i_tensor( 18, 24, 33, 1 );

        REQUIRE( m * t == result );
    }

    SECTION( "Multiplying a matrix by the identity matrix" )
    {
        auto m = i4_matrix{
            0, 1, 2, 4,
            1, 2, 4, 8,
            2, 4, 8, 16,
            4, 8, 16, 32
        };

        REQUIRE( m * i4_matrix::identity() == m );
    }

    SECTION( "Multiplying the identity matrix by a tuple" )
    {
        auto t = i_tensor( 1, 2, 3, 4 );

        REQUIRE( i4_matrix::identity() * t == t );
    }

    SECTION( "Transposing a matrix" )
    {
        auto m = i4_matrix{
            1, 9, 7, 5,
            0, 3, 3, 2,
            1, 1, 2, 5,
            8, 4, 6, 7
        };
        auto result = i4_matrix{
            1, 0, 1, 8,
            9, 3, 1, 4,
            7, 3, 2, 6,
            5, 2, 5, 7
        };

        REQUIRE( m.transpose() == result );
    }

    SECTION( "Transposing the identity matrix" )
    {
        REQUIRE( i4_matrix::identity().transpose() == i4_matrix::identity() );
    }

    SECTION( "Calculating the determinant of a 2x2 matrix" )
    {
        auto m = i2_matrix{
            1, 5,
            -3, 2
        };

        REQUIRE( m.determinant() == 17 );
    }

    SECTION( "A submatrix of a 3x3 matrix is a 2x2 matrix" )
    {
        auto m = i3_matrix{
            3, -1, 8,
            2, 9, 2,
            4, 0, -2
        };
        auto result = i2_matrix{
            2, 9,
            4, 0
        };

        REQUIRE( m.submatrix( 0, 2 ) == result );
    }

    SECTION( "A submatrix of a 4x4 matrix is a 3x3 matrix" )
    {
        auto m = i4_matrix{
            2, 4, 8, 9,
            1, 3, 5, 6,
            3, 3, 9, 7,
            -1, 0, 1, 2
        };
        auto result = i3_matrix{
            2, 8, 9,
            1, 5, 6,
            3, 9, 7
        };

        REQUIRE( m.submatrix( 3, 1 ) == result );
    }

    SECTION( "Calculating a minor of a 3x3 matrix" )
    {
        auto m = i3_matrix{
            3, 5, 0,
            2, -1, -7,
            6, -1, 5
        };
        auto sub = m.submatrix( 1, 0 );
        auto detB = sub.determinant();

        REQUIRE( m.minor( 1, 0 ) == detB );
    }

    SECTION( "Calculating a cofactor of a 3x3 matrix" )
    {
        auto m = i3_matrix{
            3, 5, 0,
            2, -1, -7,
            6, -1, 5
        };
        auto minor = m.minor( 0, 0 );

        REQUIRE( minor == -12 );

        auto cofactor = m.cofactor( 0, 0 );

        REQUIRE( cofactor == -12 );

        minor = m.minor( 1, 0 );

        REQUIRE( minor == 25 );

        cofactor = m.cofactor( 1, 0 );

        REQUIRE( cofactor == -25 );
    }

    SECTION( "Calculating the determinant of a 3x3 matrix" )
    {
        auto m = i3_matrix{
            1, 2, 6,
            -5, 8, -4,
            2, 6, 4
        };

        REQUIRE( m.cofactor( 0, 0 ) == 56 );
        REQUIRE( m.cofactor( 0, 1 ) == 12 );
        REQUIRE( m.cofactor( 0, 2 ) == -46 );
        REQUIRE( m.determinant() == -196 );
    }

    SECTION( "Calculating the determinant of a 4x4 matrix" )
    {
        auto m = i4_matrix{
            -2, -8, 3, 5,
            -3, 1, 7, 3,
            1, 2, -9, 6,
            -6, 7, 7, -9
        };

        REQUIRE( m.cofactor( 0, 0 ) == 690 );
        REQUIRE( m.cofactor( 0, 1 ) == 447 );
        REQUIRE( m.cofactor( 0, 2 ) == 210 );
        REQUIRE( m.cofactor( 0, 3 ) == 51 );
        REQUIRE( m.determinant() == -4071 );
    }

    SECTION( "Testing and invertible matrix for invertibility" )
    {
        auto m = i4_matrix{
            6, 4, 4, 4,
            5, 5, 7, 6,
            4, -9, 3, -7,
            9, 1, 7, -6
        };

        REQUIRE( m.determinant() == -2120 );
        REQUIRE( m.is_invertible() );
    }

    SECTION( "Testing and non-invertible matrix for invertibility" )
    {
        auto m = i4_matrix{
            -4, 2, -2, -3,
            9, 6, 2, 6,
            0, -5, 1, -5,
            0, 0, 0, 0
        };

        REQUIRE( m.determinant() == 0 );
        REQUIRE( !m.is_invertible() );
    }

    SECTION( "Calculate the inverse of a matrix" )
    {
        auto m = f4_matrix{
            -5, 2, 6, -8,
            1, -5, 1, 8,
            7, 7, -6, -7,
            1, -3, 7, 4
        };
        auto inv_result = f4_matrix{
            0.21805f, 0.45113f, 0.24060f, -0.04511f,
            -0.80827f, -1.45677f, -0.44361f, 0.52068f,
            -0.07895f, -0.22368f, -0.05263f, 0.19737f,
            -0.52256f, -0.81391f, -0.30075f, 0.30639f
        };
        auto inv = m.inverse();

        REQUIRE( m.determinant() == 532 );
        REQUIRE( m.cofactor( 2, 3 ) == -160 );
        REQUIRE( approx( inv( 3, 2 ), -160.f / 532.f ) );
        REQUIRE( m.cofactor( 3, 2 ) == 105 );
        REQUIRE( approx( inv( 2, 3 ), 105.f / 532.f ) );
        REQUIRE( inv == inv_result );
    }

    SECTION( "Calculating the inverse of another matrix" )
    {
        auto m = f4_matrix{
            8, -5, 9, 2,
            7, 5, 6, 1,
            -6, 0, 9, 6,
            -3, 0, -9, -4
        };
        auto inv_result = f4_matrix{
            -0.15385f, -0.15385f, -0.28205f, -0.53846f,
            -0.07692f, 0.12308f, 0.02564f, 0.03077f,
            0.35897f, 0.35897f, 0.43590f, 0.92308f,
            -0.69231f, -0.69231f, -0.76923f, -1.92308f
        };

        REQUIRE( m.inverse() == inv_result );
    }

    SECTION( "Calculating the inverse of a third matrix" )
    {
        auto m = f4_matrix{
            9, 3, 0, 9,
            -5, -2, -6, -3,
            -4, 9, 6, 4,
            -7, 6, 6, 2
        };
        auto inv_result = f4_matrix{
            -0.04074f, -0.07778f, 0.14444f, -0.22222f,
            -0.07778f, 0.03333f, 0.36667f, -0.33333f,
            -0.02901f, -0.14630f, -0.10926f, 0.12963f,
            0.17778f, 0.06667f, -0.26667f, 0.33333f
        };

        REQUIRE( m.inverse() == inv_result );
    }

    SECTION( "Multiplying a matrix product by it's inverse" )
    {
        auto a = f4_matrix{
            3, -9, 7, 3,
            3, -8, 2, -9,
            -4, 4, 4, 1,
            -6, 5, -1, 1
        };
        auto b = f4_matrix{
            8, 2, 2, 2,
            3, -1, 7, 0,
            7, 0, 5, 4,
            6, -2, 0, 5
        };
        auto c = a * b;

        REQUIRE( c * b.inverse() == a );
    }
};
