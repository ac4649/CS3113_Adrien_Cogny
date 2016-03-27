//
//  TileMapCollisionChecker.cpp
//  Assignment4
//
//  Created by Adrien Cogny on 3/26/16.
//  Copyright © 2016 Ivan Safrin. All rights reserved.
//

#include "TileMapCollisionChecker.h"

#define COLLISION_OFFSET_PUSHBACK 0.5f


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
    std::cout << "MapCoordY bottom: " << mapCoordYBottom << std::endl;
    std::cout << "MapCoordY top: " << mapCoordYTop << std::endl;
    
    
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
    
    
    //Now on y axis
    
    //top collision
    
    
    
    Tile* CenterXTopY = curLevelLoader->getLevelDataAtIndex(mapCoordYTop, mapCoordXCenter);
    
    if (CenterXTopY != nullptr)
    {
        if (CenterXTopY->getSolidValue() == true)
        {
            std::cout << "Collision with tile map (" << CenterXTopY->getTileLocationX() << "," << CenterXTopY->getTileLocationY() << ")" << std::endl;
            
            mapCoordYCenter = mapCoordYCenter-fabs(CenterXTopY->getTileLocationY() - mapCoordYTop) - COLLISION_OFFSET_PUSHBACK;
            
            
            curEntity->tileMapY = mapCoordYCenter;
            curEntity->updateWorldCoordinatesFromTileMapCoords(TILE_SIZE);
            
            collided = true;
            //curEntity->velocity_x = 0;
            curEntity->velocity_y = 0;
            //curEntity->acceleration_x = 0;
            curEntity->acceleration_y = 0;
            
            return;
        }
        
    }
    
    
    Tile*LeftXTopYchecky = curLevelLoader->getLevelDataAtIndex(mapCoordYTop, mapCoordXLeft);
    if (LeftXTopYchecky != nullptr)
    {
        if (LeftXTopYchecky->getSolidValue() == true)
        {
            std::cout << "Collision with tile map (" << LeftXTopYchecky->getTileLocationX() << "," << LeftXTopYchecky->getTileLocationY() << ")" << std::endl;
            
            mapCoordYCenter = mapCoordYCenter-fabs(LeftXTopYchecky->getTileLocationY() - mapCoordYTop) - COLLISION_OFFSET_PUSHBACK;
            
            curEntity->tileMapY = mapCoordYCenter;
            curEntity->updateWorldCoordinatesFromTileMapCoords(TILE_SIZE);
            
            collided = true;
            //curEntity->velocity_x = 0;
            curEntity->velocity_y = 0;
            //curEntity->acceleration_x = 0;
            curEntity->acceleration_y = 0;
            
            return;
        }
    }
    
    Tile* RightXTopYchecky = curLevelLoader->getLevelDataAtIndex(mapCoordYTop, mapCoordXRight);
    
    if (RightXTopYchecky != nullptr)
    {
        if (RightXTopYchecky->getSolidValue() == true)
        {
            std::cout << "Collision with tile map (" << RightXTopYchecky->getTileLocationX() << "," << RightXTopYchecky->getTileLocationY() << ")" << std::endl;
            
            mapCoordYCenter = mapCoordYCenter-fabs(RightXTopYchecky->getTileLocationY() - mapCoordYTop) - COLLISION_OFFSET_PUSHBACK;
            
            curEntity->tileMapY = mapCoordYCenter;
            
            curEntity->updateWorldCoordinatesFromTileMapCoords(TILE_SIZE);
            
            collided = true;
            //curEntity->velocity_x = 0;
            curEntity->velocity_y = 0;
            //curEntity->acceleration_x = 0;
            curEntity->acceleration_y = 0;
            
            return;
            
        }
    }
    
    
    //now do bottom side
    
    Tile* LeftXBottomYchecky = curLevelLoader->getLevelDataAtIndex(mapCoordYBottom, mapCoordXLeft);
    
    if (LeftXBottomYchecky != nullptr)
    {
        if (LeftXBottomYchecky->getSolidValue() == true)
        {
            std::cout << "Collision with tile map (" << LeftXBottomYchecky->getTileLocationX() << "," << LeftXBottomYchecky->getTileLocationY() << ")" << std::endl;
            
            std::cout << "MapCoordYBottom = " << mapCoordYBottom << " tilelocationY = " << LeftXBottomYchecky->getTileLocationY() << " Delta = " << mapCoordYBottom - LeftXBottomYchecky->getTileLocationY() << std::endl;
            
            std::cout << "New position delta = " << fabs(mapCoordYBottom - LeftXBottomYchecky->getTileLocationY()) - COLLISION_OFFSET_PUSHBACK << std::endl;
            
            mapCoordYCenter = mapCoordYCenter-fabs(mapCoordYBottom - LeftXBottomYchecky->getTileLocationY()) - COLLISION_OFFSET_PUSHBACK;
            
            std::cout << "New position " << mapCoordYCenter << std::endl;
            
            
            std::cout << mapCoordYCenter << std::endl;
            curEntity->tileMapY = mapCoordYCenter;
            
            curEntity->updateWorldCoordinatesFromTileMapCoords(TILE_SIZE);
            
            collided = true;
            //curEntity->velocity_x = 0;
            curEntity->velocity_y = 0;
            //curEntity->acceleration_x = 0;
            curEntity->acceleration_y = 0;
            
            return;
        }
        
    }
     
    Tile* CenterXBottomY = curLevelLoader->getLevelDataAtIndex(mapCoordYBottom, mapCoordXCenter);
     
    if (CenterXBottomY != nullptr)
     {
     if (CenterXBottomY->getSolidValue() == true)
     {
     std::cout << "Collision with tile map (" << CenterXBottomY->getTileLocationX() << "," << CenterXBottomY->getTileLocationY() << ")" << std::endl;
     
     mapCoordYCenter = mapCoordYCenter+fabs(mapCoordXLeft - CenterXBottomY->getTileLocationX()) - COLLISION_OFFSET_PUSHBACK;
     
     curEntity->tileMapY = mapCoordYCenter;
     
     curEntity->updateWorldCoordinatesFromTileMapCoords(TILE_SIZE);
     
     collided = true;
     //curEntity->velocity_x = 0;
     curEntity->velocity_y = 0;
     //curEntity->acceleration_x = 0;
     curEntity->acceleration_y = 0;
     
     return;
     }
    }
    
    /*
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
     
     */
    
    
    //x collisions -> check the tiles that the entity is touching
    
    //first point will be (mapCoordXRight, MapCoordY Center)
    
    //get tile at point (mapCoordXRight, MapCoordY Center)
    
    Tile* RightXCenterY = curLevelLoader->getLevelDataAtIndex(mapCoordYCenter, mapCoordXRight);
    
    if (RightXCenterY != nullptr)
    {
        if (RightXCenterY->getSolidValue() == true)
        {
            std::cout << "Collision with tile map (" << RightXCenterY->getTileLocationX() << "," << RightXCenterY->getTileLocationY() << ")" << std::endl;
            
            mapCoordXCenter = mapCoordXCenter-fabs(RightXCenterY->getTileLocationX() - mapCoordXRight) - COLLISION_OFFSET_PUSHBACK;
          
            
            curEntity->tileMapX = mapCoordXCenter;
            curEntity->updateWorldCoordinatesFromTileMapCoords(TILE_SIZE);
            
            collided = true;
            curEntity->velocity_x = 0;
            //curEntity->velocity_y = 0;
            curEntity->acceleration_x = 0;
            //curEntity->acceleration_y = 0;
            
            return;
        }

    }
    
    
    Tile* RightXTopY = curLevelLoader->getLevelDataAtIndex(mapCoordYTop, mapCoordXRight);
    
    if (RightXTopY != nullptr)
    {
        if (RightXTopY->getSolidValue() == true)
        {
            std::cout << "Collision with tile map (" << RightXTopY->getTileLocationX() << "," << RightXTopY->getTileLocationY() << ")" << std::endl;
            
            mapCoordXCenter = mapCoordXCenter-fabs(RightXTopY->getTileLocationX() - mapCoordXRight) - COLLISION_OFFSET_PUSHBACK;

            curEntity->tileMapX = mapCoordXCenter;
            curEntity->updateWorldCoordinatesFromTileMapCoords(TILE_SIZE);
            
            collided = true;
            curEntity->velocity_x = 0;
            //curEntity->velocity_y = 0;
            curEntity->acceleration_x = 0;
            //curEntity->acceleration_y = 0;
            
            return;
        }
    }
    
    Tile* RightXBottomY = curLevelLoader->getLevelDataAtIndex(mapCoordYBottom, mapCoordXRight);
    
    if (RightXBottomY != nullptr)
    {
        if (RightXBottomY->getSolidValue() == true)
        {
            std::cout << "Collision with tile map (" << RightXBottomY->getTileLocationX() << "," << RightXBottomY->getTileLocationY() << ")" << std::endl;
            
            mapCoordXCenter = mapCoordXCenter-fabs(RightXBottomY->getTileLocationX() - mapCoordXRight) - COLLISION_OFFSET_PUSHBACK;

            curEntity->tileMapX = mapCoordXCenter;
            
            curEntity->updateWorldCoordinatesFromTileMapCoords(TILE_SIZE);
            
            collided = true;
            curEntity->velocity_x = 0;
            //curEntity->velocity_y = 0;
            curEntity->acceleration_x = 0;
            //curEntity->acceleration_y = 0;
            
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
            
            mapCoordXCenter = mapCoordXCenter+fabs(mapCoordXLeft - LeftXCenterY->getTileLocationX()) + COLLISION_OFFSET_PUSHBACK;
            
            
            std::cout << mapCoordXCenter << std::endl;
            curEntity->tileMapX = mapCoordXCenter;
            
            curEntity->updateWorldCoordinatesFromTileMapCoords(TILE_SIZE);
            
            collided = true;
            curEntity->velocity_x = 0;
            //curEntity->velocity_y = 0;
            curEntity->acceleration_x = 0;
            //curEntity->acceleration_y = 0;
            
            return;
        }
        
    }
    
    
    Tile* LeftXTopY = curLevelLoader->getLevelDataAtIndex(mapCoordYTop, mapCoordXLeft);
    
    if (LeftXTopY != nullptr)
    {
        if (LeftXTopY->getSolidValue() == true)
        {
            std::cout << "Collision with tile map (" << LeftXTopY->getTileLocationX() << "," << LeftXTopY->getTileLocationY() << ")" << std::endl;
            
            mapCoordXCenter = mapCoordXCenter+fabs(mapCoordXLeft - LeftXTopY->getTileLocationX()) + COLLISION_OFFSET_PUSHBACK;
            
            curEntity->tileMapX = mapCoordXCenter;
            
            curEntity->updateWorldCoordinatesFromTileMapCoords(TILE_SIZE);
            
            collided = true;
            curEntity->velocity_x = 0;
            //curEntity->velocity_y = 0;
            curEntity->acceleration_x = 0;
            //curEntity->acceleration_y = 0;
            
            return;
        }
    }
    
    Tile* LeftXBottomY = curLevelLoader->getLevelDataAtIndex(mapCoordYBottom, mapCoordXLeft);
    
    if (LeftXBottomY != nullptr)
    {
        if (LeftXBottomY->getSolidValue() == true)
        {
            std::cout << "Collision with tile map (" << LeftXBottomY->getTileLocationX() << "," << LeftXBottomY->getTileLocationY() << ")" << std::endl;
            
            mapCoordXCenter = mapCoordXCenter+fabs(mapCoordXLeft - LeftXBottomY->getTileLocationX()) + COLLISION_OFFSET_PUSHBACK;
            
            curEntity->tileMapX = mapCoordXCenter;
            
            curEntity->updateWorldCoordinatesFromTileMapCoords(TILE_SIZE);
            
            collided = true;
            curEntity->velocity_x = 0;
            //curEntity->velocity_y = 0;
            curEntity->acceleration_x = 0;
            //curEntity->acceleration_y = 0;
            
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

