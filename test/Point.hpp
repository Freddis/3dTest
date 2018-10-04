//
//  Point.hpp
//  test
//
//  Created by Freddis on 16.09.17.
//  Copyright © 2017 Home-studio. All rights reserved.
//

#ifndef Point_hpp
#define Point_hpp
#include <stdio.h>
#include <cmath>
namespace hs {
class Point
{
    
public:
    float x;
    float y;
    float z;
    
    Point(float x, float y, float z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }
    
    hs::Point* copy()
    {
        hs::Point* p = new hs::Point(this->x,this->y,this->z);
        return p;
    }
    
    void rotateZ(float degree)
    {
        double rad = degree * (M_PI/180.f);
        hs::Point* p = this;
        double x = cos(rad)*p->x - sin(rad)*p->y;
        double y = sin(rad)*p->x + cos(rad)*p->y;
        double z = p->z;
        p->x = x;
        p->y = y;
        p->z = z;
    }
    void rotateX(float degree)
    {
        double rad = degree * (M_PI/180.f);
        hs::Point* p = this;
        double x = p->x;
        double y = cos(rad)*p->y - sin(rad)*p->z;
        double z = sin(rad)*p->y + cos(rad)*p->z;
        p->x = x;
        p->y = y;
        p->z = z;
        
    }
    void rotateY(float degree)
    {
        double rad = degree * (M_PI/180.f);
        hs::Point* p = this;
        double x = cos(rad)*p->x + sin(rad)*p->z;
        double y = p->y;
        double z = -1*sin(rad)*p->x + cos(rad)*p->z;
        p->x = x;
        p->y = y;
        p->z = z;
    }
    float getX()
    {
        return this->x;
    }
    float getY()
    {
        return this->y;
    }
    float getZ()
    {
        return this->z;
    }
};
}
#endif /* Point_hpp */

