//
//  Cube.hpp
//  test
//
//  Created by Freddis on 23.09.2018.
//  Copyright © 2018 Home-studio. All rights reserved.
//

#ifndef Cube_hpp
#define Cube_hpp

#include <stdio.h>
#include "Object3D.hpp"
#include "Point.hpp"
#include "Square.hpp"
#include <cmath>

class Cube : public Object3D
{
    Square * squares[6] = {};
    Square * front;
    Square * back;
    int worldX = 0;
    int worldY = 0;
public:
    
    Cube(float side) : Cube(new Square(side))
    {
        
    }
    
    Cube(Square* front)
    {
        float side = front->getSideSize();
        front->getA()->z = -side/2;
        front->getB()->z = -side/2;
        front->getC()->z = -side/2;
        front->getD()->z = -side/2;
        Square* back = front->copy();
        //back->moveZ(back->getSideSize());
        
        back->getA()->z += side;
        back->getB()->z += side;
        back->getC()->z += side;
        back->getD()->z += side;
        Square* left = new Square(front->getA(),back->getA(),back->getB(),front->getB());
        Square* top = new Square(front->getB(),back->getB(),back->getC(),front->getC());
        Square* right = new Square(front->getC(),back->getC(),back->getD(),front->getD());
        Square* bottom = new Square(front->getD(),back->getD(),back->getA(),front->getA());
        
        front->setColor(Color::getBlue());
        back->setColor(Color::getRed());
        left->setColor(Color::getCyan());
        top->setColor(Color::getGreen());
        right->setColor(Color::getMagenta());
        bottom->setColor(Color::getYellow());
        
        this->squares[0] = front;
        this->squares[1] = back;
        this->squares[2] = left;
        this->squares[3] = top;
        this->squares[4] = right;
        this->squares[5] = bottom;
        
        this->front = front;
        this->back = back;
    }
    
    Object3D** getPrimitives(int* size)
    {
        *size = 12;
        Object3D** primitives = new Object3D*[12];
        int index = 0;
        for(int i =0; i < 6; i++)
        {
            Square* square = this->squares[i];;
            int primsize = 0;
            Object3D** prims = square->getPrimitives(&primsize);
            for(int j=0; j < primsize; j++)
            {
                Object3D* prim = prims[j];
                primitives[index++] = prim;
            }
            delete[] prims;
        }
        return primitives;
    }
    int getNumberOfPrimitives()
    {
        return 12;
    }
    
    int getSizeOf()
    {
        GLfloat sample;
        int size = sizeof(sample)*18*8;
        return size;
    }
    Square* getFront()
    {
        return this->front;
    }
    Square* getBack()
    {
        return this->back;
    }
    
    int getNumberOfPoints()
    {
        return 8;
    }
    hs::Point** getPoints()
    {
        hs::Point** points = new hs::Point*[8];
        points[0] = this->getFront()->getA();
        points[1] = this->getFront()->getB();
        points[2] = this->getFront()->getC();
        points[3] = this->getFront()->getD();
        points[4] = this->getBack()->getA();
        points[5] = this->getBack()->getB();
        points[6] = this->getBack()->getC();
        points[7] = this->getBack()->getD();
        return points;
    }
    
    hs::Point* getCenter()
    {
        hs::Point* a = getFront()->getA();
        hs::Point* b = getBack()->getC();
        double x = (a->x+b->x)/2;
        double y = (a->y+b->y)/2;
        double z = (a->z+b->z)/2;
        float xx = x;
        float yy = y;
        float zz = z;
        hs::Point* center = new hs::Point(xx,yy,zz);
        return center;
    }
    
};

#endif /* Cube_hpp */
