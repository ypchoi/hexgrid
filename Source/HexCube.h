#pragma once



template <typename T>
struct HexCube_t
{
    T q;
    T r;
    T s;

    HexCube_t()
        : q(T()), r(T()), s(T())
    {
    }

    HexCube_t(T q_, T r_)
        : q(q_), r(r_), s(-q - r)
    {
    }

    HexCube_t(T q_, T r_, T s_)
        : q(q_), r(r_), s(s_)
    {
    }

    template <typename U>
    HexCube_t(const HexCube_t<U>& frac)
    {
        HexCube_t cube = HexCube_t::Round(frac);
        q = cube.q;
        r = cube.r;
        s = cube.s;
    }

    template <typename U>
    HexCube_t(const HexLayout_t<U>& layout, const HexPoint_t<U>& p)
    {
        static_assert(std::is_floating_point<T>::value, "Should be floating point");

        typedef HexPoint_t<U> PointType;

        const HexOrientation& M = layout.orientation;
        const PointType& half = layout.half;
        const PointType& origin = layout.origin;
        const PointType& point = PointType((p.x - origin.x) / half.x, (p.y - origin.y) / half.y);

        q = M.b0 * point.x + M.b1 * point.y;
        r = M.b2 * point.x + M.b3 * point.y;
        s = -q - r;
    }

    HexCube_t(const HexOffsetQ& h, int offset)
    {
        q = h.col;
        r = h.row - int((h.col + offset * (h.col & 1)) / 2);
        s = -q - r;
    }

    HexCube_t(const HexOffsetR& h, int offset)
    {
        q = h.col - int((h.row + offset * (h.row & 1)) / 2);
        r = h.row;
        s = -q - r;
    }

    ~HexCube_t() = default;

    template <typename U>
    HexPoint_t<U> GetCenter(const HexLayout_t<U>& layout) const
    {
        typedef HexPoint_t<U> PointType;

        const HexOrientation& M = layout.orientation;
        const PointType& half = layout.half;
        const PointType& origin = layout.origin;

        double x = (M.f0 * q + M.f1 * r) * half.x;
        double y = (M.f2 * q + M.f3 * r) * half.y;
        return PointType(x + origin.x, y + origin.y);
    }

    template <typename U>
    HexPoint_t<U> GetCorner(const HexLayout_t<U>& layout, int corner) const
    {
        return GetCenter(layout) + layout.GetCornerOffset<U>(corner);
    }

    template <typename U>
    std::vector<HexPoint_t<U>> GetCorners(const HexLayout_t<U>& layout) const
    {
        typedef HexPoint_t<U> PointType;

        PointType center = GetCenter(layout);

        std::vector<HexPoint_t<U>> corners;
        corners.reserve(6);

        for (int i = 0; i < 6; ++i)
        {
            corners.emplace_back(center + layout.GetCornerOffset<U>(i));
        }

        return corners;
    }

    HexCube_t GetDirection(int direction) const
    {
        static const std::vector<HexCube_t> s_directions =
        {
            HexCube_t(1, 0, -1), HexCube_t(1, -1, 0), HexCube_t(0, -1, 1),
            HexCube_t(-1, 0, 1), HexCube_t(-1, 1, 0), HexCube_t(0, 1, -1),
        };

        return s_directions[direction % 6];
    }

    HexCube_t GetNeighbor(int direction) const
    {
        return *this + GetDirection(direction);
    }

    std::vector<HexCube_t> GetNeighbors() const
    {
        std::vector<HexCube_t> neighbors;
        neighbors.reserve(6);

        for (int i = 0; i < 6; ++i)
        {
            neighbors.emplace_back(GetDirection(i));
        }

        return neighbors;
    }

    HexCube_t GetDiagonalNeighbor(int direction) const
    {
        static const std::vector<HexCube_t> s_diagonals =
        {
            HexCube_t(2, -1, -1), HexCube_t(1, -2, 1), HexCube_t(-1, -1, 2),
            HexCube_t(-2, 1, 1), HexCube_t(-1, 2, -1), HexCube_t(1, 1, -2),
        };

        return *this + s_diagonals[direction % 6];
    }

    HexOffsetQ GetOffsetQ(int offset) const
    {
        return HexOffsetQ(
            q,
            r + int((q + offset * (q & 1)) / 2));
    }

    HexOffsetR GetOffsetR(int offset) const
    {
        return HexOffsetR(
            q + int((r + offset * (r & 1)) / 2),
            r);
    }

    size_t GetLength() const
    {
        return size_t((std::abs(q) + std::abs(r) + std::abs(s)) / 2);
    }

    size_t GetDistance(const HexCube_t& rhs) const
    {
        return (*this - rhs).GetLength();
    }

    HexCube_t Lerp(const HexCube_t& rhs, double t) const
    {
        return HexCube_t::Lerp(*this, rhs, t);
    }

    std::vector<HexCube_t> GetLine(const HexCube_t& rhs) const
    {
        typedef HexCube_t<double> HexCubeD;
        HexCubeD offset(0.000001, 0.000001, -0.000002);
        HexCubeD nudgeA(q + offset.q, r + offset.r, s + offset.s);
        HexCubeD nudgeB(rhs.q + offset.q, rhs.r + offset.r, rhs.s + offset.s);

        size_t N = GetDistance(rhs);
        std::vector<HexCube_t> line;
        line.reserve(N);

        double step = 1.0 / std::max<double>(N, 1);
        for (size_t i = 0; i <= N; ++i)
        {
            HexCubeD frac = nudgeA.Lerp(nudgeB, step * i);
            line.emplace_back(HexCube_t(frac));
        }

        return line;
    }

    template <typename U>
    static HexCube_t Round(const HexCube_t<U>& frac)
    {
        static_assert(std::is_integral<T>::value, "Should be integral");
        static_assert(std::is_floating_point<U>::value, "Should be floating point");

        T q = T(std::round(frac.q));
        T r = T(std::round(frac.r));
        T s = T(std::round(frac.s));

        U diffQ = std::abs(q - frac.q);
        U diffR = std::abs(r - frac.r);
        U diffS = std::abs(s - frac.s);

        if (diffR < diffQ && diffS < diffQ)
        {
            q = -r - s;
        }
        else if (diffS < diffR)
        {
            r = -q - s;
        }
        else
        {
            s = -q - r;
        }

        return HexCube_t(q, r, s);
    }

    static HexCube_t Lerp(const HexCube_t& lhs, const HexCube_t& rhs, double t)
    {
        double invT = 1.0 - t;

        return HexCube_t(
            lhs.q * invT + rhs.q * t,
            lhs.r * invT + rhs.r * t,
            lhs.s * invT + rhs.s * t);
    }

    HexCube_t operator+(const HexCube_t& rhs) const
    {
        return HexCube_t(q + rhs.q, r + rhs.r, s + rhs.s);
    }

    HexCube_t operator-(const HexCube_t& rhs) const
    {
        return HexCube_t(q - rhs.q, r - rhs.r, s - rhs.s);
    }

    HexCube_t operator*(int k) const
    {
        return HexCube(q * k, r * k, s * k);
    }

    bool operator==(const HexCube_t& rhs) const
    {
        return std::tie(q, r, s) == std::tie(rhs.q, rhs.r, rhs.s);
    }

    bool operator!=(const HexCube_t& rhs) const
    {
        return !(*this == rhs);
    }

    bool operator<(const HexCube_t& rhs) const
    {
        return std::tie(q, r, s) < std::tie(rhs.q, rhs.r, rhs.s);
    }

    bool operator<=(const HexCube_t& rhs) const
    {
        return std::tie(q, r, s) <= std::tie(rhs.q, rhs.r, rhs.s);
    }
};
