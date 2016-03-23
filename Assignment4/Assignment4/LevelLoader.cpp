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
    
    
    levelMatrix = new vector<vector<int>*>; //initialize the levelMatrix
    
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
        else if(line == "[ObjectsLayer]")
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
                vector<int>* curLine = new vector<int>;
                for(int x=0; x < mapWidth; x++)
                {
                    
                    //create the vector for the line
                    
                    getline(lineStream, tile, ',');
                    unsigned char val =  (unsigned char)atoi(tile.c_str());
                    if(val > 0)
                    {
                        // be careful, the tiles in this format are indexed from 1 not 0
                        curLine->push_back(val-1);
                    }
                    else
                    {
                        curLine->push_back(0);
                    }
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
    while(getline(fileStream, line))
    {
        Entity* curEntityRead = new Entity;
        
        
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
        }
        else if(key == "location")
        {
            istringstream lineStream(value);
            string xPosition, yPosition;
            
            getline(lineStream, xPosition, ',');
            getline(lineStream, yPosition, ',');
            
            curEntityRead->x = atoi(xPosition.c_str());
            curEntityRead->y = atoi(yPosition.c_str());
            
            //float placeX = atoi(xPosition.c_str())/16*TILE_SIZE;
            //float placeY = atoi(yPosition.c_str())/16*-TILE_SIZE;
            //placeEntity(type, placeX, placeY);
        }
        
        
        levelEntities.push_back(curEntityRead);
    }
    
    return true;
    
}

const vector<vector<int>*>* LevelLoader::getLevelMatrix()
{
    return levelMatrix;
}
int LevelLoader::getLevelDataAtIndex(int height, int width)
{
    if (height > mapHeight)
    {
        std::cout << "Beyond level height" << std::endl;
        return -1;
    }
    if (width > mapWidth)
    {
        std::cout << "Beyond level width" << std::endl;
        return -1;
    }
    int returnValue = (*(*levelMatrix)[height])[width];
    
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