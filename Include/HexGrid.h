#pragma once



template <typename LayoutType>
class HexGrid_t
{
    typedef typename LayoutType::PointType PointType;
    typedef typename LayoutType::PointType::ValueType T;
    typedef HexCube_t<int> HexCube;

public:
    HexGrid_t(const LayoutType& layout)
        : m_layout(layout), m_countX(0), m_countY(0)
    {
    }

    ~HexGrid_t() = default;

    bool Create(const PointType& totalSize)
    {
        if (totalSize.x <= 0 || totalSize.y <= 0)
            return false;

        m_start = m_layout.origin;
        m_end = m_start + totalSize;

        T gridWidth = m_layout.GetUnitWidth();
        T gridHeight = m_layout.GetUnitHeight();
        T distanceH = m_layout.GetHorizontalDistance();
        T distanceV = m_layout.GetVerticalDistance();

        m_countX = (size_t)HexMath::Ceil((totalSize.x + gridWidth * 0.5f) / distanceH);
        m_countY = (size_t)HexMath::Ceil((totalSize.y + gridHeight * 0.5f) / distanceV);
        return true;
    }

    bool GetGrid(__out HexCube& out, const PointType& pixel) const
    {
        if (!IsInside(pixel))
            return false;

        out = HexCube(HexCube_t<double>(m_layout, pixel));
        return true;
    }

    typedef std::function<void(HexCube)> TForEachFunc;
    void ForEach(TForEachFunc func)
    {
        for (int y = 0; y < static_cast<int>(m_countY); ++y)
        {
            for (int x = 0; x < static_cast<int>(m_countX); ++x)
            {
                int q = m_layout.ConvertIndexToQ(x, y);
                int r = m_layout.ConvertIndexToR(x, y);
                func(HexCube(q, r));
            }
        }
    }

    bool IsInside(const PointType& point) const
    {
        return m_start.x <= point.x && point.x <= m_end.x
            && m_start.y <= point.y && point.y <= m_end.y;
    }

    std::set<HexCube> IntersectBox(const PointType& minIn, const PointType& maxIn) const
    {
        T minX = HexMath::Min(minIn.x, maxIn.x, m_end.x);
        T minY = HexMath::Min(minIn.y, maxIn.y, m_end.y);
        T maxX = HexMath::Max(minIn.x, maxIn.x, m_start.x);
        T maxY = HexMath::Max(minIn.y, maxIn.y, m_start.y);
        T xInterval = m_layout.GetUnitWidth() * 0.5f;
        T yInterval = m_layout.GetUnitHeight() * 0.5f;

        std::vector<T> snapsX = GetSnaps(minX, maxX, xInterval);
        std::vector<T> snapsY = GetSnaps(minY, maxY, yInterval);

        std::set<HexCube> grids;
        for (T y : snapsY)
        {
            for (T x : snapsX)
            {
                HexCube grid;
                if (GetGrid(grid, PointType(x, y)))
                {
                    grids.insert(grid);
                }
            }
        }

        return grids;
    }

    const LayoutType& GetLayout() const { return m_layout; }
    const PointType& GetStart() const { return m_start; }
    const PointType& GetEnd() const { return m_end; }

private:
    std::vector<T> GetSnaps(T min, T max, T grid) const
    {
        T snapMin = HexMath::SnapUp(min, grid);
        T snapMax = HexMath::SnapDown(max, grid);

        std::vector<T> snaps;
        snaps.reserve(HexMath::Ceil((max - min) / grid) + 1);

        if (HexMath::NearlyEquals(min, snapMin) == false)
            snaps.push_back(min);

        int count = HexMath::Round((snapMax - snapMin) / grid);
        for (int i = 0; i <= count; ++i)
        {
            T t = snapMin + (i * grid);
            snaps.push_back(t);
        }

        if (HexMath::NearlyEquals(max, snapMax) == false)
            snaps.push_back(max);

        return snaps;
    }

private:
    LayoutType m_layout;
    PointType m_start;
    PointType m_end;
    size_t m_countX;
    size_t m_countY;
};
