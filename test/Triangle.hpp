//
//  Triangle.hpp
//  test
//
//  Created by Freddis on 16.09.17.
//  Copyright © 2017 Home-studio. All rights reserved.
//

#ifndef Triangle_hpp
#define Triangle_hpp
#include "Point.hpp"
#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>
#include "Object3D.hpp"

class Triangle :public Object3D
{
    Point* a;
    Point* b;
    Point* c;
    GLfloat data[9] = {};
public:
    Triangle(Point *a, Point *b, Point *c)
    {
        this->a = a;
        this->b = b;
        this->c = c;
        this->color = new Color(1.0f,0.0f,0.0f);
    }
    
    
    virtual GLfloat* getVertexArray()
    {
        //std::cout << "x:" << a->x << std::endl;
        data[0] = a->x;
        data[1] = a->y;
        data[2] = a->z;
        data[3] = b->x;
        data[4] = b->y;
        data[5] = b->z;
        data[6] = c->x;
        data[7] = c->y;
        data[8] = c->z;
        return data;
    }

    int getSizeOf()
    {
        GLfloat sample;
        int size = sizeof(sample)*9;
        return size;
    }
    
    Point* getPointA()
    {
        return this->a;
    }
    
    int getNumberOfPoints()
    {
        return 3;
    }
    Point** getPoints()
    {
        Point** points = new Point*[3];
        points[0] = a;
        points[1] = b;
        points[2] = c;
        return points;
    }
    
   
};


#endif /* Triangle_hpp */
