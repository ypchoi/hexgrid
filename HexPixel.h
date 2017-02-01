#pragma once

struct HexPixel
{
    float x;
    float y;

    HexPixel()
        : x(0.0f), y(0.0f)
    {
    }

    HexPixel(float x_, float y_)
        : x(x_), y(y_)
    {
    }

    ~HexPixel()
    {
    }
};
