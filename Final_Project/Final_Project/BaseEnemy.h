//
//  BaseEnemy.hpp
//  Final_Project
//
//  Created by Adrien Cogny on 4/26/16.
//  Copyright © 2016 Ivan Safrin. All rights reserved.
//

#ifndef BaseEnemy_hpp
#define BaseEnemy_hpp

#include <stdio.h>
#include "Entity.h"
#include "EnemyAI.h"
//#include "CoreFunctions.h"

//this contains any basic things necessary for enemy function

class BaseEnemy: public Entity
{
    
public:
    BaseEnemy();

    void updateDetectorPoints(float tileSize);
    
    void moveX(float elapsed);
    void moveY(float elapsed);
    
    GeometricVector  getDetectorPointTopLeft();
    GeometricVector getDetectorPointTopRight();
    GeometricVector getDetectorPointBottomLeft();
    GeometricVector getDetectorPointBottomRight();
    
    GeometricVector  getTileMapDetectorPointTopLeft();
    GeometricVector getTileMapDetectorPointTopRight();
    GeometricVector getTileMapDetectorPointBottomLeft();
    GeometricVector getTileMapDetectorPointBottomRight();
    
    void drawDetectorPoints(ShaderProgram *theProgram, Matrix& projectionMatrix, Matrix& viewMatrix);

    void DrawSpriteUnorderedSheetSprite(ShaderProgram *theProgram, Matrix& projectionMatrix, Matrix& viewMatrix, ShaderProgram* theUntexturedProgram);

    
private:
    
    //these "geometricvectors" are the points to be checked for different AI behaviours
    GeometricVector detectorPointTopLeft;
    GeometricVector detectorPointTopRight;
    GeometricVector detectorPointBottomLeft;
    GeometricVector detectorPointBottomRight;
    
    GeometricVector tileMapDetectorPointTopLeft;
    GeometricVector tileMapDetectorPointTopRight;
    GeometricVector tileMapDetectorPointBottomLeft;
    GeometricVector tileMapDetectorPointBottomRight;
    
    bool checkDetectorPointTopLeft();
    bool checkDetectorPointTopRight();
    bool checkDetectorPointBottomLeft();
    bool checkDetectorPointBottomRight();
    
    Matrix detectorPointModelMatrix;
    
    GeometricVector updateTileMapVectorFromWorldVector(GeometricVector worldVector);
    GeometricVector updateWorldVectorFromTileMapVector(GeometricVector tileMapVector);
    
    
};

#endif /* BaseEnemy_hpp */
