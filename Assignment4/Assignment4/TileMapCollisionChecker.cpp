//
//  TileMapCollisionChecker.cpp
//  Assignment4
//
//  Created by Adrien Cogny on 3/26/16.
//  Copyright © 2016 Ivan Safrin. All rights reserved.
//

#include "TileMapCollisionChecker.h"


void TileMapCollisionChecker::checkAndResolveCollisionWithEntity(Entity* theEntityChecked,LevelLoader* theLevelLoader)
{
   
    errorCode = 0; //reset the error code at each collision detection
    
    curEntity = theEntityChecked;
    
    curLevelLoader = theLevelLoader;
    
    getCurEntityMapTileCoords();
    
    
    if (curEntityMapTileCoordX > theLevelLoader->getLevelWidth() || curEntityMapTileCoordX < 0)
    {
        errorCode = 1;
        return;
    }
    
    if (curEntityMapTileCoordY > theLevelLoader->getLevelHeight() || curEntityMapTileCoordY < 0)
    {
        errorCode = 2;
        return;
    }
    
    
    float mapCoordXCenter = curEntityMapTileCoordX;
    float mapCoordXRight = curEntityMapTileCoordX + (theEntityChecked->width/TILE_SIZE)/2;
    float mapCoordXLeft = curEntityMapTileCoordX - (theEntityChecked->width/TILE_SIZE)/2;

    float mapCoordYCenter = curEntityMapTileCoordY;
    float mapCoordYBottom = curEntityMapTileCoordY + (theEntityChecked->height/TILE_SIZE)/2;
    float mapCoordYTop = curEntityMapTileCoordY - (theEntityChecked->height/TILE_SIZE)/2;
    
    std::cout << "MapCoordX center: " << mapCoordXCenter << std::endl;
    std::cout << "MapCoordX right: " << mapCoordXRight << std::endl;
    std::cout << "MapCoordX left: " << mapCoordXLeft << std::endl;
    
    
    std::cout << "MapCoordY center: " << mapCoordYCenter << std::endl;
    std::cout << "MapCoordY right: " << mapCoordYBottom << std::endl;
    std::cout << "MapCoordY left: " << mapCoordYTop << std::endl;
    
    
    /*
    //make the entity stay within the map zone on the x dirrection
    
    if (mapCoordXRight >= curLevelLoader->getLevelWidth())
    {
        mapCoordXRight = mapCoordXRight-fabs(curLevelLoader->getLevelWidth() - mapCoordXRight) - 0.1f;
        mapCoordXCenter = mapCoordXCenter-fabs(curLevelLoader->getLevelWidth() - mapCoordXRight) - 0.1f;
        std::cout << "New XRight = " << mapCoordXRight << std::endl;
        
        curEntity->tileMapX = mapCoordXCenter;
        
        curEntity->updateWorldCoordinatesFromTileMapCoords(TILE_SIZE);
    }
    
    */
    
    //check the 6 points, max, min and center for collisions
    
    
    //x collisions -> check the tiles that the entity is touching
    
    //first point will be (mapCoordXRight, MapCoordY Center)
    
    //get tile at point (mapCoordXRight, MapCoordY Center)
    
    Tile* RightXCenterY = curLevelLoader->getLevelDataAtIndex(mapCoordYCenter, mapCoordXRight);
    
    if (RightXCenterY != nullptr)
    {
        if (RightXCenterY->getSolidValue() == true)
        {
            std::cout << "Collision with tile map (" << RightXCenterY->getTileLocationX() << "," << RightXCenterY->getTileLocationY() << ")" << std::endl;
            
            mapCoordXCenter = mapCoordXCenter-fabs(RightXCenterY->getTileLocationX() - mapCoordXRight) - 0.1f;
          
            
            curEntity->tileMapX = mapCoordXCenter;
            curEntity->updateWorldCoordinatesFromTileMapCoords(TILE_SIZE);
            
            collided = true;
            curEntity->velocity_x = 0;
            curEntity->velocity_y = 0;
            curEntity->acceleration_x = 0;
            curEntity->acceleration_y = 0;
            
            return;
        }

    }
    
    
    Tile* RightXTopY = curLevelLoader->getLevelDataAtIndex(mapCoordYTop, mapCoordXRight);
    
    if (RightXTopY != nullptr)
    {
        if (RightXTopY->getSolidValue() == true)
        {
            std::cout << "Collision with tile map (" << RightXTopY->getTileLocationX() << "," << RightXTopY->getTileLocationY() << ")" << std::endl;
            
            mapCoordXCenter = mapCoordXCenter-fabs(RightXTopY->getTileLocationX() - mapCoordXRight) - 0.1f;

            curEntity->tileMapX = mapCoordXCenter;
            curEntity->updateWorldCoordinatesFromTileMapCoords(TILE_SIZE);
            
            collided = true;
            curEntity->velocity_x = 0;
            curEntity->velocity_y = 0;
            curEntity->acceleration_x = 0;
            curEntity->acceleration_y = 0;
            
            return;
        }
    }
    
    Tile* RightXBottomY = curLevelLoader->getLevelDataAtIndex(mapCoordYBottom, mapCoordXRight);
    
    if (RightXBottomY != nullptr)
    {
        if (RightXBottomY->getSolidValue() == true)
        {
            std::cout << "Collision with tile map (" << RightXBottomY->getTileLocationX() << "," << RightXBottomY->getTileLocationY() << ")" << std::endl;
            
            mapCoordXCenter = mapCoordXCenter-fabs(RightXBottomY->getTileLocationX() - mapCoordXRight) - 0.1f;

            curEntity->tileMapX = mapCoordXCenter;
            
            curEntity->updateWorldCoordinatesFromTileMapCoords(TILE_SIZE);
            
            collided = true;
            curEntity->velocity_x = 0;
            curEntity->velocity_y = 0;
            curEntity->acceleration_x = 0;
            curEntity->acceleration_y = 0;
            
            return;
            
        }
    }
    
    
    //now do left side
    
    Tile* LeftXCenterY = curLevelLoader->getLevelDataAtIndex(mapCoordYCenter, mapCoordXLeft);
    
    if (LeftXCenterY != nullptr)
    {
        if (LeftXCenterY->getSolidValue() == true)
        {
            std::cout << "Collision with tile map (" << LeftXCenterY->getTileLocationX() << "," << LeftXCenterY->getTileLocationY() << ")" << std::endl;
            
            mapCoordXCenter = mapCoordXCenter+fabs(mapCoordXLeft - LeftXCenterY->getTileLocationX()) + 0.1f;
            
            
            std::cout << mapCoordXCenter << std::endl;
            curEntity->tileMapX = mapCoordXCenter;
            
            curEntity->updateWorldCoordinatesFromTileMapCoords(TILE_SIZE);
            
            collided = true;
            curEntity->velocity_x = 0;
            curEntity->velocity_y = 0;
            curEntity->acceleration_x = 0;
            curEntity->acceleration_y = 0;
            
            return;
        }
        
    }
    
    
    Tile* LeftXTopY = curLevelLoader->getLevelDataAtIndex(mapCoordYTop, mapCoordXLeft);
    
    if (LeftXTopY != nullptr)
    {
        if (LeftXTopY->getSolidValue() == true)
        {
            std::cout << "Collision with tile map (" << LeftXTopY->getTileLocationX() << "," << LeftXTopY->getTileLocationY() << ")" << std::endl;
            
            mapCoordXCenter = mapCoordXCenter+fabs(mapCoordXLeft - LeftXTopY->getTileLocationX()) + 0.1f;
            
            curEntity->tileMapX = mapCoordXCenter;
            
            curEntity->updateWorldCoordinatesFromTileMapCoords(TILE_SIZE);
            
            collided = true;
            curEntity->velocity_x = 0;
            curEntity->velocity_y = 0;
            curEntity->acceleration_x = 0;
            curEntity->acceleration_y = 0;
            
            return;
        }
    }
    
    Tile* LeftXBottomY = curLevelLoader->getLevelDataAtIndex(mapCoordYBottom, mapCoordXLeft);
    
    if (LeftXBottomY != nullptr)
    {
        if (LeftXBottomY->getSolidValue() == true)
        {
            std::cout << "Collision with tile map (" << LeftXBottomY->getTileLocationX() << "," << LeftXBottomY->getTileLocationY() << ")" << std::endl;
            
            mapCoordXCenter = mapCoordXCenter+fabs(mapCoordXLeft - LeftXBottomY->getTileLocationX()) + 0.1f;
            
            curEntity->tileMapX = mapCoordXCenter;
            
            curEntity->updateWorldCoordinatesFromTileMapCoords(TILE_SIZE);
            
            collided = true;
            curEntity->velocity_x = 0;
            curEntity->velocity_y = 0;
            curEntity->acceleration_x = 0;
            curEntity->acceleration_y = 0;
            
            return;
            
        }
    }
  
    
    
}


int TileMapCollisionChecker::getErrorCode()
{
    return errorCode;
}


void TileMapCollisionChecker::getCurEntityMapTileCoords()
{
    curEntityMapTileCoordX = curEntity->tileMapX;
    curEntityMapTileCoordY = curEntity->tileMapY;
    
}

