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

typedef HexPixel_t<int> HexPixelI;
typedef HexPixel_t<float> HexPixelF;
