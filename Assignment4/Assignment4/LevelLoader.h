//
//  LevelLoader.hpp
//  Assignment4
//
//  Created by Adrien Cogny on 3/18/16.
//  Copyright © 2016 Ivan Safrin. All rights reserved.
//

#ifndef LevelLoader_h
#define LevelLoader_h

#include <stdio.h>
#include <string>
#include "Matrix.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include <vector>


using namespace std;


class LevelLoader
{
    
private:
    string filename;
    
    int mapHeight;
    int mapWidth;
    
    vector<vector<int>*> levelMatrix;
    
    
    bool readHeader(ifstream& fileStream);
    bool readLayerData(ifstream& fileStream);
    bool readEntityData(ifstream& fileStream);
    
public:
    
    LevelLoader(std::string theLevelFileName):filename(theLevelFileName){};
    
    void loadLevelData();
    
    
    
};




#endif /* LevelLoader_h */
