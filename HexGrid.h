#pragma once
// http://www.redblobgames.com/grids/hexagons/

enum class eHexGridShape
{
    PointyTopped,
    FlatTopped,
};

template <class T, eHexGridShape SHAPE>
struct HexGrid_t
{
};

#include "HexGridPT.hpp"
