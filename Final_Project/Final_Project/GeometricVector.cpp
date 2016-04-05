//
//  GeometricVector.cpp
//  Final_Project
//
//  Created by Adrien Cogny on 4/5/16.
//  Copyright © 2016 Ivan Safrin. All rights reserved.
//

#include "GeometricVector.h"


float GeometricVector::getx()
{
    return x;
}
float GeometricVector::gety()
{
    return y;
}
float GeometricVector::getz()
{
    return z;
}

void GeometricVector::setxyz(float newx, float newy, float newz)
{
    x = newx;
    y = newy;
    z = newz;

}


void GeometricVector::setx(float newx)
{
    x = newx;
}
void GeometricVector::sety(float newy)
{
    y = newy;
}
void GeometricVector::setz(float newz)
{
    z = newz;
}