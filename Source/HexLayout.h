#pragma once
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif



template <typename T>
struct HexLayout_t
{
    typedef HexPoint_t<T> PointType;

    const HexOrientation orientation;
    const PointType half;
    const PointType origin;

    HexLayout_t(const HexOrientation& orientation_, const HexPoint_t<T>& half_, const HexPoint_t<T>& origin_)
        : orientation(orientation_), half(half_), origin(origin_)
    {
    }

    template <typename U>
    HexPoint_t<U> GetCornerOffset(int corner) const
    {
        double angle = 2.0 * M_PI * (orientation.startRadian - corner) / 6;
        return HexPoint(half.x * std::cos(angle), half.y * std::sin(angle));
    }

    T GetUnitWidth() const
    {
        return GetCornerOffset<T>(0).x - GetCornerOffset<T>(3).x;
    }

    T GetUnitHeight() const
    {
        return GetCornerOffset<T>(5).y - GetCornerOffset<T>(2).y;
    }

    T GetHorizontalDistance() const
    {
        return GetUnitWidth() * orientation.widthToHorizonDistanceRatio;
    }

    T GetVerticalDistance() const
    {
        return GetUnitHeight() * orientation.heightToVertialDistanceRatio;
    }

    int ConvertIndexToQ(int x, int y) const
    {
        return orientation.indexToQ(x, y);
    }

    int ConvertIndexToR(int x, int y) const
    {
        return orientation.indexToR(x, y);
    }
};

