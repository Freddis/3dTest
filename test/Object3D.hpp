//
//  Object3D.hpp
//  test
//
//  Created by Freddis on 22.09.2018.
//  Copyright © 2018 Home-studio. All rights reserved.
//

#ifndef Object3D_hpp
#define Object3D_hpp

#include <stdio.h>
#include <OpenGL/gl3.h>
#include "Color.hpp"
#include "Point.hpp"

class Object3D
{
protected:
    Object3D* primitives[1] = {};
    Color* color = new Color(0.0f,0.0f,0.0f);
    float worldX = 0;
    float worldY = 0;
    float worldZ = 0;
    int rotationX  = 0;
public:
    
    virtual GLfloat* getWorldVertexes(){
        GLfloat* vertexArr = this->getVertexArray();
        int len = this->getSizeOf()/sizeof(GLfloat);
        int k =0;
        for(int j = 0; j < len; j++)
        {
            if(k == 0)
            {
                vertexArr[j] += worldX;
            }
            else if(k == 1)
            {
                vertexArr[j] += worldY;
            }
            else if(k == 2)
            {
                vertexArr[j] += worldZ;
            }
            else {
                vertexArr[j] += worldX;
                k = 0;
            }
            k++;
        }
        return vertexArr;
    }
    virtual GLfloat* getVertexArray(){
        int size = 0;
        Object3D** primitives = this->getPrimitives(&size);
        
        int primitivesSize = 0;
        for(int i = 0; i < size; i++)
        {
            Object3D* obj = primitives[i];
            primitivesSize += obj->getSizeOf()/sizeof(GLfloat);
        }
        //int *arr = new int[1000];
        GLfloat* array = new GLfloat[primitivesSize];
        int index = 0;
        for(int i = 0; i < size; i++)
        {
            Object3D* obj = primitives[i];
            int maxSize = obj->getSizeOf()/sizeof(GLfloat);
            GLfloat* vertexes  = obj->getVertexArray();
            for(int j =0; j < maxSize; j++)
            {
                array[index++] = vertexes[j];
            }
        }
        return array;
        
    }
    virtual int getSizeOf() = 0;
    Color* getColor(){
        return this->color;
    };
    void setColor(Color* color)
    {
        this->color = color;
        int size;
        if(this->getNumberOfPrimitives() <= 1)
        {
            return;
        }
        Object3D** primitives = this->getPrimitives(&size);
        for(int i =0; i < this->getNumberOfPrimitives(); i++)
        {
            primitives[i]->setColor(color);
        }
    }
    virtual Object3D** getPrimitives(int *size){
        primitives[0] = this;
        *size = 1;
        return primitives;
    };
    virtual Point** getPoints() = 0;
    virtual int getNumberOfPoints() = 0;
    
    virtual int getNumberOfPrimitives()
    {
        return 1;
    }
    
    void moveX(float value)
    {
//        Point** points = this->getPoints();
//        int len = this->getNumberOfPoints();
//        for(int i =0; i < len; i++)
//        {
//            points[i]->x += value;
//        }
        worldX += value;
        int size;
        if(this->getNumberOfPrimitives() <= 1)
        {
            return;
        }
        Object3D** primitives = this->getPrimitives(&size);
        for(int i =0; i < this->getNumberOfPrimitives(); i++)
        {
            primitives[i]->moveX(value);
        }
    }
    void moveY(float value)
    {
//        Point** points = this->getPoints();
//        int len = this->getNumberOfPoints();
//        for(int i =0; i < len; i++)
//        {
//            points[i]->y += value;
//        }
        worldY += value;
        int size;
        if(this->getNumberOfPrimitives() <= 1)
        {
            return;
        }
        Object3D** primitives = this->getPrimitives(&size);
        for(int i =0; i < this->getNumberOfPrimitives(); i++)
        {
            primitives[i]->moveY(value);
        }
    }
    void moveZ(float value)
    {
//        Point** points = this->getPoints();
//        int len = this->getNumberOfPoints();
//        for(int i =0; i < len; i++)
//        {
//            points[i]->z += value;
//        }
        worldZ += value;
        int size;
        if(this->getNumberOfPrimitives() <= 1)
        {
            return;
        }
        Object3D** primitives = this->getPrimitives(&size);
        for(int i =0; i < this->getNumberOfPrimitives(); i++)
        {
            primitives[i]->moveZ(value);
        }
    }
    void rotateX(float degree)
    {
        rotationX += degree;
        Point** points = this->getPoints();
        int len = this->getNumberOfPoints();
        for(int i =0; i < len; i++)
        {
            points[i]->rotateX(degree);
        }
    }
    int getRotationX()
    {
        return rotationX;
    }
    void rotateY(float degree)
    {
        Point** points = this->getPoints();
        int len = this->getNumberOfPoints();
        for(int i =0; i < len; i++)
        {
            points[i]->rotateY(degree);
        }
    }
    void rotateZ(float degree)
    {
        Point** points = this->getPoints();
        int len = this->getNumberOfPoints();
        for(int i =0; i < len; i++)
        {
            points[i]->rotateZ(degree);
        }
    }
    
    float getZ()
    {
        return worldZ;
    }
    float getX()
    {
        return worldX;
    }
    float getY()
    {
        return worldY;
    }
};

#endif /* Object3D_hpp */
