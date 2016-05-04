//
//  CoreFunctions.hpp
//  Final_Project
//
//  Created by Adrien Cogny on 4/27/16.
//  Copyright © 2016 Ivan Safrin. All rights reserved.
//

#ifndef CoreFunctions_h
#define CoreFunctions_h

#include <stdio.h>
#include "ShaderProgram.h"
//#include "LevelLoader.h"
//#include "TileMapCollisionChecker.h"


class CoreFunctions
{
public:
    CoreFunctions(){}
    
    //lerp function
    float lerp(float v0, float v1, float t) {
        return (1.0-t)*v0 + t*v1;
    }
    
    float getTileSize();
    
private:
    
    float tile_size = 30.0f;
    
    
    
};


#endif /* CoreFunctions_h */
