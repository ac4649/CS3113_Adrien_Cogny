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
    bool isDeathTile; //this is a tile that if collided with leads to the death of the player.
    
    
public:
    
    Tile(int tileValueInput, int tileLocationXInput, int tileLocationYInput, bool isSolidInput, bool isDeathInput):tileValue(tileValueInput),tileLocationX(tileLocationXInput),tileLocationY(tileLocationYInput),isSolid(isSolidInput),isDeathTile(isDeathInput){};
    
    
    int getTileValue();
    int getTileLocationX();
    int getTileLocationY();
    
    bool getSolidValue();
    bool getDeathValue();
    
    
};



#endif /* Tile_h */
