#include "HexConvert.h"
#include "HexPixel.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


template <class T>
struct HexGrid_t<T, eHexGridShape::PointyTopped>
{
    using Index = HexIndex_t<int>;
    using Pixel = HexPixel_t<T>;
    static const eHexGridShape Shape = eHexGridShape::PointyTopped;

    const Index index;
    const Pixel center;
    const T radius;

    HexGrid_t()
        : index({}), center({}), radius(T())
    {
    }

    HexGrid_t(const Pixel& offset, const Index& index_, T radius_)
        : index(index_)
        , center(offset + HexConvert<Shape>::template ToPixel<Index::ValueType, T>(index_, radius_))
        , radius(radius_)
    {
    }

    ~HexGrid_t()
    {
    }

    float GetWidth() const
    {
        return GetWidth(radius);
    }

    float GetHeight() const
    {
        return GetHeight(radius);
    }

    template <class INDEX>
    Pixel GetDistance(const INDEX& other) const
    {
        Pixel otherCenter = HexConvert<Shape>::template ToPixel<Pixel>(other, radius);
        return Pixel(otherCenter.x - center.x, otherCenter.y - center.y);
    }

    Pixel GetCorner(int i) const
    {
        float radian = (60.0f * i + 30.0f) * (static_cast<float>(M_PI) / 180.0f);
        float x = center.x + radius * std::cos(radian);
        float y = center.y + radius * std::sin(radian);
        return Pixel(x, y);
    }

    static float GetWidth(float radius)
    {
        return std::sqrt(3.0f) * 0.5f * GetHeight(radius);
    }

    static float GetHeight(float radius)
    {
        return radius * 2.0f;
    }

    static float GetHorizontalDistance(float radius)
    {
        return GetWidth(radius);
    }

    static float GetVerticalDistance(float radius)
    {
        return GetHeight(radius) * 0.75f;
    }
};
