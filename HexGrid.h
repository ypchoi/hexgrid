#pragma once

// http://www.redblobgames.com/grids/hexagons/

enum class eHexGridType
{
    PointyTopped,
    FlatTopped,
};

template <class INDEX, class PIXEL, eHexGridType TYPE>
struct HexGrid
{
};

#include "HexGridPT.hpp"
