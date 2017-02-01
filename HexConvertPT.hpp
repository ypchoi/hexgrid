template <>
struct HexConvert<eHexGridType::PointyTopped>
{
    template <class T>
    static HexIndex<T> ToHex(const T& x, const T& y, float radius)
    {
        T q = x * std::sqrt(3.0f) / 3.0f - y / 3.0f;
        T r = ((y * 2.0f) / 3.0f) / radius;
        return HexIndex(q, r);
    }

    template <class FROM, class TO = FROM>
    static HexIndex<TO> ToHex(const HexCubeIndex<FROM>& cube)
    {
        return HexIndex<TO>(cube.x, cube.z);
    }

    template <class FROM, class TO = FROM>
    static HexCubeIndex<TO> ToCube(const HexIndex<FROM>& hex)
    {
        TO x = hex.q;
        TO z = hex.r;
        TO y = -x - z;
        return HexCubeIndex<TO>(x, y, z);
    }

    template <class PIXEL, class T>
    static PIXEL ToPixel(const HexIndex<T>& hex, float radius)
    {
        PIXEL pixel;
        pixel.x = radius * std::sqrt(3.0f) * (hex.q + hex.r * 0.5f);
        pixel.y = radius * 1.5f * hex.r;
        return pixel;
    }

    template <class PIXEL, class T>
    static PIXEL ToPixel(const HexCubeIndex<T>& cube, float radius)
    {
        HexIndex<T> hex(cube);
        return ToPixel<T, PIXEL>(hex, radius);
    }

    static HexCubeIndexI Round(const HexCubeIndexF& rhs)
    {
        int rx = static_cast<int>(std::round(rhs.x));
        int ry = static_cast<int>(std::round(rhs.y));
        int rz = static_cast<int>(std::round(rhs.z));

        float dx = std::abs(rx - rhs.x);
        float dy = std::abs(ry - rhs.y);
        float dz = std::abs(rz - rhs.z);

        if (dy < dx && dz < dx)
            rx = -ry - rz;
        else if (dz < dy)
            ry = -rx - rz;
        else
            rz = -rx - ry;

        return HexCubeIndexI(rx, ry, rz);
    }

    static HexIndexI Round(const HexIndexF& rhs)
    {
        HexCubeIndexF cubef = ToCube(rhs);
        HexCubeIndexI cubei = Round(cubef);
        return ToHex<int, int>(cubei);
    }
};

typedef HexConvert<eHexGridType::PointyTopped> HexConvertPT;
