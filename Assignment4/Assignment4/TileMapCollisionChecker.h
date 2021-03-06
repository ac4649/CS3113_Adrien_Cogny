//
//  TileMapCollisionChecker.hpp
//  Assignment4
//
//  Created by Adrien Cogny on 3/26/16.
//  Copyright © 2016 Ivan Safrin. All rights reserved.
//

#ifndef TileMapCollisionChecker_h
#define TileMapCollisionChecker_h

#include <stdio.h>
#include "LevelLoader.h"
#include "Entity.h"
#include "Tile.h"

class TileMapCollisionChecker
{
    
private:
    
    LevelLoader* curLevelLoader;
    Entity* curEntity;
    float curEntityMapTileCoordX;
    float curEntityMapTileCoordY;
    
    
    
    void getCurEntityMapTileCoords();
    
    bool collided;
    
    int errorCode;
    
    /*
     ErrorCodes:
     0 = nothing wrong;
     1 = Entity out of bounds X
     2 = Entity out of bounds Y
     
     
     */
    
    
    
public:
    
    void checkAndResolveCollisionWithEntity(Entity* theEntityChecked,LevelLoader* theLevelLoader);
    
    
    int getErrorCode();
    
    
};


#endif /* TileMapCollisionChecker_h */
