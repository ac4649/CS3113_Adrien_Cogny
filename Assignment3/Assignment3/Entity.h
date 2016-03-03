//
//  Entity.h
//  Assignment3
//
//  Created by Adrien Cogny on 3/1/16.
//  Copyright © 2016 Adrien Cogny. All rights reserved.
//

#ifndef Entity_h
#define Entity_h

#include <stdio.h>

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "Matrix.h"
#include "Bullet.h"



class Entity{
public:
    
    Entity():
    x(0.0),
    y(0.0),
    width(0.0),
    height(0.0),
    rotation(0.0),
    textureID(0),
    textureLocationX(0.0),
    textureLocationY(0.0),
    textureWidth(0.0),
    textureHeight(0.0),
    textureSheetHeight(0.0),
    textureSheetWidth(0.0),
    speed(0.0),
    direction_x(0.0),
    direction_y(0.0),
    fireSpeed(0.0),
    lastFired(0.0),
    fireDirection_Y(0.0),
    fireDirection_X(0.0),
    bulletTexture(0)
    {};
    
    void Draw();
    
    float x;
    float y;
    float width;
    float height;
    
    float rotation;
    
    int textureID;
    float textureLocationX;
    float textureLocationY;
    float textureWidth;
    float textureHeight;
    
    float textureSheetHeight;
    float textureSheetWidth;
    
    
    float speed;
    float direction_x;
    float direction_y;
    
    float fireSpeed; //rate of fire
    float lastFired; //last time the bullet was fired -> timestamp to compare with elapsed time
    float fireDirection_Y;
    float fireDirection_X;
    
    int columnNumber;
    
    int bulletTexture;
    
    Matrix modelMatrix;
    
    Bullet* fire(); // will fire a bullet -> this is only useful
    
    bool collidesWithLeftSide();
    bool collidesWithRightSide();

    bool collidesWithEntity(Entity* theEntity); // check collisions with other entities
    bool collidesWithBullet(Bullet* theBullet); // check collisions with bullets

    
    //scoring
    int points;
    
    //load texture function
    GLuint LoadTexture(const char *image_path);
    
};

#endif /* Entity_h */
