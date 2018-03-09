#pragma once



struct HexOffsetCoord
{
    const int col;
    const int row;

    HexOffsetCoord(int col_, int row_)
        : col(col_), row(row_)
    {
    }
};



struct HexOffsetQ : public HexOffsetCoord
{
    HexOffsetQ(int col_, int row_)
        : HexOffsetCoord(col_, row_)
    {
    }
};



struct HexOffsetR : public HexOffsetCoord
{
    HexOffsetR(int col_, int row_)
        : HexOffsetCoord(col_, row_)
    {
    }
};

