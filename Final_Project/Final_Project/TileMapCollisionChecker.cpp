//
//  TileMapCollisionChecker.cpp
//  Assignment4
//
//  Created by Adrien Cogny on 3/26/16.
//  Copyright © 2016 Ivan Safrin. All rights reserved.
//

#include "TileMapCollisionChecker.h"

#define COLLISION_OFFSET_PUSHBACK 0.01f


void TileMapCollisionChecker::checkAndResolveCollisionWithEntity(Entity* theEntityChecked,LevelLoader* theLevelLoader)
{
    
   
    checkAndResolveCollisionXWithEntity(theEntityChecked, theLevelLoader);
    
    checkAndResolveCollisionYWithEntity(theEntityChecked,theLevelLoader);
    
    return;

}

void TileMapCollisionChecker::checkAndResolveCollisionXWithEntity(Entity* theEntityChecked,LevelLoader* theLevelLoader)
{
    
    
    
    curEntity = theEntityChecked;
    
    curLevelLoader = theLevelLoader;
    
    getCurEntityMapTileCoords();
    
    if (errorCode != 0)
    {
        return;
    }

    
    
    float mapCoordXCenter = curEntityMapTileCoordX;
    float mapCoordXRight = curEntityMapTileCoordX + (theEntityChecked->width/TILE_SIZE)/2;
    float mapCoordXLeft = curEntityMapTileCoordX - (theEntityChecked->width/TILE_SIZE)/2;
    
    float mapCoordYCenter = curEntityMapTileCoordY;
    float mapCoordYBottom = curEntityMapTileCoordY + (theEntityChecked->height/TILE_SIZE)/2;
    float mapCoordYTop = curEntityMapTileCoordY - (theEntityChecked->height/TILE_SIZE)/2;
    
    
    collidedX = false;
    
    // x axis (6 points) (center left and center right)
    
    Tile* LeftXCenterY = curLevelLoader->getLevelDataAtIndex(mapCoordYCenter, mapCoordXLeft);
    
    if (LeftXCenterY != nullptr && collidedX != true)
    {
        if (LeftXCenterY->getSolidValue() == true)
        {
            
            mapCoordXCenter = mapCoordXCenter+fabs(LeftXCenterY->getTileLocationX() - mapCoordXLeft) + COLLISION_OFFSET_PUSHBACK;
            
            //std::cout << "LeftXCenterY " <<std::endl;
            
            //std::cout << "NEW  MapCoordXCenter: " << mapCoordXCenter << std::endl;
            curEntity->tileMapPosition.setx(mapCoordXCenter);
            
            curEntity->updateXWorldCoordinatesFromTileMapCoords(TILE_SIZE);
            
            collidedX = true;
            curEntity->velocity.setx(0);
            //curEntity->velocity.sety(0);
            curEntity->acceleration.setx(0);
            //curEntity->acceleration.sety(0);
            
        }
        
    }
    
    
    Tile* LeftXTopY = curLevelLoader->getLevelDataAtIndex(mapCoordYTop, mapCoordXLeft);
    
    if (LeftXTopY != nullptr  && collidedX != true)
    {
        if (LeftXTopY->getSolidValue() == true)
        {
            
            mapCoordXCenter = mapCoordXCenter+fabs(LeftXTopY->getTileLocationX() - mapCoordXLeft) + COLLISION_OFFSET_PUSHBACK;
            
            //std::cout << "leftXTopY " <<std::endl;
            
            //std::cout << "NEW  MapCoordXCenter: " << mapCoordXCenter << std::endl;
            curEntity->tileMapPosition.setx(mapCoordXCenter);
            
            curEntity->updateXWorldCoordinatesFromTileMapCoords(TILE_SIZE);
            
            collidedX = true;
            curEntity->velocity.setx(0);
            //curEntity->velocity.sety(0);
            curEntity->acceleration.setx(0);
            //curEntity->acceleration.sety(0);
            
        }
    }
    
    Tile* LeftXBottomYcheckX = curLevelLoader->getLevelDataAtIndex(mapCoordYBottom, mapCoordXLeft);
    
    if (LeftXBottomYcheckX != nullptr  && collidedX != true)
    {
        if (LeftXBottomYcheckX->getSolidValue() == true)
        {
            
            //std::cout << "leftXBottomYCheckX " <<std::endl;
            
            //std::cout << "OLD center: " << curEntity->tileMapPosition.getx() << std::endl;
            
            //std::cout << "FABS Difference" << mapCoordXLeft - LeftXBottomYcheckX->getTileLocationX()  << std::endl;
            
            mapCoordXCenter = mapCoordXCenter+fabs(mapCoordXLeft - LeftXBottomYcheckX->getTileLocationX()) + COLLISION_OFFSET_PUSHBACK;
            
            
            //std::cout << "NEW  MapCoordXCenter: " << mapCoordXCenter << std::endl;
            curEntity->tileMapPosition.setx(mapCoordXCenter);
            
            //std::cout << "New center: " << curEntity->tileMapPosition.getx() << std::endl;
            
            curEntity->updateXWorldCoordinatesFromTileMapCoords(TILE_SIZE);
            
            collidedX = true;
            curEntity->velocity.setx(0);
            //curEntity->velocity.sety(0);
            curEntity->acceleration.setx(0);
            //curEntity->acceleration.sety(0);
            
        }
        
    }
    
    Tile* RightXCenterY = curLevelLoader->getLevelDataAtIndex(mapCoordYCenter, mapCoordXRight);
    
    if (RightXCenterY != nullptr  && collidedX != true)
    {
        if (RightXCenterY->getSolidValue() == true)
        {
            
            mapCoordXCenter = mapCoordXCenter-fabs(RightXCenterY->getTileLocationX() - mapCoordXRight) - COLLISION_OFFSET_PUSHBACK;
            //std::cout << "RightXCenterY " <<std::endl;
            
            //std::cout << "NEW  MapCoordXCenter: " << mapCoordXCenter << std::endl;
            
            
            curEntity->tileMapPosition.setx(mapCoordXCenter);
            curEntity->updateXWorldCoordinatesFromTileMapCoords(TILE_SIZE);
            
            collidedX = true;
            curEntity->velocity.setx(0);
            //curEntity->velocity.sety(0);
            curEntity->acceleration.setx(0);
            //curEntity->acceleration.sety(0);
            
        }
        
    }
    
    
    
    Tile* RightXTopY = curLevelLoader->getLevelDataAtIndex(mapCoordYTop, mapCoordXRight);
    
    if (RightXTopY != nullptr  && collidedX != true)
    {
        if (RightXTopY->getSolidValue() == true)
        {
            
            mapCoordXCenter = mapCoordXCenter-fabs(RightXTopY->getTileLocationX() - mapCoordXRight) - COLLISION_OFFSET_PUSHBACK;
            //std::cout << "RightXTopY " <<std::endl;
            
            //std::cout << "NEW  MapCoordXCenter: " << mapCoordXCenter << std::endl;
            
            
            curEntity->tileMapPosition.setx(mapCoordXCenter);
            curEntity->updateXWorldCoordinatesFromTileMapCoords(TILE_SIZE);
            
            collidedX = true;
            curEntity->velocity.setx(0);
            //curEntity->velocity.sety(0);
            curEntity->acceleration.setx(0);
            //curEntity->acceleration.sety(0);
            
        }
        
    }
    
    Tile* RightXBottomYcheckX = curLevelLoader->getLevelDataAtIndex(mapCoordYBottom, mapCoordXRight);
    
    if (RightXBottomYcheckX != nullptr  && collidedX != true)
    {
        if (RightXBottomYcheckX->getSolidValue() == true)
        {
            
            mapCoordXCenter = mapCoordXCenter-fabs(RightXBottomYcheckX->getTileLocationX() - mapCoordXRight) - COLLISION_OFFSET_PUSHBACK;
            //std::cout << "RightXBottomCheckX " <<std::endl;
            
            //std::cout << "NEW  MapCoordXCenter: " << mapCoordXCenter << std::endl;
            
            
            curEntity->tileMapPosition.setx(mapCoordXCenter);
            curEntity->updateXWorldCoordinatesFromTileMapCoords(TILE_SIZE);
            
            collidedX = true;
            curEntity->velocity.setx(0);
            //curEntity->velocity.sety(0);
            curEntity->acceleration.setx(0);
            //curEntity->acceleration.sety(0);
            
        }
        
    }
    
    
}

void TileMapCollisionChecker::checkAndResolveCollisionYWithEntity(Entity* theEntityChecked,LevelLoader* theLevelLoader)
{

    
    curEntity = theEntityChecked;
    
    curLevelLoader = theLevelLoader;
    
    getCurEntityMapTileCoords();
    
    if (errorCode != 0)
    {
        return;
    }
    
    
    float mapCoordXCenter = curEntityMapTileCoordX;
    float mapCoordXRight = curEntityMapTileCoordX + (theEntityChecked->width/TILE_SIZE)/2;
    float mapCoordXLeft = curEntityMapTileCoordX - (theEntityChecked->width/TILE_SIZE)/2;
    
    float mapCoordYCenter = curEntityMapTileCoordY;
    float mapCoordYBottom = curEntityMapTileCoordY + (theEntityChecked->height/TILE_SIZE)/2;
    float mapCoordYTop = curEntityMapTileCoordY - (theEntityChecked->height/TILE_SIZE)/2;
    
    
    collidedX = false;
    collidedY = false;
    
    //y axis (6 points) (center top and center bottom)
    
    
    Tile* CenterXTopY = curLevelLoader->getLevelDataAtIndex(mapCoordYTop, mapCoordXCenter);
    
    if (CenterXTopY != nullptr && collidedY != true)
    {
        if (CenterXTopY->getSolidValue() == true)
        {
            mapCoordYCenter = mapCoordYCenter-fabs(CenterXTopY->getTileLocationY() - mapCoordYTop) - COLLISION_OFFSET_PUSHBACK;
            
            curEntity->tileMapPosition.sety(mapCoordYCenter);
            curEntity->updateYWorldCoordinatesFromTileMapCoords(TILE_SIZE);
            
            //std::cout << "CenterXTopY " <<std::endl;
            //std::cout << "NEW MapCoordYCenter: "  << mapCoordYCenter <<std::endl;
            
            collidedY = true;
            //curEntity->velocity.setx(0);
            curEntity->velocity.sety(0);
            //curEntity->acceleration.setx(0);
            curEntity->acceleration.sety(0);
        }
        
    }
    
    
    Tile* LeftTopY = curLevelLoader->getLevelDataAtIndex(mapCoordYTop, mapCoordXLeft);
    
    if (LeftTopY != nullptr && collidedY != true)
    {
        if (LeftTopY->getSolidValue() == true)
        {
            mapCoordYCenter = mapCoordYCenter-fabs(LeftTopY->getTileLocationY() - mapCoordYTop) - COLLISION_OFFSET_PUSHBACK;
            
            curEntity->tileMapPosition.sety(mapCoordYCenter);
            curEntity->updateYWorldCoordinatesFromTileMapCoords(TILE_SIZE);
            
            //std::cout << "LeftTopy " <<std::endl;
            
            //std::cout << "NEW MapCoordYCenter: "  << mapCoordYCenter <<std::endl;
            
            collidedY = true;
            //curEntity->velocity.setx(0);
            curEntity->velocity.sety(0);
            //curEntity->acceleration.setx(0);
            curEntity->acceleration.sety(0);
        }
        
    }
    
    Tile* RightTopY = curLevelLoader->getLevelDataAtIndex(mapCoordYTop, mapCoordXRight);
    
    if (RightTopY != nullptr && collidedY != true)
    {
        if (RightTopY->getSolidValue() == true)
        {
            mapCoordYCenter = mapCoordYCenter-fabs(RightTopY->getTileLocationY() - mapCoordYTop) - COLLISION_OFFSET_PUSHBACK;
            
            curEntity->tileMapPosition.sety(mapCoordYCenter);
            curEntity->updateYWorldCoordinatesFromTileMapCoords(TILE_SIZE);
            
            //std::cout << "RightTopY " <<std::endl;
            
            //std::cout << "NEW MapCoordYCenter: "  << mapCoordYCenter <<std::endl;
            
            collidedY = true;
            //curEntity->velocity.setx(0);
            curEntity->velocity.sety(0);
            //curEntity->acceleration.setx(0);
            curEntity->acceleration.sety(0);
        }
        
    }
    
    
    
    Tile* CenterXBottomY = curLevelLoader->getLevelDataAtIndex(mapCoordYBottom, mapCoordXCenter);
    
    if (CenterXBottomY != nullptr && collidedY != true)
    {
        if (CenterXBottomY->getSolidValue() == true)
        {
            
            mapCoordYCenter = mapCoordYCenter - fabs(CenterXBottomY->getTileLocationY() - mapCoordYBottom) - COLLISION_OFFSET_PUSHBACK;
            
            //std::cout << "CenterXBottomY " <<std::endl;
            
            //std::cout << "NEW MapCoordYCenter: "  << mapCoordYCenter << std::endl;
            
            curEntity->outputEntityTileMapPosition();
            curEntity->outputEntityWorldPosition();
            
            curEntity->tileMapPosition.sety(mapCoordYCenter);
            curEntity->updateYWorldCoordinatesFromTileMapCoords(TILE_SIZE);
            
            curEntity->outputEntityTileMapPosition();
            curEntity->outputEntityWorldPosition();
            
            collidedY = true;
            //curEntity->velocity.setx(0);
            curEntity->velocity.sety(0);
            //curEntity->acceleration.setx(0);
            curEntity->acceleration.sety(0);
            
            if (curEntity->EntityType == "Player")
            {
                curEntity->collidedBottom = true;
            }
            
        }
    }
    
    Tile* LeftXBottomY = curLevelLoader->getLevelDataAtIndex(mapCoordYBottom, mapCoordXLeft);
    
    if (LeftXBottomY != nullptr && collidedY != true)
    {
        if (LeftXBottomY->getSolidValue() == true)
        {
            
            mapCoordYCenter = mapCoordYCenter - fabs(LeftXBottomY->getTileLocationY() - mapCoordYBottom) - COLLISION_OFFSET_PUSHBACK;
            
            //std::cout << "LeftXBottomY " <<std::endl;
            
            //std::cout << "NEW MapCoordYCenter: "  << mapCoordYCenter << std::endl;
            
            curEntity->tileMapPosition.sety(mapCoordYCenter);
            
            
            curEntity->updateYWorldCoordinatesFromTileMapCoords(TILE_SIZE);
            
            collidedY = true;
            //curEntity->velocity.setx(0);
            curEntity->velocity.sety(0);
            //curEntity->acceleration.setx(0);
            curEntity->acceleration.sety(0);
            
            if (curEntity->EntityType == "Player")
            {
                curEntity->collidedBottom = true;
            }
            
        }
    }
    
    Tile* RightXBottomY = curLevelLoader->getLevelDataAtIndex(mapCoordYBottom, mapCoordXRight);
    
    if (RightXBottomY != nullptr && collidedY != true)
    {
        if (RightXBottomY->getSolidValue() == true)
        {
            
            mapCoordYCenter = mapCoordYCenter - fabs(RightXBottomY->getTileLocationY() - mapCoordYBottom) - COLLISION_OFFSET_PUSHBACK;
            //std::cout << "RightXBottomY " <<std::endl;
            
            //std::cout << "NEW MapCoordYCenter: "  << mapCoordYCenter << std::endl;
            
            curEntity->tileMapPosition.sety(mapCoordYCenter);
            
            
            curEntity->updateYWorldCoordinatesFromTileMapCoords(TILE_SIZE);
            
            collidedY = true;
            //curEntity->velocity.setx(0);
            curEntity->velocity.sety(0);
            //curEntity->acceleration.setx(0);
            curEntity->acceleration.sety(0);
            
            if (curEntity->EntityType == "Player")
            {
                curEntity->collidedBottom = true;
            }
            
        }
    }
    
}


void TileMapCollisionChecker::checkAndResolveCollisionOnEdges(Entity* theEntityChecked,LevelLoader* theLevelLoader)
{
    
        
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
    
    
    
}


int TileMapCollisionChecker::getErrorCode()
{
    return errorCode;
}


void TileMapCollisionChecker::getCurEntityMapTileCoords()
{
    //update the map coordinates from the world coordinates before
    curEntity->updateTileMapCoordinatesFromWorldCoords(TILE_SIZE);
    
    curEntityMapTileCoordX = curEntity->tileMapPosition.getx();
    curEntityMapTileCoordY = curEntity->tileMapPosition.gety();
    
}

