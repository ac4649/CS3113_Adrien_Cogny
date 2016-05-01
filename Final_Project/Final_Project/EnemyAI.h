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
#include "BaseEnemy.h"
#include "Tile.h"

class LevelLoader;
class TileMapCollisionChecker;
class BaseEnemy;

class EnemyAI
{
public:
    
    EnemyAI(TileMapCollisionChecker* passedCollisionChecker, LevelLoader* passedLevelLoader):theCollisionChecker(passedCollisionChecker), theLevelLoader(passedLevelLoader){};
    
    TileMapCollisionChecker* theCollisionChecker;
    LevelLoader* theLevelLoader;
    
    void simpleBackForth(BaseEnemy* theEnemy); //simple solid point detection that reverses the location of the enemys
    
private:
    
    
    
};


#endif /* EnnemyAI_hpp */
