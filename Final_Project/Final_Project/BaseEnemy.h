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

//this contains any basic things necessary for enemy function

class BaseEnemy: public Entity
{
    
public:
    BaseEnemy();

    void updateDetectorPoints(float tileSize);
    
    void moveX(float elapsed);
    void moveY(float elapsed);
    
    
    EnemyAI* AIBehaviour;

    
private:
    
    //these "geometricvectors" are the points to be checked for different AI behaviours
    GeometricVector detectorPointTopLeft;
    GeometricVector detectorPointTopRight;
    GeometricVector detectorPointBottomLeft;
    GeometricVector detectorPointBottomRight;
    
    
    
};

#endif /* BaseEnemy_hpp */
