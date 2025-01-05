#pragma once
#include <cstdlib>
#include <cstring>
#include <type_traits>


template<typename T>
concept Arithmetic = std::is_arithmetic_v<T>;

template<typename T>
concept Integral = std::is_integral_v<T>;

template<typename T>
concept SupportsAddition = requires (T a, T b) {
    { a + b } -> std::convertible_to<T>;
};

template<typename T>
concept SupportsAdditionAssignment = requires (T a, T b) {
    { a += b } -> std::convertible_to<T>;
};


template<typename T>
concept SupportsSubtraction = requires (T a, T b) {
    { a - b } -> std::convertible_to<T>;
};

template<typename T>
concept SupportsSubtractionAssignment = requires (T a, T b) {
    { a -= b } -> std::convertible_to<T>;
};

template<typename T>
concept SupportsMultiplication = requires (T a, T b) {
    { a * b } -> std::convertible_to<T>;
};

template<typename T>
concept SupportsDivision = requires (T a, T b) {
    { a / b } -> std::convertible_to<T>;
};


template<typename T>
concept SupportsModulo = requires (T a, T b) {
    { a % b } -> std::convertible_to<T>;
};

template<typename T>
concept SupportsIncrement = requires (T a) {
    { a++ } -> std::convertible_to<T>;
};

template<typename T>
concept SupportsDecrement = requires (T a) {
    { a-- } -> std::convertible_to<T>;
};

template<typename E>
struct FEnableBitMaskOperators {
    static constexpr bool enable = false;
};

template<typename E>
typename std::enable_if_t<FEnableBitMaskOperators<E>::enable, E> 
operator|(E lhs,E rhs) 
{
    return static_cast<E>(
        static_cast<std::underlying_type_t<E>>(lhs) |
        static_cast<std::underlying_type_t<E>>(rhs)
    );
}

template<typename E, typename OE>
typename std::enable_if_t<FEnableBitMaskOperators<E>::enable, E> 
operator|(E lhs, OE rhs) 
{
    return static_cast<E>(
        static_cast<std::underlying_type_t<E>>(lhs) |
        static_cast<std::underlying_type_t<OE>>(rhs)
    );
}

template<typename E>
typename std::enable_if_t<FEnableBitMaskOperators<E>::enable, E> 
operator|(int lhs,E rhs) 
{
    return static_cast<E>(
        static_cast<std::underlying_type_t<E>>(lhs) |
        static_cast<std::underlying_type_t<E>>(rhs)
    );
}

template<typename E>
typename std::enable_if_t<FEnableBitMaskOperators<E>::enable, E> 
operator|(E lhs,int rhs) 
{
    return static_cast<E>(
        static_cast<std::underlying_type_t<E>>(lhs) |
        static_cast<std::underlying_type_t<E>>(rhs)
    );
}

template<typename E>
typename std::enable_if_t<FEnableBitMaskOperators<E>::enable, E> 
operator&(E lhs,E rhs) 
{
    return static_cast<E>(
        static_cast<std::underlying_type_t<E>>(lhs) |
        static_cast<std::underlying_type_t<E>>(rhs)
    );
}

template<typename E>
typename std::enable_if_t<FEnableBitMaskOperators<E>::enable, E> 
operator<<(E lhs,E rhs) 
{
    return static_cast<E>(
        static_cast<std::underlying_type_t<E>>(lhs) |
        static_cast<std::underlying_type_t<E>>(rhs)
    );
}

template<typename E>
typename std::enable_if_t<FEnableBitMaskOperators<E>::enable, E> 
operator<<(E lhs, int rhs) 
{
    return static_cast<E>(
        static_cast<std::underlying_type_t<E>>(lhs) |
        static_cast<std::underlying_type_t<E>>(rhs)
    );
}

template<typename E>
typename std::enable_if_t<FEnableBitMaskOperators<E>::enable, E> 
operator<<(int lhs, E rhs) 
{
    return static_cast<E>(
        static_cast<std::underlying_type_t<E>>(lhs) |
        static_cast<std::underlying_type_t<E>>(rhs)
    );
}

template<typename E>
typename std::enable_if_t<FEnableBitMaskOperators<E>::enable, E> 
operator~(E lhs) 
{
    return static_cast<E>(
        ~static_cast<std::underlying_type_t<E>>(lhs)
    );
}