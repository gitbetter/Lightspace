#pragma once

#include <limits>
#include <stdexcept>
#include <type_traits>
#include <cmath>
#include <string>
#include <atomic>

#if DOUBLE_PRECISION
using fpnum = double;
#else
using fpnum = float;
#endif

namespace ls {
    static constexpr float epsilon = std::numeric_limits<fpnum>::epsilon() * 100;
    static constexpr float infinity = std::numeric_limits<fpnum>::infinity();
    static constexpr float pi = 3.14159265358979323846f;
    static constexpr float pi_over_2 = pi * 0.5f;
    static constexpr float pi_over_3 = pi * 0.33333f;
    static constexpr float pi_over_4 = pi * 0.25f;
    static constexpr float pi_over_5 = pi * 0.2f;
    static constexpr float pi_over_6 = pi * 0.16667f;
    static constexpr float two_pi = pi * 2.f;

    template<
        typename T,
        typename = std::enable_if_t<std::is_arithmetic<T>::value>
    >
        inline bool approx( const T& lhs, const T& rhs ) noexcept
    {
        return fabs( static_cast<fpnum>( lhs ) - static_cast<fpnum>( rhs ) ) <= epsilon;
    }

    template<typename T>
    inline T clamp( T x, T a, T b ) noexcept
    {
        return x > b ? b : ( x < a ? a : x );
    }

    inline uint32_t get_uid() noexcept
    {
        static std::atomic_uint32_t current_uid = 0;
        return ++current_uid;
    }

    template<
        typename T,
        typename = std::enable_if_t<std::is_arithmetic<T>::value>
    >
        inline uint8_t sign( const T& num ) noexcept
    {
        return num < 0 ? -1 : 1;
    }

    class method_not_supported : public std::exception
    {
    public:

        const char* what() const throw( )
        {
            return "This method is not supported for the given class instantiation.";
        }

    };
}