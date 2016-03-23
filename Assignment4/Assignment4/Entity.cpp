//
//  Entity.cpp
//  Assignment4
//
//  Created by Adrien Cogny on 3/23/16.
//  Copyright © 2016 Ivan Safrin. All rights reserved.
//

#include "Entity.h"
#include <iostream>





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

void Entity::setTexture(const char *image_path, float locationX, float locationY, float widthInput, float heightInput,float textureSheetWidthInput, float textureSheetHeightInput)
{
    
    textureID = LoadTexture(image_path);
    textureLocationX = locationX;
    textureLocationY = locationY;
    textureWidth = widthInput;
    textureHeight = heightInput;
    textureSheetHeight = textureSheetHeightInput;
    textureSheetWidth = textureSheetWidthInput;

    
}
