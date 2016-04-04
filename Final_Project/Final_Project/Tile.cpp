//
//  Tile.cpp
//  Assignment4
//
//  Created by Adrien Cogny on 3/26/16.
//  Copyright © 2016 Ivan Safrin. All rights reserved.
//

#include "Tile.h"

int Tile::getTileValue()
{
    return tileValue;
}

int Tile::getTileLocationX()
{
    return tileLocationX;
}

int Tile::getTileLocationY()
{
    return tileLocationY;
}

bool Tile::getSolidValue()
{
    return isSolid;
}