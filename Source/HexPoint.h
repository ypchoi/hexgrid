#pragma once



template <typename T>
struct HexPoint_t
{
    typedef T ValueType;
    static_assert(std::is_floating_point<T>::value, "Should be floating point");

    T x;
    T y;

    HexPoint_t()
        : x(T()), y(T())
    {
    }

    HexPoint_t(T v)
        : x(v), y(v)
    {
    }

    HexPoint_t(T x_, T y_)
        : x(x_), y(y_)
    {
    }

    HexPoint_t operator+(const HexPoint_t& rhs) const
    {
        return HexPoint_t(x + rhs.x, y + rhs.y);
    }

    HexPoint_t operator-(const HexPoint_t& rhs) const
    {
        return HexPoint_t(x - rhs.x, y - rhs.y);
    }
};

