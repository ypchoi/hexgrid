#pragma once
#include <cmath>


class HexMath
{
public:
    template<typename T>
    static const T Min(const T value)
    {
        return value;
    }

    template<typename T, typename... TArgs>
    static const T Min(const T lhs, const TArgs... args)
    {
        const T rhs = Min(args...);
        return lhs < rhs ? lhs : rhs;
    }

    template<typename T>
    static const T Max(const T value)
    {
        return value;
    }

    template<typename T, typename... TArgs>
    static const T Max(const T lhs, const TArgs... args)
    {
        T rhs = Max(args...);
        return lhs < rhs ? rhs : lhs;
    }

    template<typename T>
    static T Abs(const T value)
    {
        return (value < 0) ? -value : value;
    }

    template<typename T>
    static T Round(const T value)
    {
        return std::round(value);
    }

    template<typename T>
    static T Ceil(const T value)
    {
        return std::ceil(value);
    }

    template<typename T>
    static T Floor(const T value)
    {
        return std::floor(value);
    }

    template<typename T>
    static T SnapDown(const T value, const T grid)
    {
        return Floor(value / grid) * grid;
    }

    template<typename T>
    static T SnapUp(const T value, const T grid)
    {
        return Ceil(value / grid) * grid;
    }

    template<typename T>
    static bool NearlyEquals(const T lhs, const T rhs, const T epsilon = 0.000001f)
    {
        T diff = Abs(rhs - lhs);
        return (diff <= epsilon);
    }
};
