#pragma once

enum class eHexGridShape
{
    PointyTopped,
    FlatTopped,
};

template <class T, eHexGridShape SHAPE>
struct HexGrid_t;

template <class TGrid>
class HexGridManager;

template <class T>
struct HexIndex_t;
