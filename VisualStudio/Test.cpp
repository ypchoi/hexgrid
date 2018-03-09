// Generated code -- http://www.redblobgames.com/grids/hexagons/
#include "stdafx.h"
#include "../Include/HexInclude.h"


typedef HexPoint_t<double> HexPoint;
typedef HexLayout_t<double> HexLayout;
typedef HexCube_t<int> HexCube;
typedef HexCube_t<double> FractionalHexCube;

const int EVEN = 1;
const int ODD = -1;
const HexOrientation layout_pointy = HexOrientation::Pointy();
const HexOrientation layout_flat = HexOrientation::Flat();

// Tests
void complain(const char* name)
{
    std::cout << "FAIL " << name << std::endl;
}


void equal_hex(const char* name, HexCube a, HexCube b)
{
    if (!(a.q == b.q && a.s == b.s && a.r == b.r))
    {
        complain(name);
    }
}

void equal_offsetcoord(const char* name, HexOffsetCoord a, HexOffsetCoord b)
{
    if (!(a.col == b.col && a.row == b.row))
    {
        complain(name);
    }
}

void equal_int(const char* name, int a, int b)
{
    if (!(a == b))
    {
        complain(name);
    }
}

void equal_hex_array(const char* name, const std::vector<HexCube>& a, const std::vector<HexCube>& b)
{
    equal_int(name, a.size(), b.size());
    for (size_t i = 0; i < a.size(); i++)
    {
        equal_hex(name, a[i], b[i]);
    }
}

void test_hex_arithmetic()
{
    equal_hex("hex_add", HexCube(4, -10, 6), HexCube(1, -3, 2) + HexCube(3, -7, 4));
    equal_hex("hex_subtract", HexCube(-2, 4, -2), HexCube(1, -3, 2) - HexCube(3, -7, 4));
}

void test_hex_direction()
{
    equal_hex("hex_direction", HexCube(0, -1, 1), HexCube(0, 0, 0).GetDirection(2));
}

void test_hex_neighbor()
{
    equal_hex("hex_neighbor", HexCube(1, -3, 2), HexCube(1, -2, 1).GetNeighbor(2));
}

void test_hex_diagonal()
{
    equal_hex("hex_diagonal", HexCube(-1, -1, 2), HexCube(1, -2, 1).GetDiagonalNeighbor(3));
}

void test_hex_distance()
{
    equal_int("hex_distance", 7, HexCube(3, -7, 4).GetDistance(HexCube(0, 0, 0)));
}

void test_hex_round()
{
    FractionalHexCube a = FractionalHexCube(0, 0, 0);
    FractionalHexCube b = FractionalHexCube(1, -1, 0);
    FractionalHexCube c = FractionalHexCube(0, -1, 1);
    equal_hex("hex_round 1", HexCube(5, -10, 5), HexCube(FractionalHexCube(0, 0, 0).Lerp(FractionalHexCube(10, -20, 10), 0.5)));
    equal_hex("hex_round 2", HexCube(a), HexCube(a.Lerp(b, 0.499)));
    equal_hex("hex_round 3", HexCube(b), HexCube(a.Lerp(b, 0.501)));
    equal_hex("hex_round 4", HexCube(a), HexCube(FractionalHexCube(a.q * 0.4 + b.q * 0.3 + c.q * 0.3, a.r * 0.4 + b.r * 0.3 + c.r * 0.3, a.s * 0.4 + b.s * 0.3 + c.s * 0.3)));
    equal_hex("hex_round 5", HexCube(c), HexCube(FractionalHexCube(a.q * 0.3 + b.q * 0.3 + c.q * 0.4, a.r * 0.3 + b.r * 0.3 + c.r * 0.4, a.s * 0.3 + b.s * 0.3 + c.s * 0.4)));
}

void test_hex_linedraw()
{
    equal_hex_array(
        "hex_linedraw",
        {HexCube(0, 0, 0), HexCube(0, -1, 1), HexCube(0, -2, 2), HexCube(1, -3, 2), HexCube(1, -4, 3), HexCube(1, -5, 4)},
        HexCube(0, 0, 0).GetLine(HexCube(1, -5, 4)));
}

void test_layout()
{
    HexCube h = HexCube(3, 4, -7);
    HexLayout flat = HexLayout(layout_flat, HexPoint(10, 15), HexPoint(0, 0));
    equal_hex("layout flat", h, FractionalHexCube(flat, h.GetCenter(flat)));
    HexLayout pointy = HexLayout(layout_pointy, HexPoint(10, 15), HexPoint(0, 0));
    equal_hex("layout pointy", h, FractionalHexCube(pointy, h.GetCenter(pointy)));
    auto flat_corners = HexCube(0, 0, 0).GetCorners(flat);
    auto pointy_corners = HexCube(0, 0, 0).GetCorners(pointy);
    flat.GetUnitWidth();
    flat.GetUnitHeight();
    pointy.GetUnitWidth();
    pointy.GetUnitHeight();

    HexGrid_t<HexLayout> manager(pointy);
    manager.Create(HexPoint(1024, 1024));

    HexCube out;
    manager.GetGrid(out, HexPoint(-1, 10));
    manager.GetGrid(out, HexPoint(10, 10));
}

void test_conversion_roundtrip()
{
    HexCube a = HexCube(3, 4, -7);
    HexOffsetCoord o(1, -3);
    HexOffsetQ q(1, -3);
    HexOffsetR r(1, -3);
    equal_hex("conversion_roundtrip even-q", a, HexCube(a.GetOffsetQ(EVEN), EVEN));
    equal_offsetcoord("conversion_roundtrip even-q", o, HexCube(q, EVEN).GetOffsetQ(EVEN));
    equal_hex("conversion_roundtrip odd-q", a, HexCube(a.GetOffsetQ(ODD), ODD));
    equal_offsetcoord("conversion_roundtrip odd-q", o, HexCube(q, ODD).GetOffsetQ(ODD));

    equal_hex("conversion_roundtrip even-r", a, HexCube(a.GetOffsetR(EVEN), EVEN));
    equal_offsetcoord("conversion_roundtrip even-r", o, HexCube(r, EVEN).GetOffsetR(EVEN));
    equal_hex("conversion_roundtrip odd-r", a, HexCube(a.GetOffsetR(ODD), ODD));
    equal_offsetcoord("conversion_roundtrip odd-r", o, HexCube(r, ODD).GetOffsetR(ODD));
}

void test_offset_from_cube()
{
    equal_offsetcoord("offset_from_cube even-q", HexOffsetCoord(1, 3), HexCube(1, 2, -3).GetOffsetQ(EVEN));
    equal_offsetcoord("offset_from_cube odd-q", HexOffsetCoord(1, 2), HexCube(1, 2, -3).GetOffsetQ(ODD));
}

void test_offset_to_cube()
{
    equal_hex("offset_to_cube even-", HexCube(1, 2, -3), HexCube(HexOffsetQ(1, 3), EVEN));
    equal_hex("offset_to_cube odd-q", HexCube(1, 2, -3), HexCube(HexOffsetQ(1, 2), ODD));
}

void test_all()
{
    test_hex_arithmetic();
    test_hex_direction();
    test_hex_neighbor();
    test_hex_diagonal();
    test_hex_distance();
    test_hex_round();
    test_hex_linedraw();
    test_layout();
    test_conversion_roundtrip();
    test_offset_from_cube();
    test_offset_to_cube();
}

int main()
{
    test_all();
    return 0;
}
