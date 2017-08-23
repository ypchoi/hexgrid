#pragma once

#ifndef __inout
#define __inout
#endif

#ifndef __in
#define __in
#endif

#ifndef __out
#define __out
#endif

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
