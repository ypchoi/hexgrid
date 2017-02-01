#pragma once

template <class T>
struct HexCubeIndex
{
    const T x;
    const T y;
    const T z;

    typedef T TType;
    typedef std::array<HexCubeIndex<T>, 6> TNeighbors;

    HexCubeIndex()
        : x(T()), y(T()), z(T())
    {
    }

    HexCubeIndex(const T& x_, const T& y_, const T& z_)
        : x(x_), y(y_), z(z_)
    {
    }

    ~HexCubeIndex()
    {
    }

    TNeighbors GetNeighbors() const
    {
        static const TNeighbors directions =
        {
            HexCubeIndex(1, -1, 0), HexCubeIndex(1, 0, -1), HexCubeIndex(0, 1, -1),
            HexCubeIndex(-1, 1, 0), HexCubeIndex(-1, 0, 1), HexCubeIndex(0, -1, 1),
        };

        TNeighbors neighbors =
        {
            *this + directions[0], *this + directions[1], *this + directions[2],
            *this + directions[3], *this + directions[4], *this + directions[5],
        };

        return neighbors;
    }

    int GetCost(const HexCubeIndex& rhs) const
    {
        return (std::abs(x - rhs.x) + std::abs(y - rhs.y) + std::abs(z - rhs.z)) / 2;
    }
};

typedef HexCubeIndex<int> HexCubeIndexI;
typedef HexCubeIndex<float> HexCubeIndexF;

