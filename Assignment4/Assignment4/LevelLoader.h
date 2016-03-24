//
//  LevelLoader.hpp
//  Assignment4
//
//  Created by Adrien Cogny on 3/18/16.
//  Copyright © 2016 Ivan Safrin. All rights reserved.
//

#ifndef LevelLoader_h
#define LevelLoader_h

#define TILE_SIZE 25.0f


#include <stdio.h>
#include <string>
#include "Matrix.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include <vector>

#include "Entity.h"


using namespace std;


class LevelLoader
{
    
private:
    string filename;
    
    int mapHeight;
    int mapWidth;
    
    int spriteCountX;
    int spriteCountY;
    
    vector<vector<int>*>* levelMatrix; //this is the tilemap
    
    vector<Entity*> levelEntities; //this is the entities
    
    bool readHeader(ifstream& fileStream);
    bool readLayerData(ifstream& fileStream);
    bool readEntityData(ifstream& fileStream);
    
    int mapSize;
    
public:
    
    LevelLoader(std::string theLevelFileName):filename(theLevelFileName){};
    
    void loadLevelData();
    
    
    const vector<vector<int>*>* getLevelMatrix();
    
    int getLevelDataAtIndex(int row, int col);
    
    
    void debugLevel(); //this simply printint out the level to consol for testing purposes
    
    
    
    int getLevelHeight();
    int getLevelWidth();
    int getSpriteCountX();
    int getSpriteCountY();
        
    int getMapSize();
    
    int getNumEntities();
    Entity* getEntityForIndex(int index);
    
};




#endif /* LevelLoader_h */
