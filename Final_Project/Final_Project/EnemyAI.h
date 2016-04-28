//
//  EnnemyAI.hpp
//  Final_Project
//
//  Created by Adrien Cogny on 4/26/16.
//  Copyright © 2016 Ivan Safrin. All rights reserved.
//

#ifndef EnemyAI_h
#define EnemyAI_h

#include <stdio.h>
#include "CoreFunctions.h"
#include "GeometricVector.h"
#include "LevelLoader.h"
#include "TileMapCollisionChecker.h"

class LevelLoader;
class TileMapCollisionChecker;

class EnemyAI
{
public:
    
    EnemyAI(){};
    
    bool detectPointSolid(GeometricVector* thePointToCheck); //simple solid point detection
    
private:
    
    
    
    
    
    
};


#endif /* EnnemyAI_hpp */
