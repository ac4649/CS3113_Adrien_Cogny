//
//  Entity.cpp
//  Assignment4
//
//  Created by Adrien Cogny on 3/23/16.
//  Copyright © 2016 Ivan Safrin. All rights reserved.
//

#include "Entity.h"
#include <iostream>

/*
Entity::Entity()
{

    //create all the objects
    GeometricVector position = GeometricVector();
    GeometricVector tileMapPosition = GeometricVector();
    
    GeometricVector velocity = GeometricVector();
    GeometricVector acceleration = GeometricVector();
    GeometricVector friction = GeometricVector();
    GeometricVector gravity = GeometricVector();
    
    GeometricVector topEdge = GeometricVector();
    GeometricVector bottomEdge = GeometricVector();
    
    
}
*/

bool Entity::collidesWithEntity(Entity* theEntity)
{
    
    if (position.getx() + width/2 < theEntity->position.getx() - theEntity->width/2 || position.getx() -width/2 > theEntity->position.getx() - theEntity->width/2 || position.gety() + height/2 < theEntity->position.gety() - theEntity->height/2 || position.gety()-height/2 > theEntity->position.gety() - theEntity->height/2)
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

void Entity::updateTileMapCoordinatesFromWorldCoords(float tileSize)
{
    
    /*
    tileMapX = x/tileSize;
    tileMapY = -y/tileSize;
    */
    
    tileMapPosition.setx(position.getx()/tileSize);
    tileMapPosition.sety(-position.gety()/tileSize);
    
    
    
}

void Entity::updateWorldCoordinatesFromTileMapCoords(float tileSize)
{
    /*
    x = tileMapX*tileSize+width/2;
    y = -tileMapY*tileSize-height/2;
    */
    position.setx(tileMapPosition.getx()*tileSize+width/2);
    position.sety(-tileMapPosition.gety()*tileSize-height/2);
    
    
}

void Entity::updateXWorldCoordinatesFromTileMapCoords(float tileSize)
{
    //x = tileMapX*tileSize+width/2;
    //x = tileMapX*tileSize;
    position.setx(tileMapPosition.getx()*tileSize);

}
void Entity::updateYWorldCoordinatesFromTileMapCoords(float tileSize)
{
    //y = -tileMapY*tileSize+height/2;
    //y = -tileMapY*tileSize;
    position.sety(-tileMapPosition.gety()*tileSize);


}

void Entity::outputEntityTileMapPosition()
{
 
    std::cout << "ENTITY TILE MAP POSITION: " << std::endl;
    std::cout << "x = " << tileMapPosition.getx() << std::endl;
    std::cout << "y = " << tileMapPosition.gety() << std::endl;
    std::cout << "z = " << tileMapPosition.getz() << std::endl;
    
}
void Entity::outputEntityWorldPosition()
{
    std::cout << "ENTITY WORLD POSITION: " << std::endl;
    std::cout << "x = " << position.getx() << std::endl;
    std::cout << "y = " << position.gety() << std::endl;
    std::cout << "z = " << position.getz() << std::endl;
}
