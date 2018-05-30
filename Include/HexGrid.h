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

        m_countX = (size_t)std::ceil((totalSize.x + gridWidth * 0.5f) / distanceH);
        m_countY = (size_t)std::ceil((totalSize.y + gridHeight * 0.5f) / distanceV);
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
        std::set<HexCube> grids;

        T minX = (std::min)(minIn.x, maxIn.x);
        T minY = (std::min)(minIn.y, maxIn.y);

        T maxX = (std::max)(minIn.x, maxIn.x);
        T maxY = (std::max)(minIn.y, maxIn.y);

        const PointType minPoint = PointType::Max(m_start, PointType(minX, minY));
        const PointType maxPoint = PointType::Min(PointType(maxX, maxY), m_end);

        T xInterval = m_layout.GetUnitWidth() * 0.5f;
        T yInterval = m_layout.GetUnitHeight() * 0.5f;

        T width = maxPoint.x - minPoint.x;
        T height = maxPoint.y - minPoint.y;

        int xCount = (int)std::ceil(width / xInterval);
        int yCount = (int)std::ceil(height / yInterval);

        for (int yIndex = 0; yIndex <= yCount; ++yIndex)
        {
            T y = (std::min)(minPoint.y + (yIndex * yInterval), maxPoint.y);

            for (int xIndex =0; xIndex <= xCount; ++xIndex)
            {
                T x = (std::min)(minPoint.x + (xIndex * xInterval), maxPoint.x);

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
    LayoutType m_layout;
    PointType m_start;
    PointType m_end;
    size_t m_countX;
    size_t m_countY;
};
