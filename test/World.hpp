//
//  World.hpp
//  test
//
//  Created by Freddis on 24.09.2018.
//  Copyright © 2018 Home-studio. All rights reserved.
//

#ifndef World_hpp
#define World_hpp

#include <stdio.h>
#include "Object3D.hpp"

class World
{
    Object3D* objects[100];
    int objectCount = 0;
    Object3D** oldPrimitives;
    float x = 0;
    float y = 0;
    float z = 0;
    float rotationX = 0;
    float rotationY = 0;
    float rotationZ = 0;
    float fov = 90;
    float nearPane = 0.1f;
    float farPane = 1000.0f;
public:
    void addObject(Object3D* obj)
    {
        objects[objectCount] = obj;
        objectCount++;
    }
    
    Object3D** getPrimitives()
    {
        Object3D** primitives = new Object3D*[getNumberOfPrimitives()];
        int index = 0;
        for(int i =0; i < objectCount; i++)
        {
            
            Object3D* obj = objects[i];
            int primSize = 0;
            Object3D** objPrimitives = obj->getPrimitives(&primSize);
            for(int j =0; j < primSize; j++)
            {
                primitives[index++] = objPrimitives[j];
            }
        }
        return primitives;
    }
    
    int getNumberOfPrimitives()
    {
        int number = 0;
        for(int i =0; i < objectCount; i++)
        {
            number += objects[i]->getNumberOfPrimitives();
        }
        return number;
    }
    void moveX(float value)
    {
        this->x += value;
        for(int i =0; i < objectCount; i++)
        {
           // objects[i]->moveX(value);
        }
    }
    void moveY(float value)
    {
        this->y += value;
        for(int i =0; i < objectCount; i++)
        {
           // objects[i]->moveY(value);
        }
    }
    void moveZ(float value)
    {
        this->z += value;
        for(int i =0; i < objectCount; i++)
        {
          //  objects[i]->moveZ(value);
        }
    }
    
    void rotateX(float value)
    {
        this->rotationX += value;
    }
    void rotateY(float value)
    {
        this->rotationY += value;
    }
    void rotateZ(float value)
    {
        this->rotationZ += value;
    }
    float getRotationZ()
    {
        return this->rotationZ;
    }
    float getRotationY()
    {
        return this->rotationY;
    }
    float getRotationX()
    {
        return this->rotationX;
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
    float getFov()
    {
        return this->fov;
    }
    void setFov(float fov)
    {
        this->fov = fov;
    }
    float getNearPane()
    {
        return this->nearPane;
    }
    void setNearPane(float fov)
    {
        this->nearPane = fov;
    }
    float getFarPane()
    {
        return this->farPane;
    }
    void setFarPane(float fov)
    {
        this->farPane = fov;
    }
    
    
};
#endif /* World_hpp */
