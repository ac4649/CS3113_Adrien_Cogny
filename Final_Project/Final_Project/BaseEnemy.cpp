//
//  BaseEnemy.cpp
//  Final_Project
//
//  Created by Adrien Cogny on 4/26/16.
//  Copyright © 2016 Ivan Safrin. All rights reserved.
//

#include "BaseEnemy.h"
#include "Tile.h"

BaseEnemy::BaseEnemy():Entity(){}

void BaseEnemy::updateDetectorPoints(float tileSize)
{
    float x = position.getx();
    float y = position.gety();
    float z = position.getz();
    
    
    
    detectorPointTopLeft.setx(x - tileSize/2 - 1/10*tileSize); //corner - 1/10* tilesize
    detectorPointTopLeft.sety(y - tileSize/2 - 1/10*tileSize);
    
    detectorPointTopRight.setx(x + tileSize/2 + 1/10*tileSize); //corner - 1/10* tilesize
    detectorPointTopRight.sety(y - tileSize/2 + 1/10*tileSize);

    
    detectorPointBottomLeft.setx(x + tileSize/2 + 1/10*tileSize); //corner - 1/10* tilesize
    detectorPointBottomLeft.sety(y - tileSize/2 - 1/10*tileSize);
    
    detectorPointBottomRight.setx(x + tileSize/2 + 1/10*tileSize); //corner - 1/10* tilesize
    detectorPointBottomRight.sety(y - tileSize/2 - 1/10*tileSize);
    

}

void BaseEnemy::moveX(float elapsed)
{
    
    std::cout << "Moving enemy X" << std::endl;
    //change the velocity x with respect to friction, gravity and acceleration
    velocity.setx(coreFunctionObject.lerp(velocity.getx(),0.0f, elapsed * friction.getx()));
    velocity.setx(velocity.getx() + acceleration.getx()*elapsed);
    velocity.setx(velocity.getx() + gravity.getx()*elapsed);
    
    //change the position of X
    position.setx(position.getx() + velocity.getx()*elapsed);
    
    updateTileMapCoordinatesFromWorldCoords(coreFunctionObject.getTileSize());
    
}

void BaseEnemy::moveY(float elapsed)
{
    std::cout << "Moving enemy Y" << std::endl;
    
    //change the velocity x with respect to friction, gravity and acceleration
    velocity.sety(coreFunctionObject.lerp(velocity.gety(), 0.0f, elapsed * friction.gety()));
    velocity.sety(velocity.gety() + acceleration.gety()*elapsed);
    velocity.sety(velocity.gety() + gravity.gety()* elapsed);
    
    //change the position Y
    position.sety(position.gety() + velocity.gety()*elapsed);
    
    updateTileMapCoordinatesFromWorldCoords(coreFunctionObject.getTileSize());
    

}




bool BaseEnemy::checkDetectorPointTopLeft()
{
    
    updateTileMapCoordinatesFromWorldCoords(coreFunctionObject.getTileSize());
    
    return false;

    
}

bool BaseEnemy::checkDetectorPointTopRight()
{
    
    updateTileMapCoordinatesFromWorldCoords(coreFunctionObject.getTileSize());

    return false;
}

bool BaseEnemy::checkDetectorPointBottomLeft()
{
    
    updateTileMapCoordinatesFromWorldCoords(coreFunctionObject.getTileSize());

    return false;
    
}

bool BaseEnemy::checkDetectorPointBottomRight()
{
    
    updateTileMapCoordinatesFromWorldCoords(coreFunctionObject.getTileSize());

    return false;
    
}

GeometricVector  BaseEnemy::getDetectorPointTopLeft()
{
    return detectorPointTopLeft;
}

GeometricVector BaseEnemy::getDetectorPointTopRight()
{
    return detectorPointTopRight;
}
GeometricVector BaseEnemy::getDetectorPointBottomLeft()
{
    return detectorPointBottomLeft;
}
GeometricVector BaseEnemy::getDetectorPointBottomRight()
{
    return detectorPointBottomRight;
}



