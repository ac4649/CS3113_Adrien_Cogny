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
    { // allocate our map data
        //levelMatrix = new vector<vector<int>*>;
        
        for(int i = 0; i < mapHeight; ++i)
        {
            levelMatrix[i] = new vector<int>[mapWidth];
        }
        return true;
    }
    
    
}


bool readLayerData(ifstream& fileStream)
{
    
    return true;
    
}


bool readEntityData(ifstream& fileStream)
{
    
    return true;
    
}