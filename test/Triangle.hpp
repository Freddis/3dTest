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
    hs::Point* a;
    hs::Point* b;
    hs::Point* c;
    int vsize = 12;
    GLfloat data[36] = {};
    bool textureSwapped = false;
public:
    Triangle(hs::Point*a, hs::Point*b, hs::Point*c)
    {
        this->a = a;
        this->b = b;
        this->c = c;
        this->color = new Color(1.0f,0.0f,0.0f);
    }
    
    hs::Point* getCenter()
    {
        float x = (a->x+b->x+c->x)/3 + getX();
        float y = (a->y+b->y+c->y)/3 + getY();
        float z = (a->z+b->z+c->z)/3 + getZ();
        auto center = new hs::Point(x,y,z);
        return center;
    }
    hs::Point* createNormal()
    {
        hs::Point ab(b->x-a->x, b->y-a->y, b->z-a->z);
        hs::Point ac(c->x-a->x, c->y-a->y, c->z-a->z);
        hs::Point* a = &ab;
        hs::Point* b = &ac;
        float x = a->y*b->z - a->z*b->y;
        float y = a->z*b->x - a->x*b->z;
        float z = a->x*b->y - a->y*b->x;
        hs::Point* normal = new hs::Point(x,y,z);
        normal->normalize();
        return normal;
    }
    virtual GLfloat* getVertexArray()
    {
        hs::Point* arr[3] = {a,b,c};
        hs::Point* normal = this->createNormal();
        int texCoords[6] = {1,1,1,0,0,0};
        if(textureSwapped)
        {
            texCoords[0] = 0;
            texCoords[1] = 0;
            texCoords[2] = 0;
            texCoords[3] = 1;
            texCoords[4] = 1;
            texCoords[5] = 1;
        }
        for(int i =0; i < 3;i++)
        {
            data[0+i*vsize] = arr[i]->x;
            data[1+i*vsize] = arr[i]->y;
            data[2+i*vsize] = arr[i]->z;
            data[3+i*vsize] = texCoords[i*2];
            data[4+i*vsize] = texCoords[i*2+1];
            data[5+i*vsize] = this->color->r;
            data[6+i*vsize] = this->color->g;
            data[7+i*vsize] = this->color->b;
            data[8+i*vsize] = this->color->a;
            data[9+i*vsize] = normal->x;
            data[10+i*vsize] = normal->y;
            data[11+i*vsize] = normal->z;
        }
        delete[] normal;
        return data;
    }
    
    void swapTexturePosition()
    {
        this->textureSwapped = true;
    }
    
    int getSizeOf()
    {
        int size = sizeof(data);
        return size;
    }
    
    hs::Point* getA()
    {
        return this->a;
    }
    hs::Point* getB()
    {
        return this->b;
    }
    hs::Point* getC()
    {
        return this->c;
    }
    
    int getNumberOfPoints()
    {
        return 3;
    }
    hs::Point** getPoints()
    {
        hs::Point** points = new hs::Point*[3];
        points[0] = a;
        points[1] = b;
        points[2] = c;
        return points;
    }
    
    void flip()
    {
        auto tmp = a;
        a = c;
        c = tmp;
    }
    
   
};


#endif /* Triangle_hpp */
