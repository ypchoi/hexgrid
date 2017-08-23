#pragma once

template <class T>
struct HexPixel_t
{
    typedef T ValueType;

    T x;
    T y;

    HexPixel_t()
        : x(T()), y(T())
    {
    }

    HexPixel_t(const T& x_, const T& y_)
        : x(x_), y(y_)
    {
    }

    ~HexPixel_t()
    {
    }
};

template <class T>
HexPixel_t<T> operator+(const HexPixel_t<T>& lhs, const HexPixel_t<T>& rhs)
{
    return HexPixel_t<T>(lhs.x + rhs.x, lhs.y + rhs.y);
}

template <class T>
HexPixel_t<T> operator-(const HexPixel_t<T>& lhs, const HexPixel_t<T>& rhs)
{
    return HexPixel_t<T>(lhs.x - rhs.x, lhs.y - rhs.y);
}

typedef HexPixel_t<int> HexPixelI;
typedef HexPixel_t<float> HexPixelF;
