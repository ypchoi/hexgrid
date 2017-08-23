// HexGrid.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../Source/HexInclude.h"

int _tmain(int /*argc*/, _TCHAR* /*argv*/[])
{
    HexPixelF offset(512.0f, 256.0f);
    HexIndexI hex(0, 0);
    HexGrid_t<float, eHexGridShape::PointyTopped> grid(offset, hex, 10.0f);
    for (int i = 0; i < 6; ++i)
    {
        HexPixelF pixel = grid.GetCorner(i);
        printf("%.1f, %.1f\n", pixel.x, pixel.y);
    }

    HexCubeIndexF c1 = HexConvertPT::ToCube<int, float>(hex);
    HexCubeIndexF c2(1.5f, 5.6f, 3.2f);
    HexCubeIndexI i1 = HexConvertPT::Round<float, int>(c2);
    HexIndexI hexi = HexConvertPT::Round<float, int>(HexIndexF(1.6f, 3.2f));
    c1.GetNeighbors();
    c1.GetCost(c2);
    hexi.GetNeighbors();
    hexi.GetCost(hexi);

    HexLine::Get<int>(i1, i1);

    return 0;
}

