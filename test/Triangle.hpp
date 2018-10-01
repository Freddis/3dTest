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
    GLfloat data[15] = {};
    bool textureSwapped = false;
public:
    Triangle(hs::Point*a, hs::Point*b, hs::Point*c)
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
        data[3] = 1.0;
        data[4] = 1.0;
        
        data[5] = b->x;
        data[6] = b->y;
        data[7] = b->z;
        if(textureSwapped)
        {
            data[8] = 0.0;
            data[9] = 1.0;
        }
        else
        {
            data[8] = 1.0;
            data[9] = 0.0;
        }
        
        data[10] = c->x;
        data[11] = c->y;
        data[12] = c->z;
        data[13] = 0.0;
        data[14] = 0.0;
        return data;
    }
    
    void swapTexturePosition()
    {
        this->textureSwapped = true;
    }
    
    int getSizeOf()
    {
        GLfloat sample;
        int size = sizeof(sample)*(15);
        return size;
    }
    
    hs::Point* getPointA()
    {
        return this->a;
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
    
   
};


#endif /* Triangle_hpp */
