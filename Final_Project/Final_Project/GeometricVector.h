//
//  GeometricVector.hpp
//  Final_Project
//
//  Created by Adrien Cogny on 4/5/16.
//  Copyright © 2016 Ivan Safrin. All rights reserved.
//

#ifndef GeometricVector_h
#define GeometricVector_h

#include <stdio.h>


class GeometricVector
{
    
public:
    
    GeometricVector();
    GeometricVector(float x, float y, float z);
    
    float length();
    GeometricVector normalized();
    
    float getx();
    float gety();
    float getz();
    
    void setx(float newx);
    void sety(float newy);
    void setz(float newz);
    
    void setxyz(float newx, float newy, float newz);
    
private:
    
    float x;
    float y;
    float z;
    
};



#endif /* GeometricVector_h */
