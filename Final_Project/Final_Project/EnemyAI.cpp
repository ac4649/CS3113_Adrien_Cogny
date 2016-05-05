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
    
    //std::cout << " Enemy VelocityX = " << enemyXVel << std::endl;
    
    if (enemyXVel < 0)
    {
        //velocity going towards the left, check the bottom Left point only
        
        GeometricVector bottomLeft = theEnemy->getTileMapDetectorPointBottomLeft();
        
        Tile* theCheckedTileLeft = theLevelLoader->getLevelDataAtIndex(bottomLeft.gety(), bottomLeft.getx());
        

        if (theCheckedTileLeft == nullptr)
        {
            //revert the direction because edge of the map
            std::cout << "No left tile" << std::endl;
            theEnemy->acceleration.setx(-theEnemy->acceleration.getx());
            theEnemy->velocity.setx(0);
        }
        
        else if (theCheckedTileLeft->getSolidValue() == false)
        {
            //no more solid ground -> revert side
            std::cout << "Left tile not solid" << std::endl;

            theEnemy->acceleration.setx(-theEnemy->acceleration.getx());
            theEnemy->velocity.setx(0);
        }
        

    }
    
    else if (enemyXVel > 0)
    {
        
        GeometricVector bottomRight = theEnemy->getTileMapDetectorPointBottomRight();
        
        
        
        Tile* theCheckedTileRight = theLevelLoader->getLevelDataAtIndex(bottomRight.gety(), bottomRight.getx());
        
        
        if (theCheckedTileRight == nullptr)
        {
            //revert the direction because edge of the map
            std::cout << "No right tile" << std::endl;
            
            theEnemy->acceleration.setx(-theEnemy->acceleration.getx());
            theEnemy->velocity.setx(0);
        }
        
        else if (theCheckedTileRight->getSolidValue() == false)
        {
            //no more solid ground -> revert side
            std::cout << "Right tile not solid" << std::endl;
            
            theEnemy->acceleration.setx(-theEnemy->acceleration.getx());
            theEnemy->velocity.setx(0);
        }
        
    }
}
