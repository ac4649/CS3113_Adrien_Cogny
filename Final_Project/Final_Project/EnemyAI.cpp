//
//  EnnemyAI.cpp
//  Final_Project
//
//  Created by Adrien Cogny on 4/26/16.
//  Copyright © 2016 Ivan Safrin. All rights reserved.
//

#include "EnemyAI.h"


void EnemyAI::simpleBackForth(BaseEnemy* theEnemy)
{
    
    //check the velocity direction:
    
    float enemyXVel = theEnemy->velocity.getx();
    
    if (enemyXVel < 0)
    {
        //velocity going towards the left, check the bottom Left point only
        
        GeometricVector bottomLeft = theEnemy->getDetectorPointBottomLeft();
        
        Tile* theCheckedTileLeft = theLevelLoader->getLevelDataAtIndex(bottomLeft.getx(), bottomLeft.gety());
        
        
        std::cout <<" Enemy X = " << theEnemy->position.getx() << std::endl;
        std::cout <<" Enemy Y = " << theEnemy->position.gety() << std::endl;
        
        if (theCheckedTileLeft == nullptr)
        {
            //revert the direction because edge of the map
            theEnemy->acceleration.setx(-theEnemy->acceleration.getx());
            theEnemy->velocity.setx(0);
        }
        
        if (theCheckedTileLeft->getSolidValue() == false)
        {
            //no more solid ground -> revert side
            theEnemy->acceleration.setx(-theEnemy->acceleration.getx());
            theEnemy->velocity.setx(0);
        }
        
        
        GeometricVector bottomRight = theEnemy->getDetectorPointBottomRight();
        
        Tile* theCheckedTileRight = theLevelLoader->getLevelDataAtIndex(bottomLeft.getx(), bottomLeft.gety());
        
        
        std::cout <<" Enemy X = " << theEnemy->position.getx() << std::endl;
        std::cout <<" Enemy Y = " << theEnemy->position.gety() << std::endl;
        
        if (theCheckedTileRight == nullptr)
        {
            //revert the direction because edge of the map
            theEnemy->acceleration.setx(-theEnemy->acceleration.getx());
            theEnemy->velocity.setx(0);
        }
        
        if (theCheckedTileRight->getSolidValue() == false)
        {
            //no more solid ground -> revert side
            theEnemy->acceleration.setx(-theEnemy->acceleration.getx());
            theEnemy->velocity.setx(0);
        }

    }
}
