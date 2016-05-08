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
    //float z = position.getz();
    
    
    
    detectorPointTopLeft.setx(x - tileSize/2 - tileSize/10); //corner - tilesize/10
    detectorPointTopLeft.sety(y + tileSize/2 + tileSize/10);
    
    detectorPointTopRight.setx(x + tileSize/2 + tileSize/10); //corner - 1/1* tilesize
    detectorPointTopRight.sety(y + tileSize/2 + tileSize/10);

    
    detectorPointBottomLeft.setx(x - tileSize/2 - tileSize/10); //corner - 1/1* tilesize
    detectorPointBottomLeft.sety(y - tileSize/2 - tileSize/10);
    
    detectorPointBottomRight.setx(x + tileSize/2 + tileSize/10); //corner - 1/1* tilesize
    detectorPointBottomRight.sety(y - tileSize/2 - tileSize/10);
    
    
    tileMapDetectorPointTopLeft = updateTileMapVectorFromWorldVector(detectorPointTopLeft);
    
    tileMapDetectorPointTopRight = updateTileMapVectorFromWorldVector(detectorPointTopRight);
    
    tileMapDetectorPointBottomLeft = updateTileMapVectorFromWorldVector(detectorPointBottomLeft);
    
    tileMapDetectorPointBottomRight = updateTileMapVectorFromWorldVector(detectorPointBottomRight);
    

}

void BaseEnemy::moveX(float elapsed)
{
    
    //std::cout << "Moving enemy X" << std::endl;
    //change the velocity x with respect to friction, gravity and acceleration
    velocity.setx(coreFunctionObject.lerp(velocity.getx(),0.0f, elapsed * friction.getx()));
    velocity.setx(velocity.getx() + acceleration.getx()*elapsed);
    velocity.setx(velocity.getx() + gravity.getx()*elapsed);
    
    //change the position of X
    position.setx(position.getx() + velocity.getx()*elapsed);
    
    updateTileMapCoordinatesFromWorldCoords(coreFunctionObject.getTileSize());
    
    updateDetectorPoints(coreFunctionObject.getTileSize());
    
    Entity::outputEntityWorldPosition();
    Entity::outputEntityTileMapPosition();
    
}

void BaseEnemy::moveY(float elapsed)
{
    //std::cout << "Moving enemy Y" << std::endl;
    
    //change the velocity x with respect to friction, gravity and acceleration
    velocity.sety(coreFunctionObject.lerp(velocity.gety(), 0.0f, elapsed * friction.gety()));
    velocity.sety(velocity.gety() + acceleration.gety()*elapsed);
    velocity.sety(velocity.gety() + gravity.gety()* elapsed);
    
    //change the position Y
    position.sety(position.gety() + velocity.gety()*elapsed);
    
    updateTileMapCoordinatesFromWorldCoords(coreFunctionObject.getTileSize());
    
    updateDetectorPoints(coreFunctionObject.getTileSize());


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


void BaseEnemy::drawDetectorPoints(ShaderProgram *theProgram, Matrix& projectionMatrix, Matrix& viewMatrix)
{
    
    theProgram->setModelMatrix(detectorPointModelMatrix);
    theProgram->setProjectionMatrix(projectionMatrix);
    theProgram->setViewMatrix(viewMatrix);

    float vertices[] =
    {
        
        detectorPointBottomRight.getx(), detectorPointBottomRight.gety(),
        detectorPointBottomLeft.getx(), detectorPointBottomLeft.gety(),
        detectorPointTopRight.getx(), detectorPointTopRight.gety(),
        detectorPointTopLeft.getx(), detectorPointTopLeft.gety()
        
    };
    
    glVertexAttribPointer(theProgram->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(theProgram->positionAttribute);

    
    glDrawArrays(GL_POINTS, 0, 4);
    
    glDisableVertexAttribArray(theProgram->positionAttribute);
    glDisableVertexAttribArray(theProgram->texCoordAttribute);
    
    /*
    std::cout << "Detector Point Positions" << std::endl;
    
    
    std::cout << "Top Left: " << detectorPointTopLeft.getx() << " , " << detectorPointTopLeft.gety()<< std::endl;
    std::cout << "Top Right: " << detectorPointTopRight.getx() << " , " << detectorPointTopRight.gety() << std::endl;
    std::cout << "Bottom Left: " << detectorPointBottomLeft.getx() << " , " << detectorPointBottomLeft.gety() << std::endl;
    
    std::cout << "Bottom Right: " << detectorPointBottomRight.getx() << " , " << detectorPointBottomRight.gety() << std::endl;
     */


}

void BaseEnemy::DrawSpriteUnorderedSheetSprite(ShaderProgram *theProgram, Matrix& projectionMatrix, Matrix& viewMatrix,ShaderProgram* theUntexturedProgram)
{
    
   // std::cout << "DRAWING ENTITY WITH DETECTOR POINTS" << std::endl;
   
    modelMatrix.identity();

    
    if (velocity.getx() > 0)
    {
        
        //reverse the direction of the sprite

    }
    
    
    //bind the texture
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    
    theProgram->setModelMatrix(modelMatrix);
    
    //displayedEntity->modelMatrix.identity();
    //displayedEntity->modelMatrix.Translate(-totalUnitsWidth/2, -totalUnitsHeight, 0);
    
    theProgram->setProjectionMatrix(projectionMatrix);
    theProgram->setViewMatrix(viewMatrix);
    
    float u = textureLocationX / textureSheetWidth;
    float v = textureLocationY /textureSheetHeight;
    
    float spriteNormalizedWidth = textureWidth/textureSheetWidth;
    float spriteNormalizedHeight = textureHeight/textureSheetHeight;
    
    GLfloat texCoords[] = {
        u, v+spriteNormalizedHeight,
        u+spriteNormalizedWidth, v,
        u, v,
        u+spriteNormalizedWidth, v,
        u, v+spriteNormalizedHeight,
        u+spriteNormalizedWidth, v+spriteNormalizedHeight
    };
    
    float vertices[] =
    {
        
        position.getx()-width/2, position.gety()-height/2,
        position.getx()+width/2, position.gety()+height/2,
        position.getx()-width/2, position.gety()+height/2,
        
        position.getx()+width/2, position.gety()+height/2,
        position.getx()-width/2, position.gety()-height/2,
        position.getx()+width/2, position.gety()-height/2
        
    };
    
    glVertexAttribPointer(theProgram->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(theProgram->positionAttribute);
    
    glVertexAttribPointer(theProgram->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(theProgram->texCoordAttribute);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    
    glDisableVertexAttribArray(theProgram->positionAttribute);
    glDisableVertexAttribArray(theProgram->texCoordAttribute);
    /*
    std::cout << "Entity Corner Positions" << std::endl;
    
    
    std::cout << "Top Left: " << position.getx()-width/2 << " , " << position.gety()+height/2<< std::endl;
    std::cout << "Top Right: " << position.getx()+width/2 << " , " << position.gety()+height/2 << std::endl;
    std::cout << "Bottom Left: " << position.getx()-width/2 << " , " << position.gety()-height/2 << std::endl;

    std::cout << "Bottom Right: " << position.getx()+width/2 << " , " << position.gety()-height/2 << std::endl;

    */
    drawDetectorPoints(theUntexturedProgram, projectionMatrix, viewMatrix);
    
    //std::cout << "DONE DRAWING ENTITY WITH DETECTOR POINTS" << std::endl;


}

GeometricVector BaseEnemy::updateTileMapVectorFromWorldVector(GeometricVector worldVector)
{
 
    GeometricVector returnedVector;
    
    returnedVector.setx(worldVector.getx()/coreFunctionObject.getTileSize());
    returnedVector.sety(-worldVector.gety()/coreFunctionObject.getTileSize());
    returnedVector.setz(0);
    
    return returnedVector;
    
}


GeometricVector BaseEnemy::updateWorldVectorFromTileMapVector(GeometricVector tileMapVector)
{
    
    
    GeometricVector returnedVector(tileMapVector.getx()*coreFunctionObject.getTileSize()+width/2, -tileMapVector.gety()*coreFunctionObject.getTileSize()-height/2, 0);
    
    
    
    return returnedVector;
}



GeometricVector  BaseEnemy::getTileMapDetectorPointTopLeft()
{
    return tileMapDetectorPointTopLeft;
}

GeometricVector BaseEnemy::getTileMapDetectorPointTopRight()
{
    return tileMapDetectorPointTopRight;
}
GeometricVector BaseEnemy::getTileMapDetectorPointBottomLeft()
{
    return tileMapDetectorPointBottomLeft;
}
GeometricVector BaseEnemy::getTileMapDetectorPointBottomRight()
{
    return tileMapDetectorPointBottomRight;
}


