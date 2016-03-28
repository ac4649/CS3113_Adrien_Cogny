//
//  TileMapCollisionChecker.cpp
//  Assignment4
//
//  Created by Adrien Cogny on 3/26/16.
//  Copyright © 2016 Ivan Safrin. All rights reserved.
//

#include "TileMapCollisionChecker.h"

#define COLLISION_OFFSET_PUSHBACK 0.00000001f


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
    
    
    //4 point collision system first
    
    
    //y axis (2 points) (center top and center bottom)
    
    
    Tile* CenterXTopY = curLevelLoader->getLevelDataAtIndex(mapCoordYTop, mapCoordXCenter);
    
    if (CenterXTopY != nullptr)
    {
        if (CenterXTopY->getSolidValue() == true)
        {
            
            
            //std::cout << "MapCoordX center: " << mapCoordXCenter << std::endl;
            //std::cout << "MapCoordX right: " << mapCoordXRight << std::endl;
            //std::cout << "MapCoordX left: " << mapCoordXLeft << std::endl;
            
            
            //std::cout << "MapCoordY center: " << mapCoordYCenter << std::endl;
            //std::cout << "MapCoordY bottom: " << mapCoordYBottom << std::endl;
            //std::cout << "MapCoordY top: " << mapCoordYTop << std::endl;
            
            
            
            //std::cout << "TOP COLLISION" << std::endl;
            //std::cout << "Collision with tile map (" << CenterXTopY->getTileLocationX() << "," << CenterXTopY->getTileLocationY() << ")" << std::endl;
            
            mapCoordYCenter = mapCoordYCenter-fabs(CenterXTopY->getTileLocationY() - mapCoordYTop) - COLLISION_OFFSET_PUSHBACK;
            
            
            curEntity->tileMapY = mapCoordYCenter;
            curEntity->updateYWorldCoordinatesFromTileMapCoords(TILE_SIZE);
            
            //std::cout << "NEW MapCoordYCenter: "  << mapCoordYCenter <<std::endl;
            
            collided = true;
            //curEntity->velocity_x = 0;
            curEntity->velocity_y = 0;
            //curEntity->acceleration_x = 0;
            curEntity->acceleration_y = 0;
        }
        
    }
    
    
    Tile* CenterXBottomY = curLevelLoader->getLevelDataAtIndex(mapCoordYBottom, mapCoordXCenter);
    
    if (CenterXBottomY != nullptr)
    {
        if (CenterXBottomY->getSolidValue() == true)
        {
         
            //std::cout << "MapCoordX center: " << mapCoordXCenter << std::endl;
            //std::cout << "MapCoordX right: " << mapCoordXRight << std::endl;
            //std::cout << "MapCoordX left: " << mapCoordXLeft << std::endl;
            
            
            //std::cout << "MapCoordY center: " << mapCoordYCenter << std::endl;
            //std::cout << "MapCoordY bottom: " << mapCoordYBottom << std::endl;
            //std::cout << "MapCoordY top: " << mapCoordYTop << std::endl;
            
            
            
            //std::cout << "BOTTOM COLLISION" << std::endl;

            //std::cout << "Collision with tile map (" << CenterXBottomY->getTileLocationX() << "," << CenterXBottomY->getTileLocationY() << ")" << std::endl;
            
            //std::cout << "OLD CENTER: " << mapCoordYCenter << std::endl;
            
            //std::cout << "TileTopCoord: " << CenterXBottomY->getTileLocationY() << std::endl;
            //std::cout << "TileBottomCoord: " << CenterXBottomY->getTileLocationY() + 1.0f << std::endl;
            
            //std::cout << "Displacement: " << fabs(mapCoordYBottom - CenterXBottomY->getTileLocationY()) << std::endl;
            
            //std::cout << "Fudge Factor: " << COLLISION_OFFSET_PUSHBACK << std::endl;
            
            //std::cout << "Total Displacement: " << fabs(CenterXBottomY->getTileLocationY() - mapCoordYBottom) + COLLISION_OFFSET_PUSHBACK << std::endl;
            
            mapCoordYCenter = mapCoordYCenter - fabs(CenterXBottomY->getTileLocationY() - mapCoordYBottom) - COLLISION_OFFSET_PUSHBACK;
            
            //std::cout << "NEW MapCoordYCenter: "  << mapCoordYCenter << std::endl;
            
            curEntity->tileMapY = mapCoordYCenter;
            
            
            curEntity->updateYWorldCoordinatesFromTileMapCoords(TILE_SIZE);
            
            collided = true;
            //curEntity->velocity_x = 0;
            curEntity->velocity_y = 0;
            //curEntity->acceleration_x = 0;
            curEntity->acceleration_y = 0;
            
            if (curEntity->EntityType == "Player")
            {
                curEntity->collidedBottom = true;
            }
            
        }
    }
    
    
    // x axis (2 points) (center left and center right)
    
    
    Tile* LeftXCenterY = curLevelLoader->getLevelDataAtIndex(mapCoordYCenter, mapCoordXLeft);
    
    if (LeftXCenterY != nullptr)
    {
        if (LeftXCenterY->getSolidValue() == true)
        {
            
            //std::cout << "MapCoordX center: " << mapCoordXCenter << std::endl;
            //std::cout << "MapCoordX right: " << mapCoordXRight << std::endl;
            //std::cout << "MapCoordX left: " << mapCoordXLeft << std::endl;
            
            
            //std::cout << "MapCoordY center: " << mapCoordYCenter << std::endl;
            //std::cout << "MapCoordY bottom: " << mapCoordYBottom << std::endl;
            //std::cout << "MapCoordY top: " << mapCoordYTop << std::endl;
            
            
            
            
            
            //std::cout << "LEFT COLLISION" << std::endl;
            //std::cout << "Collision with tile map (" << LeftXCenterY->getTileLocationX() << "," << LeftXCenterY->getTileLocationY() << ")" << std::endl;
            
            //std::cout << "OLD CENTER: " << mapCoordXCenter << std::endl;
            //std::cout << "TileLeftCoord: " << LeftXCenterY->getTileLocationX() << std::endl;
            //std::cout << "TileRightCoord: " << LeftXCenterY->getTileLocationX() + 1.0f << std::endl;

            //std::cout << "Displacement: " << fabs(LeftXCenterY->getTileLocationX() + 1.0f - mapCoordXLeft) << std::endl;
            //std::cout << "Fudge Factor: " << COLLISION_OFFSET_PUSHBACK << std::endl;
            //std::cout << "Total Displacement: " << fabs(LeftXCenterY->getTileLocationX() + 1.0f - mapCoordXLeft) + COLLISION_OFFSET_PUSHBACK << std::endl;
            
            mapCoordXCenter = mapCoordXCenter+fabs(LeftXCenterY->getTileLocationX() + 1.0f - mapCoordXLeft) + COLLISION_OFFSET_PUSHBACK;
            
            
            //std::cout << "NEW CENTER: " << mapCoordXCenter << std::endl;
            curEntity->tileMapX = mapCoordXCenter;
            
            curEntity->updateXWorldCoordinatesFromTileMapCoords(TILE_SIZE);
            
            collided = true;
            curEntity->velocity_x = 0;
            //curEntity->velocity_y = 0;
            curEntity->acceleration_x = 0;
            //curEntity->acceleration_y = 0;
            
        }
        
    }
    
    Tile* RightXCenterY = curLevelLoader->getLevelDataAtIndex(mapCoordYCenter, mapCoordXRight);
    
    if (RightXCenterY != nullptr)
    {
        if (RightXCenterY->getSolidValue() == true)
        {
            
            
            //std::cout << "MapCoordX center: " << mapCoordXCenter << std::endl;
            //std::cout << "MapCoordX right: " << mapCoordXRight << std::endl;
            //std::cout << "MapCoordX left: " << mapCoordXLeft << std::endl;
            
            
            //std::cout << "MapCoordY center: " << mapCoordYCenter << std::endl;
            //std::cout << "MapCoordY bottom: " << mapCoordYBottom << std::endl;
            //std::cout << "MapCoordY top: " << mapCoordYTop << std::endl;
        
            
            //std::cout << "RIGHT COLLISION" << std::endl;

            //std::cout << "Collision with tile map (" << RightXCenterY->getTileLocationX() << "," << RightXCenterY->getTileLocationY() << ")" << std::endl;
            
            mapCoordXCenter = mapCoordXCenter-fabs(RightXCenterY->getTileLocationX() - mapCoordXRight) - COLLISION_OFFSET_PUSHBACK;
            
            
            curEntity->tileMapX = mapCoordXCenter;
            curEntity->updateXWorldCoordinatesFromTileMapCoords(TILE_SIZE);
            
            collided = true;
            curEntity->velocity_x = 0;
            //curEntity->velocity_y = 0;
            curEntity->acceleration_x = 0;
            //curEntity->acceleration_y = 0;
            
        }
        
    }
     
    
    
    
    //make the entity stay within the map zone on the x dirrection
    
    if (mapCoordXRight >= curLevelLoader->getLevelWidth())
    {
        mapCoordXRight = mapCoordXRight-fabs(curLevelLoader->getLevelWidth() - mapCoordXRight) - 0.1f;
        mapCoordXCenter = mapCoordXCenter-fabs(curLevelLoader->getLevelWidth() - mapCoordXRight) - 0.1f;
        //std::cout << "New XRight = " << mapCoordXRight << std::endl;
        
        curEntity->tileMapX = mapCoordXCenter;
        
        curEntity->updateXWorldCoordinatesFromTileMapCoords(TILE_SIZE);
    }
    else if (mapCoordXLeft < 0)
    {
        
        mapCoordXCenter = mapCoordXCenter+fabs(0 - mapCoordXRight) + 0.1f;
    }
    
    return;

}


int TileMapCollisionChecker::getErrorCode()
{
    return errorCode;
}


void TileMapCollisionChecker::getCurEntityMapTileCoords()
{
    //update the map coordinates from the world coordinates before
    curEntity->updateTileMapCoordinatesFromWorldCoords(TILE_SIZE);
    
    curEntityMapTileCoordX = curEntity->tileMapX;
    curEntityMapTileCoordY = curEntity->tileMapY;
    
}

