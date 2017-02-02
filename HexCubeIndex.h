#pragma once

template <class T>
struct HexCubeIndex_t
{
    const T x;
    const T y;
    const T z;

    typedef T ValueType;
    typedef std::array<HexCubeIndex_t<T>, 6> NeighborType;

    HexCubeIndex_t()
        : x(T()), y(T()), z(T())
    {
    }

    HexCubeIndex_t(const T& x_, const T& y_, const T& z_)
        : x(x_), y(y_), z(z_)
    {
    }

    ~HexCubeIndex_t()
    {
    }

    NeighborType GetNeighbors() const
    {
        static const NeighborType directions =
        {
            HexCubeIndex_t(1, -1, 0), HexCubeIndex_t(1, 0, -1), HexCubeIndex_t(0, 1, -1),
            HexCubeIndex_t(-1, 1, 0), HexCubeIndex_t(-1, 0, 1), HexCubeIndex_t(0, -1, 1),
        };

        NeighborType neighbors =
        {
            *this + directions[0], *this + directions[1], *this + directions[2],
            *this + directions[3], *this + directions[4], *this + directions[5],
        };

        return neighbors;
    }

    int GetCost(const HexCubeIndex_t& rhs) const
    {
        return (std::abs(x - rhs.x) + std::abs(y - rhs.y) + std::abs(z - rhs.z)) / 2;
    }
};

template <class T>
HexCubeIndex_t<T> operator+(const HexCubeIndex_t<T>& lhs, const HexCubeIndex_t<T>& rhs)
{
    return HexCubeIndex_t<T>(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}

typedef HexCubeIndex_t<int> HexCubeIndexI;
typedef HexCubeIndex_t<float> HexCubeIndexF;

