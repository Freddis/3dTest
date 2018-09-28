//
//  Square.hpp
//  test
//
//  Created by Freddis on 23.09.2018.
//  Copyright © 2018 Home-studio. All rights reserved.
//

#ifndef Square_hpp
#define Square_hpp

#include <stdio.h>
#include "Object3D.hpp"
#include "Point.hpp"
#include "Triangle.hpp"
#include <cmath>

class Square : public Object3D
{
    GLfloat data[18] = {};
    Point *a;
    Point *b;
    Point *c;
    Point *d;
    Triangle* top;
    Triangle* bottom;
    Color *color2;
    
public:
    Square(Point *a,Point *b, Point *c, Point *d)
    {
        this->a = a;
        this->b = b;
        this->c = c;
        this->d = d;
        Triangle *triangle1 = new Triangle(this->a,this->b,this->c);
        Triangle *triangle2 = new Triangle(this->a,this->d,this->c);
        
        top = triangle1;
        bottom = triangle2;
    }
    Square(float side)
    {
        float half = side/2;
        this->a = new Point(-half,-half,-half);
//        this->a = a;
        this->b = new Point(a->x,a->y+side,a->z);
        this->c = new Point(a->x+side,a->y+side,a->z);
        this->d = new Point(a->x+side,a->y,a->z);
        Triangle *triangle1 = new Triangle(this->a,this->b,this->c);
        Triangle *triangle2 = new Triangle(this->a,this->d,this->c);
        
        top = triangle1;
        bottom = triangle2;
    }
  
    virtual GLfloat* getVertexArray()
    {
        data[0] = a->x;
        data[1] = a->y;
        data[2] = a->z;
        data[3] = b->x;
        data[4] = b->y;
        data[5] = b->z;
        data[6] = c->x;
        data[7] = c->y;
        data[8] = c->z;
        data[9] = a->x;
        data[10] = a->y;
        data[11] = a->z;
        data[12] = d->x;
        data[13] = d->y;
        data[14] = d->z;
        data[15] = c->x;
        data[16] = c->y;
        data[17] = c->z;
        return data;
    }
   
    Point* getA()
    {
        return this->a;
    }
    Point* getB()
    {
        return this->b;
    }
    Point* getC()
    {
        return this->c;
    }
    Point* getD()
    {
        return this->d;
    }
    
    int getNumberOfPrimitives()
    {
        return 2;
    }
    Object3D** getPrimitives(int* size)
    {
        *size = 2;
        Object3D** triangles = new Object3D*[2];//{triangle1,triangle2};
        triangles[0] = top;
        triangles[1] = bottom;
        return triangles;
    }
    int getSizeOf()
    {
        GLfloat sample;
        int size = sizeof(sample)*18;
        return size;
    }
    
    Square* copy()
    {
        Square* copy = new Square(this->getA()->copy(),this->getB()->copy(),this->getC()->copy(),this->getD()->copy());
        copy->moveX(this->getX());
        copy->moveY(this->getY());
        copy->moveZ(this->getZ());
        return copy;
    }
    
    float getSideSize()
    {
        float diff = abs(this->getA()->x -this->getC()->x);
        return diff;
    }
    int getNumberOfPoints()
    {
        return 4;
    }
    Point** getPoints()
    {
        Point** points = new Point*[4];
        points[0] = this->getA();
        points[1] = this->getB();
        points[2] = this->getC();
        points[3] = this->getD();
        return points;
    }
    
};

#endif /* Square_hpp */
