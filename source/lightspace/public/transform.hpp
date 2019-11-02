#pragma once

#include "common.hpp"
#include "matrix.hpp"

namespace ls {
    namespace transform {
        template<typename T>
        const matrix<T, 4, 4> translation( T x, T y, T z )
        {
            matrix<T, 4, 4> mat{
                1, 0, 0, x,
                0, 1, 0, y,
                0, 0, 1, z,
                0, 0, 0, 1
            };
            return mat;
        }

        template<typename T>
        const matrix<T, 4, 4> scale( T x, T y, T z )
        {
            matrix<T, 4, 4> mat{
                x, 0, 0, 0,
                0, y, 0, 0,
                0, 0, z, 0,
                0, 0, 0, 1
            };
            return mat;
        }

        template<typename T>
        const matrix<T, 4, 4> rotation_x( T rads )
        {
            matrix<T, 4, 4> mat{
                1, 0,				 0,					0,
                0, std::cos( rads ), -std::sin( rads ), 0,
                0, std::sin( rads ), std::cos( rads ),  0,
                0, 0,				 0,					1
            };
            return mat;
        }

        template<typename T>
        const matrix<T, 4, 4> rotation_y( T rads )
        {
            matrix<T, 4, 4> mat{
                std::cos( rads ),  0, std::sin( rads ), 0,
                0,				   1, 0,			    0,
                -std::sin( rads ), 0, std::cos( rads ), 0,
                0,				   0, 0,				1
            };
            return mat;
        }

        template<typename T>
        const matrix<T, 4, 4> rotation_z( T rads )
        {
            matrix<T, 4, 4> mat{
                std::cos( rads ), -std::sin( rads ), 0, 0,
                std::sin( rads ), std::cos( rads ),  0, 0,
                0,				  0,				 1, 0,
                0,				  0,				 0, 1
            };
            return mat;
        }

        template<typename T>
        const matrix<T, 4, 4> shear( T x_to_y, T x_to_z, T y_to_x, T y_to_z, T z_to_x, T z_to_y )
        {
            matrix<T, 4, 4> mat{
                1,		x_to_y,	x_to_z, 0,
                y_to_x, 1,		y_to_z, 0,
                z_to_x, z_to_y,	1,		0,
                0,		0,		0,		1
            };
            return mat;
        }
    }
}