#pragma once

template <class TGrid>
class HexGridManager
{
    typedef unsigned __int32 uint32;
    typedef typename TGrid::Index Index;
    typedef typename TGrid::Pixel Pixel;
    typedef typename TGrid::Pixel::ValueType T;
    static const eHexGridShape Shape = TGrid::Shape;

public:
    HexGridManager()
        : m_radius(T()), m_countX(0), m_countY(0)
    {
    }

    ~HexGridManager()
    {
    }

    bool Create(const Pixel& start, const Pixel& end, T radius)
    {
        Release();

        if (end.x <= start.x || end.y <= start.y)
            return false;

        m_start = start;
        m_end = end;
        m_radius = radius;

        T totalWidth = end.x - start.x;
        T totalHeight = end.y - start.y;

        T gridWidth = TGrid::GetWidth(radius);
        T gridHeight = TGrid::GetHeight(radius);

        T gridDistanceH = TGrid::GetHorizontalDistance(radius);
        T gridDistanceV = TGrid::GetVerticalDistance(radius);

        uint32 countX = (uint32)std::ceil((totalWidth + gridWidth * 0.5f) / gridDistanceH);
        uint32 countY = (uint32)std::ceil((totalHeight + gridWidth * 0.5f) / gridDistanceV);

        m_pGrids.reset(new TGrid[countX * countY]);
        if (!m_pGrids)
            return false;

        m_countX = countX;
        m_countY = countY;

        for (int y = 0; y < countY; ++y)
        {
            for (int x = 0; x < countX; ++x)
            {
                int arrayIndex = (y  * countX) + x;
                TGrid::Index gridIndex = ArrayToHex(arrayIndex);
                TGrid& grid = m_pGrids[arrayIndex];
                new (&grid) TGrid(gridIndex, radius);
            }
        }

        return true;
    }

    void Release()
    {
        m_start = Pixel();
        m_end = Pixel();
        m_radius = T();
        m_pGrids.reset();
        m_countX = 0;
        m_countY = 0;
    }

    const TGrid* GetGrid(const Index& index) const
    {
        int arrayIndex = HexToArray(index);
        if (arrayIndex < 0)
            return nullptr;

        return &(m_pGrids[arrayIndex]);
    }

    const TGrid* GetGrid(const Pixel& pixel) const
    {
        Pixel offsetted = pixel - m_start;
        Index index = HexConvert<Shape>::ToHex<Pixel::ValueType, Index::ValueType>(offsetted, m_radius);
        return GetGrid(index);
    }

    void ForEach(std::function<bool(const TGrid&)> func)
    {
        for (uint32 y = 0; y < m_countY; ++y)
        {
            for (uint32 x = 0; x < m_countX; ++x)
            {
                int arrayIndex = (y * m_countX) + x;
                if (!func(m_pGrids[arrayIndex]))
                    return;
            }
        }
    }

    const Pixel& GetStart() const { return m_start; }
    const Pixel& GetEnd() const { return m_end; }

private:
    int HexToArray(const Index& index) const
    {
        int y = index.r;
        int startX = -((y + 1) / 2);
        int x = index.q - startX;

        if (x < 0 || y < 0 || m_countX <= x || m_countY <= y)
            return -1;

        return (y * m_countX) + x;
    }

    Index ArrayToHex(int arrayIndex) const
    {
        int x = arrayIndex % m_countX;
        int y = arrayIndex / m_countX;
        return ArrayToHex(x, y);
    }

    Index ArrayToHex(int x, int y) const
    {
        int startX = -((y + 1) / 2);
        return TGrid::Index(x + startX, y);
    }

private:
    Pixel m_start;
    Pixel m_end;
    T m_radius;
    std::unique_ptr<TGrid[]> m_pGrids;
    uint32 m_countX;
    uint32 m_countY;
};