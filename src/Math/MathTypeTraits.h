#ifndef Magnum_Math_MathTypeTraits_h
#define Magnum_Math_MathTypeTraits_h
/*
    Copyright © 2010, 2011, 2012 Vladimír Vondruš <mosra@centrum.cz>

    This file is part of Magnum.

    Magnum is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License version 3
    only, as published by the Free Software Foundation.

    Magnum is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License version 3 for more details.
*/

/** @file
 * @brief Class Magnum::Math::MathTypeTraits
 */

#include <cstdint>
#include <cmath>

/** @brief Precision when testing floats for equality */
#ifndef FLOAT_EQUALITY_PRECISION
#define FLOAT_EQUALITY_PRECISION 1.0e-6
#endif

/** @brief Precision when testing doubles for equality */
#ifndef DOUBLE_EQUALITY_PRECISION
#define DOUBLE_EQUALITY_PRECISION 1.0e-12
#endif

/**
@brief Precision when testing long doubles for equality
@todo some proper value please
*/
#ifndef LONG_DOUBLE_EQUALITY_PRECISION
#define LONG_DOUBLE_EQUALITY_PRECISION 1.0e-18
#endif

namespace Magnum { namespace Math {

#ifndef DOXYGEN_GENERATING_OUTPUT
namespace Implementation {
    template<class T> struct MathTypeTraitsDefault {
        MathTypeTraitsDefault() = delete;

        inline constexpr static bool equals(T a, T b) {
            return a == b;
        }
    };
}
#endif

/**
@brief Traits class for numeric types

Traits classes are usable for detecting type features at compile time without
the need for repeated code such as method overloading or template
specialization for given types.
*/
template<class T> struct MathTypeTraits: Implementation::MathTypeTraitsDefault<T> {
    /*
     * The following values are implemented as inline functions, not as
     * static const variables, because the compiler will inline the return
     * values instead of referencing to static data and unlike static const
     * variables the functions can be overloaded, deleted and hidden.
     */

    #ifdef DOXYGEN_GENERATING_OUTPUT
    /**
     * @brief Corresponding floating-point type for normalization
     *
     * If the type is not already floating-point, defines smallest larger
     * floating-point type.
     */
    typedef U FloatingPointType;

    /**
     * @brief Epsilon value for fuzzy compare
     *
     * Returns minimal difference between numbers to be considered
     * inequal. Returns 1 for integer types and reasonably small value for
     * floating-point types. Not implemented for arbitrary types.
     */
    inline constexpr static T epsilon();

    /**
     * @brief Fuzzy compare
     *
     * Uses fuzzy compare for floating-point types (using epsilon() value),
     * pure equality comparison everywhere else.
     */
    static bool equals(T a, T b);
    #endif
};

/** @bug Infinity comparison! */

/**
 * @todo Implement better fuzzy comparison algorithm, like at
 * http://floating-point-gui.de/errors/comparison/ or
 * http://www.cygnus-software.com/papers/comparingfloats/comparingfloats.htm
 */

#ifndef DOXYGEN_GENERATING_OUTPUT
/* Integral scalar types */
namespace Implementation {
    template<class T> struct MathTypeTraitsIntegral: MathTypeTraitsDefault<T> {
        inline constexpr static T epsilon() { return T(1); }
    };
}

template<> struct MathTypeTraits<std::uint8_t>: Implementation::MathTypeTraitsIntegral<std::uint8_t> {
    typedef float FloatingPointType;
};
template<> struct MathTypeTraits<std::int8_t>: Implementation::MathTypeTraitsIntegral<std::int8_t> {
    typedef float FloatingPointType;
};
template<> struct MathTypeTraits<std::uint16_t>: Implementation::MathTypeTraitsIntegral<std::uint16_t> {
    typedef float FloatingPointType;
};
template<> struct MathTypeTraits<std::int16_t>: Implementation::MathTypeTraitsIntegral<std::int16_t> {
    typedef float FloatingPointType;
};
template<> struct MathTypeTraits<std::uint32_t>: Implementation::MathTypeTraitsIntegral<std::uint32_t> {
    typedef double FloatingPointType;
};
template<> struct MathTypeTraits<std::int32_t>: Implementation::MathTypeTraitsIntegral<std::int32_t> {
    typedef double FloatingPointType;
};
template<> struct MathTypeTraits<std::uint64_t>: Implementation::MathTypeTraitsIntegral<std::uint64_t> {
    typedef long double FloatingPointType;
};
template<> struct MathTypeTraits<std::int64_t>: Implementation::MathTypeTraitsIntegral<std::int64_t> {
    typedef long double FloatingPointType;
};

/* Floating-point scalar types */
namespace Implementation {
    template<class T> struct MathTypeTraitsFloatingPoint {
        MathTypeTraitsFloatingPoint() = delete;

        inline static bool equals(T a, T b) {
            return std::abs(a - b) < MathTypeTraits<T>::epsilon();
        }
    };
}

template<> struct MathTypeTraits<float>: Implementation::MathTypeTraitsFloatingPoint<float> {
    typedef float FloatingPointType;

    inline constexpr static float epsilon() { return FLOAT_EQUALITY_PRECISION; }
};
template<> struct MathTypeTraits<double>: Implementation::MathTypeTraitsFloatingPoint<double> {
    typedef double FloatingPointType;

    inline constexpr static double epsilon() { return DOUBLE_EQUALITY_PRECISION; }
};
template<> struct MathTypeTraits<long double>: Implementation::MathTypeTraitsFloatingPoint<long double> {
    typedef long double FloatingPointType;

    inline constexpr static long double epsilon() { return LONG_DOUBLE_EQUALITY_PRECISION; }
};
#endif

}}

#endif
