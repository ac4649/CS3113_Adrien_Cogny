//
//  Entity.cpp
//  Assignment3
//
//  Created by Adrien Cogny on 3/1/16.
//  Copyright © 2016 Adrien Cogny. All rights reserved.
//

#include "Entity.h"
#include <iostream>


Bullet* Entity::fire()
{
    
    float curTicks = (float)SDL_GetTicks()/1000.0f;
    
    //std::cout << 1/fireSpeed << " vs ";
    //std::cout << curTicks-lastFired << std::endl;
    
    if (curTicks-lastFired < fireSpeed)
    {
        return nullptr;
    }
    
    //std::cout << "Fired a new Bullet" << std::endl;
    
    Bullet* newBullet = new Bullet;
    newBullet->x = this->x;
    newBullet->y = this->y;
    
    newBullet->speed = 6;
    newBullet->width = 0.1;
    newBullet->height = 0.3;
    
    newBullet->direction_x = fireDirection_X;
    newBullet->direction_y = fireDirection_Y;
    
    
    newBullet->textureID = textureID;
    newBullet->textureLocationX = 856;
    newBullet->textureLocationY = 421;
    newBullet->textureHeight = 54;
    newBullet->textureWidth = 9;
    
    newBullet->textureSheetHeight = 1024;
    newBullet->textureSheetWidth = 1024;
    
    lastFired = curTicks;
    
    return newBullet;
}


bool Entity::collidesWithEntity(Entity* theEntity)
{
 
    if (x + width/2 < theEntity->x - theEntity->width/2 || x -width/2 > theEntity->x - theEntity->width/2 || y + height/2 < theEntity->y - theEntity->height/2 || y-height/2 > theEntity->y - theEntity->height/2)
    {
        //No collision
    }
    else
    {
        std::cout << "Collision with entity" << std::endl;
        return true;
    }
    
    
    
    return false;
    
}

bool Entity::collidesWithBullet(Bullet* theBullet)
{
    if (x + width/2 < theBullet->x - theBullet->width/2 || x -width/2 > theBullet->x - theBullet->width/2 || y + height/2 < theBullet->y - theBullet->height/2 || y-height/2 > theBullet->y - theBullet->height/2)
    {
        //No collision
    }
    else
    {
        std::cout << "Collision" << std::endl;
        return true;
    }
    
    
    
    return false;
}



GLuint Entity::LoadTexture(const char *image_path)
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