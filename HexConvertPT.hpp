template <>
struct HexConvert<eHexGridShape::PointyTopped>
{
    template <class TIn, class TOut = TIn>
    static HexIndex_t<TOut> ToHex(const HexPixel_t<TIn>& p, TIn radius)
    {
        TIn q = ((p.x * std::sqrt(3.0f) - p.y) / 3.0f) / radius;
        TIn r = ((p.y * 2.0f) / 3.0f) / radius;
        return Round<TIn, TOut>(HexIndex_t<TIn>(q, r));
    }

    template <class TIn, class TOut = TIn>
    static HexIndex_t<TOut> ToHex(const HexCubeIndex_t<TIn>& cube)
    {
        return HexIndex_t<TOut>(cube.x, cube.z);
    }

    template <class TIn, class TOut = TIn>
    static HexCubeIndex_t<TOut> ToCube(const HexIndex_t<TIn>& hex)
    {
        TOut x = hex.q;
        TOut z = hex.r;
        TOut y = -x - z;
        return HexCubeIndex_t<TOut>(x, y, z);
    }

    template <class TIn, class TOut = TIn>
    static HexPixel_t<TOut> ToPixel(const HexIndex_t<TIn>& hex, TIn radius)
    {
        TOut x = radius * std::sqrt(3.0f) * (hex.q + hex.r * 0.5f);
        TOut y = radius * 1.5f * hex.r;
        return HexPixel_t<TOut>(x, y);
    }

    template <class TIn, class TOut = TIn>
    static HexPixel_t<TOut> ToPixel(const HexCubeIndex_t<TIn>& cube, TIn radius)
    {
        HexIndex_t<TIn> hex(cube);
        return ToPixel<TOut>(hex, radius);
    }

    template <class TIn, class TOut>
    static HexCubeIndex_t<TOut> Round(const HexCubeIndex_t<TIn>& rhs)
    {
        TOut rx = static_cast<TOut>(std::round(rhs.x));
        TOut ry = static_cast<TOut>(std::round(rhs.y));
        TOut rz = static_cast<TOut>(std::round(rhs.z));

        auto dx = std::abs(rx - rhs.x);
        auto dy = std::abs(ry - rhs.y);
        auto dz = std::abs(rz - rhs.z);

        if (dy < dx && dz < dx)
            rx = -ry - rz;
        else if (dz < dy)
            ry = -rx - rz;
        else
            rz = -rx - ry;

        return HexCubeIndex_t<TOut>(rx, ry, rz);
    }

    template <class TIn, class TOut>
    static HexIndex_t<TOut> Round(const HexIndex_t<TIn>& rhs)
    {
        HexCubeIndex_t<TIn> cube = ToCube(rhs);
        HexCubeIndex_t<TOut> rounded = Round<TIn, TOut>(cube);
        return ToHex(rounded);
    }
};

typedef HexConvert<eHexGridShape::PointyTopped> HexConvertPT;
