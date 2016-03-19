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
                vector<int> curLine;
                for(int x=0; x < mapWidth; x++)
                {
                    
                    //create the vector for the line
                    
                    getline(lineStream, tile, ',');
                    unsigned char val =  (unsigned char)atoi(tile.c_str());
                    if(val > 0)
                    {
                        // be careful, the tiles in this format are indexed from 1 not 0
                        curLine.push_back(val-1);
                    }
                    else
                    {
                        curLine.push_back(0);
                    }
                }
                levelMatrix.push_back(curLine);
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
        }
        else if(key == "location")
        {
            istringstream lineStream(value);
            string xPosition, yPosition;
            
            getline(lineStream, xPosition, ',');
            getline(lineStream, yPosition, ',');
            //float placeX = atoi(xPosition.c_str())/16*TILE_SIZE;
            //float placeY = atoi(yPosition.c_str())/16*-TILE_SIZE;
            //placeEntity(type, placeX, placeY);
        }
    }
    
    return true;
    
}