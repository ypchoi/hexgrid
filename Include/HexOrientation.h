#pragma once
#include <functional>

#define SQRT3 (std::sqrt(3))



struct HexOrientation
{
    typedef std::function<int(int, int)> TConvertFunc;

    const double f0;
    const double f1;
    const double f2;
    const double f3;
    const double b0;
    const double b1;
    const double b2;
    const double b3;
    const double startRadian;
    const double widthToHorizonDistanceRatio;
    const double heightToVertialDistanceRatio;
    const TConvertFunc indexToQ;
    const TConvertFunc indexToR;

    HexOrientation(double f0_, double f1_, double f2_, double f3_, double b0_, double b1_, double b2_, double b3_, double startRadian_, double widthToHorizonDistanceRatio_, double heightToVertialDistanceRatio_, TConvertFunc indexToQ_, TConvertFunc indexToR_)
        : f0(f0_), f1(f1_), f2(f2_), f3(f3_)
        , b0(b0_), b1(b1_), b2(b2_), b3(b3_)
        , startRadian(startRadian_)
        , widthToHorizonDistanceRatio(widthToHorizonDistanceRatio_), heightToVertialDistanceRatio(heightToVertialDistanceRatio_)
        , indexToQ(indexToQ_), indexToR(indexToR_)
    {
    }

    static HexOrientation Pointy()
    {
        TConvertFunc indexToQ = [](int x, int y)
        {
            return -(y / 2) + x;
        };

        TConvertFunc indexToR = [](int /*x*/, int y)
        {
            return y;
        };

        return HexOrientation(
            SQRT3, SQRT3 * 0.5, 0.0, 1.5,
            SQRT3 / 3.0, -1.0 / 3.0, 0.0, 2.0 / 3.0,
            0.5,
            1.0, 0.75,
            indexToQ, indexToR);
    }

    static HexOrientation Flat()
    {
        TConvertFunc indexToQ = [](int x, int /*y*/)
        {
            return x;
        };

        TConvertFunc indexToR = [](int x, int y)
        {
            return -(x / 2) + y;
        };

        return HexOrientation(
            1.5, 0.0, SQRT3 * 0.5, SQRT3,
            2.0 / 3.0, 0.0, -1.0 / 3.0, SQRT3 / 3.0,
            0.0,
            0.75, 1.0,
            indexToQ, indexToR);
    }
};
