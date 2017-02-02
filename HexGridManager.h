#pragma once

template <class GRID>
class HexGridManager
{
public:
    HexGridManager()
        : m_countX(0), m_countY(0)
    {
    }

    ~HexGridManager()
    {
    }

    using uint32 = unsigned __int32;

    bool Create(float totalWidth, float totalHeight, float radius)
    {
        Release();

        uint32 countX = (uint32)std::ceil(totalWidth / GRID::GetHorizontalDistance(radius));
        uint32 countY = (uint32)std::ceil(totalHeight / GRID::GetVerticalDistance(radius));

        m_pGrids.reset(new GRID[countX * countY]);
        if (!m_pGrids)
            return false;

        m_countX = countX;
        m_countY = countY;

        for (int y = 0; y < countY; ++y)
        {
            int startX = -((y + 1) / 2);

            for (int x = 0; x < countX; ++x)
            {
                GRID::Index gridIndex = GRID::Index(x + startX, y);

                int arrayIndex = (y  * countX) + x;
                GRID& grid = m_pGrids[arrayIndex];
                new (&grid) GRID(gridIndex, radius);
            }
        }

        return true;
    }

    void Release()
    {
        m_pGrids.reset();
        m_countX = 0;
        m_countY = 0;
    }

    void ForEach(std::function<bool(const GRID&)> func)
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

private:
    std::unique_ptr<GRID[]> m_pGrids;
    uint32 m_countX;
    uint32 m_countY;
};