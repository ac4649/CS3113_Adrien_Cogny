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
    
    float fireSpeed; //rate of fire (number of bullet per second)
    float lastFired; //last time the bullet was fired -> timestamp to compare with elapsed time
    float fireDirection_Y;
    float fireDirection_X;
    
    
    int bulletTexture;
    
    Matrix modelMatrix;
    
    
    Bullet* fire()
    {
        
        float curTicks = (float)SDL_GetTicks()/1000.0f;
        
        //std::cout << 1/fireSpeed << " vs ";
        //std::cout << curTicks-lastFired << std::endl;
        
        if (curTicks-lastFired < 1/fireSpeed)
        {
            return nullptr;
        }
        
        //std::cout << "Fired a new Bullet" << std::endl;
        
        Bullet* newBullet = new Bullet;
        newBullet->x = this->x;
        newBullet->y = this->y;
        
        newBullet->speed = 6;
        newBullet->width = 0.01;
        newBullet->height = 0.3;
        
        newBullet->direction_x = fireDirection_X;
        newBullet->direction_y = fireDirection_Y;
        
        lastFired = curTicks;
        
        return newBullet;
    }
    
    //load texture function
    GLuint LoadTexture(const char *image_path)
    {
        SDL_Surface *surface = IMG_Load(image_path);
        GLuint textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA,
                     GL_UNSIGNED_BYTE, surface->pixels);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        SDL_FreeSurface(surface);
        return textureID;
    }
    
};

#endif /* Entity_h */
