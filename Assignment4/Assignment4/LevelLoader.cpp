//
//  LevelLoader.cpp
//  Assignment4
//
//  Created by Adrien Cogny on 3/18/16.
//  Copyright © 2016 Ivan Safrin. All rights reserved.
//

#include "LevelLoader.h"


void LevelLoader::loadLevelData()
{
    
    
    levelMatrix = new vector<vector<Tile*>*>; //initialize the levelMatrix
    
    ifstream fileStream(filename);
    std::cout << filename << std::endl;
    
    string line;
    while (getline(fileStream, line))
    {
        if(line == "[header]")
        {
            if(!readHeader(fileStream))
            {
                //not a valid file
                return;
            }
        }
        else if(line == "[layer]")
        {
            readLayerData(fileStream);
        }
        else if(line == "[ObjectLayer]")
        {
            readEntityData(fileStream);
        }
    }
    
    mapSize = mapWidth*mapHeight;
    
}

bool LevelLoader::readHeader(ifstream& fileStream)
{
    
    string line;
    mapWidth = -1;
    mapHeight = -1;
    
    bool foundGravityX = false;
    bool foundGravityY = false;

    while(getline(fileStream, line))
    {
        if(line == "")
        {
            break;
        }
        
        istringstream sStream(line);
        
        string key,value;
        
        getline(sStream, key, '=');
        
        getline(sStream, value);
        
        
        if(key == "width")
        {
            mapWidth = atoi(value.c_str());
        }
        else if(key == "height")
        {
            mapHeight = atoi(value.c_str());
        }
        
        if (key == "GravityX")
        {
            levelGravityX = atof(value.c_str()); //this keeps the gravity for the level if there is one in the level definition
            
            foundGravityX = true;
        }
        if (key == "GravityY") {
            levelGravityX = atof(value.c_str()); //this keeps the gravity for the level if there is one in the level definition
            foundGravityY = true;

        }
    }
    
    
    if (!foundGravityX)
    {
        //no gravity defined in level, go to default
        levelGravityX = 0.0;
    }
    if (!foundGravityY)
    {
        levelGravityY = 9.8;
    }
    
    if(mapWidth == -1 || mapHeight == -1)
    {
        return false;
    }
    else
    {
        
        //because I am using vectors as the levelMatrix, no need to allocate size
        //create and allocate the levelMatrix
        return true;
    }
    
    
}


bool LevelLoader::readLayerData(ifstream& fileStream)
{
    
    string line;
    
    while(getline(fileStream, line))
    {
        if(line == "")
        {
            break;
        }
        
        istringstream sStream(line);
        
        string key,value;
        
        getline(sStream, key, '=');
        getline(sStream, value);
        
        if(key == "data")
        {
            for(int y=0; y < mapHeight; y++)
            {
                getline(fileStream, line);
                istringstream lineStream(line);
                string tile;
                vector<Tile*>* curLine = new vector<Tile*>;
                for(int x=0; x < mapWidth; x++)
                {
                    
                    //create the vector for the line
                    int theValue;
                    
                    getline(lineStream, tile, ',');
                    unsigned char val =  (unsigned char)atoi(tile.c_str());
                    if(val > 0)
                    {
                        // be careful, the tiles in this format are indexed from 1 not 0
                        theValue = val-1;
                    }
                    else
                    {
                        theValue = 0;
                    }
                    
                    bool solidValue = false;
                    
                    if (theValue == 1)
                    {
                        solidValue = true;
                    }
                    
                    Tile* theCurTile = new Tile(theValue,x,y,solidValue);
                    
                    curLine->push_back(theCurTile);
                    
                    
                }

                levelMatrix->push_back(curLine);
            }
        }
    }
    
    return true;
    
}


bool LevelLoader::readEntityData(ifstream& fileStream)
{
    
    string line;
    string type;
    Entity* curEntityRead = new Entity;
    
    while(getline(fileStream, line))
    {
        if(line == "")
        {
            break;
        }
        
        istringstream sStream(line);
        string key,value;
        
        getline(sStream, key, '=');
        getline(sStream, value);
        
        if(key == "type")
        {
            type = value;
            curEntityRead->EntityType = type;
            if (type == "Player")
            {
                //set the spritesheet to use and the coordinates
                curEntityRead->setTexture("playersprite.png", 0, 0, 66, 92, 66, 92);
                curEntityRead->width = TILE_SIZE;
                curEntityRead->height = TILE_SIZE;
                
                
            }
        }
        else if(key == "location")
        {
            istringstream lineStream(value);
            string xPosition, yPosition;
            
            getline(lineStream, xPosition, ',');
            getline(lineStream, yPosition, ',');
            
            curEntityRead->x = atof(xPosition.c_str())*TILE_SIZE+curEntityRead->width/2;
            curEntityRead->y = -atof(yPosition.c_str())*TILE_SIZE-curEntityRead->height/2;
            
            
            
            //float placeX = atoi(xPosition.c_str())/16*TILE_SIZE;
            //float placeY = atoi(yPosition.c_str())/16*-TILE_SIZE;

        }
        
        //set the gravity for the current entity
        
        curEntityRead->gravity_x = levelGravityX;
        curEntityRead->gravity_y = levelGravityY;
        
        
    }
    
    levelEntities.push_back(curEntityRead);

    return true;
    
}

const vector<vector<Tile*>*>* LevelLoader::getLevelMatrix()
{
    return levelMatrix;
}
Tile* LevelLoader::getLevelDataAtIndex(int height, int width)
{
    if (height > mapHeight)
    {
        std::cout << "Beyond level height" << std::endl;
        return nullptr;
    }
    if (width > mapWidth)
    {
        std::cout << "Beyond level width" << std::endl;
        return nullptr;
    }
    Tile* returnValue = (*(*levelMatrix)[height])[width];
    
    return returnValue;
}
int LevelLoader::getLevelHeight()
{
    return mapHeight;
}
int LevelLoader::getLevelWidth()
{
    return mapWidth;
}
int LevelLoader::getMapSize()
{
    return mapSize;
}

int LevelLoader::getNumEntities()
{
    return levelEntities.size();
}

Entity* LevelLoader::getEntityForIndex(int index)
{
    
    return levelEntities[index];
    
}
