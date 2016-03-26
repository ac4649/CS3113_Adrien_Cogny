//
//  Entity.hpp
//  Assignment4
//
//  Created by Adrien Cogny on 3/23/16.
//  Copyright © 2016 Ivan Safrin. All rights reserved.
//

#ifndef Entity_h
#define Entity_h

#include <stdio.h>
#include <iostream>

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "Matrix.h"



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
    velocity_x(0.0),
    velocity_y(0.0),
    acceleration_x(0.0),
    acceleration_y(0.0),
    friction_x(0.0),
    friction_y(0.0),
    gravity_x(0.0),
    gravity_y(0.0)
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
    
    
    float velocity_x;
    float velocity_y;
    float acceleration_x;
    float acceleration_y;
    
    float friction_x;
    float friction_y;
    float gravity_x;
    float gravity_y;
    
    int columnNumber;
    
    Matrix modelMatrix;
    
    bool collidesWithLeftSide();
    bool collidesWithRightSide();
    
    bool collidesWithEntity(Entity* theEntity); // check collisions with other entities
    
    //load texture function
    GLuint LoadTexture(const char *image_path);
    
    void setTexture(const char *image_path, float locationX, float locationY, float width, float height,float textureSheedWidth, float textureSheetHeight);
    
    std::string EntityType; // entity 0 = player, entity 1 = ennemy
    
};



#endif /* Entity_h */
