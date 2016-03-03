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
    Bullet():
    x(0.0),
    y(0.0),
    width(0.0),
    height(0.0),
    textureID(0),
    textureLocationX(0.0),
    textureLocationY(0.0),
    textureWidth(0.0),
    textureHeight(0.0),
    textureSheetHeight(0.0),
    textureSheetWidth(0.0),
    speed(0.0),
    direction_x(0.0),
    direction_y(0.0)
    {};
    
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
