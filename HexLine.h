#pragma once

template <class T>
struct HexIndex_t;

template <class T>
struct HexCubeIndex_t;

struct HexLine
{
    template <class T>
    static std::list<HexCubeIndex_t<T>> Get(const HexCubeIndex_t<T>& begin, const HexCubeIndex_t<T>& end)
    {
        std::list<HexCubeIndex_t<T>> path;

        int cost = end.GetCost(begin);
        for (int i = 0; i <= cost; ++i)
        {
            float t = (i * 1.0f) / cost;
            HexCubeIndex_t<float> f = Lerp<T, float>(begin, end, t);
            HexCubeIndex_t<T> p = HexConvertPT::Round<float, T>(f);
            path.push_back(p);
        }

        return path;
    }

    template <class TIn, class TOut>
    static HexCubeIndex_t<TOut> Lerp(const HexCubeIndex_t<TIn>& a, const HexCubeIndex_t<TIn>& b, float t)
    {
        return HexCubeIndex_t<TOut>(
            Lerp(a.x, b.x, t),
            Lerp(a.y, b.y, t),
            Lerp(a.z, b.z, t));
    }

    static float Lerp(float a, float b, float t)
    {
        return a + (b - a) * t;
    }
};
