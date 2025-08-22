//
//  Cube.h
//  test
//
//  Created by Freddis on 23.09.2018.
//  Copyright © 2018 Home-studio. All rights reserved.
//

#ifndef Cube_hpp
#define Cube_hpp

#include <stdio.h>
#include "Object3D.h"
#include "Point.h"
#include "Square.h"
#include <cmath>

class Cube : public Object3D
{
    Square * squares[6] = {};
    Square * front;
    Square * back;
    Square * left;
    Square * right;
    Square * top;
    Square * bottom;
public:
    
    Cube(float side) : Cube(new Square(side))
    {
        
    }
    
    Cube(Square* front)
    {
        float side = front->getSideSize();
        front->getA()->z = +side/2;
        front->getB()->z = +side/2;
        front->getC()->z = +side/2;
        front->getD()->z = +side/2;
        
        Square* back = front->copy();
        //back->moveZ(back->getSideSize());
        
        back->getA()->z -= side;
        back->getB()->z -= side;
        back->getC()->z -= side;
        back->getD()->z -= side;
        back->flip();
        Square* left = new Square(front->getA(),back->getC(),back->getB(),front->getB());
        //left->flip();
        Square* top = new Square(front->getB(),back->getB(),back->getA(),front->getC());
        //top->flip();
        Square* right = new Square(front->getC(),back->getA(),back->getD(),front->getD());
        //right->flip();
        Square* bottom = new Square(front->getD(),back->getD(),back->getC(),front->getA());
        //w wbottom->flip();
        
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
        this->left = left;
        this->right = right;
        this->top = top;
        this->bottom = bottom;
        
        auto center = getCenter();
        worldX = center.getX();
        worldY = center.getY();
        worldZ = center.getZ();
    }
    
    Cube* copy()
    {
        auto cube = new Cube(front->getSideSize());
        cube->moveX(getX());
        cube->moveY(getY());
        cube->moveZ(getZ());
        cube->rotateX(getRotationX());
        cube->rotateY(getRotationY());
        cube->rotateZ(getRotationZ());
        
        int size;
        auto thisPrims = this->getPrimitives(&size);
        bool oneColor = this->getFront()->getColor() == this->getBack()->getColor();
        if(oneColor)
        {
            cube->setColor(this->getColor()->copy());
            delete[] thisPrims;
            return cube;
        }
        auto cubePrims = cube->getPrimitives(&size);
        for(auto i =0; i < size; i++)
        {
            cubePrims[i]->setColor(thisPrims[i]->getColor()->copy());
        }
        delete[] thisPrims;
        delete[] cubePrims;
        return cube;
    }
    virtual void moveX(float val)
    {
        this->worldX += val;
        for(int i =0; i < 6; i ++)
        {
            squares[i]->moveX(val);
        }
    }
    virtual void moveY(float val)
    {
        this->worldY += val;
        for(int i =0; i < 6; i ++)
        {
            squares[i]->moveY(val);
        }
    }
    virtual void moveZ(float val)
    {
        this->worldZ += val;
        for(int i =0; i < 6; i ++)
        {
            squares[i]->moveZ(val);
        }
    }
    virtual void setColor(Color* color)
    {
        Object3D::setColor(color);
        this->getFront()->setColor(color);
        this->getBack()->setColor(color);
    }
    void flip()
    {
        for(int i =0; i < 6; i++)
        {
            Square* square = this->squares[i];
            square->flip();
        }
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
    Square* getLeft()
    {
        return this->left;
    }
    Square* getRight()
    {
        return this->right;
    }
    Square* getTop()
    {
        return this->top;
    }
    Square* getBottom()
    {
        return this->bottom;
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
    
    hs::Point getCenter()
    {
        hs::Point* a = getFront()->getA();
        hs::Point* b = getBack()->getC();
        double x = (a->x+b->x)/2;
        double y = (a->y+b->y)/2;
        double z = (a->z+b->z)/2;
        float xx = x;
        float yy = y;
        float zz = z;
        hs::Point center(xx,yy,zz);
        return center;
    }
    
};

#endif /* Cube_hpp */
