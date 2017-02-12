//
//  main.cpp
//  hexgrid
//
//  Created by AKer on 2017. 2. 12..
//  Copyright © 2017년 AKer. All rights reserved.
//  

#import "PrefixHeader.pch"

int main(int argc, const char * argv[])
{
    typedef HexGrid_t<int, eHexGridShape::PointyTopped> TGrid;
    
    HexPixelI start(0, 0);
    HexPixelI end(512, 512);
    float radius = 50.0f;
    
    HexGridManager<TGrid> manager;
    manager.Create(start, end, radius);
    
    HexCubeIndexI a1(0, 0, 0);
    HexCubeIndexI b1(10, 5, 3);
    auto line1 = HexLine::Get(a1, b1);
    
    HexIndexI a2(1, 2);
    HexIndexI b2(4, 5);
    HexCubeIndexI a3 = HexConvert<eHexGridShape::PointyTopped>::ToHex<int, int>(a2);
    HexCubeIndexI b3 = HexConvertPT::ToHex<int, int>(b2);
    auto line2 = HexLine::Get(a3, b3);
    
    return 0;
}
