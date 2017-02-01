#pragma once
template <class T>
struct HexCubeIndex;

template <class T>
struct HexIndex
{
    const T q;
    const T r;

    typedef T TType;
    typedef std::array<HexIndex<T>, 6> TNeighbors;

    HexIndex()
        : q(T()), r(T())
    {
    }

    HexIndex(const T& q_, const T& r_)
        : q(q_), r(r_)
    {
    }

    ~HexIndex()
    {
    }

    TNeighbors GetNeighbors() const
    {
        static const TNeighbors directions =
        {
            HexIndex(1, 0), HexIndex(1, -1), HexIndex(0, -1),
            HexIndex(-1, 0), HexIndex(-1, 1), HexIndex(0, 1),
        };

        TNeighbors neighbors =
        {
            *this + directions[0], *this + directions[1], *this + directions[2],
            *this + directions[3], *this + directions[4], *this + directions[5],
        };

        return neighbors;
    }

    int GetCost(const HexIndex& rhs) const
    {
        return (std::abs(q - rhs.q) + std::abs(q + r - rhs.q - rhs.r) + std::abs(r - rhs.r)) / 2;
    }
};

typedef HexIndex<int> HexIndexI;
typedef HexIndex<float> HexIndexF;
