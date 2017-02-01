// HexGrid.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "HexInclude.h"
#include "HexPixel.h"

int _tmain(int argc, _TCHAR* argv[])
{
    HexIndexI hex(0, 0);
    HexGrid<HexIndexI, HexPixel, eHexGridType::PointyTopped> grid(hex, 10.0f);
    for (int i = 0; i < 6; ++i)
    {
        HexPixel pixel = grid.GetCorner(i);
        printf("%.1f, %.1f\n", pixel.x, pixel.y);
    }

    HexCubeIndexF c1 = HexConvertPT::ToCube<int, float>(hex);
    HexCubeIndexF c2(1.5f, 5.6f, 3.2f);
    HexCubeIndexI i1 = HexConvertPT::Round(c2);
    HexIndexI hexi = HexConvertPT::Round(HexIndexF(1.6f, 3.2f));

	return 0;
}

