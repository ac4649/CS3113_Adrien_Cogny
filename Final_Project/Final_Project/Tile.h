//
//  Tile.hpp
//  Assignment4
//
//  Created by Adrien Cogny on 3/26/16.
//  Copyright © 2016 Ivan Safrin. All rights reserved.
//

#ifndef Tile_h
#define Tile_h

#include <stdio.h>

class Tile
{
    
private:
    
    int tileValue;
    int tileLocationX;
    int tileLocationY;
    
    bool isSolid;
    
    
public:
    
    Tile(int tileValueInput, int tileLocationXInput, int tileLocationYInput, bool isSolidInput):tileValue(tileValueInput),tileLocationX(tileLocationXInput),tileLocationY(tileLocationYInput),isSolid(isSolidInput){};
    
    
    int getTileValue();
    int getTileLocationX();
    int getTileLocationY();
    
    bool getSolidValue();
    
    
};



#endif /* Tile_h */
