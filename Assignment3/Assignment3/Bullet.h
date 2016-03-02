//
//  Bullet.hpp
//  Assignment3
//
//  Created by Adrien Cogny on 3/1/16.
//  Copyright © 2016 Ivan Safrin. All rights reserved.
//

#ifndef Bullet_h
#define Bullet_h

#include <stdio.h>
#include "Matrix.h"

class Bullet
{
    
public:
    
    void Draw();
    
    float x;
    float y;
    float width;
    float height;
    
    float speed;
    float direction_x;
    float direction_y;
    
    int textureID;
    float textureLocationX;
    float textureLocationY;
    float textureWidth;
    float textureHeight;
    
    float textureSheetHeight;
    float textureSheetWidth;
    
    Matrix modelMatrix;
    
};


#endif /* Bullet_h */
