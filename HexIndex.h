#pragma once
template <class T>
struct HexCubeIndex_t;

template <class T>
struct HexIndex_t
{
    const T q;
    const T r;

    typedef T ValueType;
    typedef std::array<HexIndex_t<T>, 6> NeighborType;

    HexIndex_t()
        : q(T()), r(T())
    {
    }

    HexIndex_t(const T& q_, const T& r_)
        : q(q_), r(r_)
    {
    }

    ~HexIndex_t()
    {
    }

    NeighborType GetNeighbors() const
    {
        static const NeighborType directions =
        {
            HexIndex_t(1, 0), HexIndex_t(1, -1), HexIndex_t(0, -1),
            HexIndex_t(-1, 0), HexIndex_t(-1, 1), HexIndex_t(0, 1),
        };

        NeighborType neighbors =
        {
            *this + directions[0], *this + directions[1], *this + directions[2],
            *this + directions[3], *this + directions[4], *this + directions[5],
        };

        return neighbors;
    }

    int GetCost(const HexIndex_t& rhs) const
    {
        return (std::abs(q - rhs.q) + std::abs(q + r - rhs.q - rhs.r) + std::abs(r - rhs.r)) / 2;
    }
};

template <class T>
HexIndex_t<T> operator+(const HexIndex_t<T>& lhs, const HexIndex_t<T>& rhs)
{
    return HexIndex_t<T>(lhs.q + rhs.q, lhs.r + rhs.r);
}

typedef HexIndex_t<int> HexIndexI;
typedef HexIndex_t<float> HexIndexF;