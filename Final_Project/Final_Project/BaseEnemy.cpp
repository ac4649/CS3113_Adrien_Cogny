//
//  BaseEnemy.cpp
//  Final_Project
//
//  Created by Adrien Cogny on 4/26/16.
//  Copyright © 2016 Ivan Safrin. All rights reserved.
//

#include "BaseEnemy.h"

void BaseEnemy::updateDetectorPoints(float tileSize)
{
    float x = position.getx();
    float y = position.gety();
    float z = position.getz();
    
    
    
    detectorPointTopLeft.setx(x - tileSize/2 - 1/10*tileSize); //corner - 1/10* tilesize
    //detectorPointTopLeft.sety(y - tileSize/2);
    
    detectorPointTopRight.setx(x + tileSize/2 + 1/10*tileSize); //corner - 1/10* tilesize

}

void BaseEnemy::moveX(float elapsed)
{
    
    std::cout << "Moving enemy X" << std::endl;
    //change the velocity x with respect to friction, gravity and acceleration
    velocity.setx(coreFunctionObject.lerp(velocity.getx(),0.0f, elapsed * friction.getx()));
    velocity.setx(velocity.getx() + acceleration.getx()*elapsed);
    velocity.setx(velocity.getx() + gravity.getx()*elapsed);
    
    position.setx(position.getx() + velocity.getx()*elapsed);
    
    
}

void BaseEnemy::moveY(float elapsed)
{
    std::cout << "Moving enemy Y" << std::endl;
    
    //change the velocity x with respect to friction, gravity and acceleration
    velocity.sety(coreFunctionObject.lerp(velocity.gety(), 0.0f, elapsed * friction.gety()));
    velocity.sety(velocity.gety() + acceleration.gety()*elapsed);
    velocity.sety(velocity.gety() + gravity.gety()* elapsed);
    
    //change the position Y
    position.sety(position.gety() + velocity.gety()*elapsed);
    
    

}