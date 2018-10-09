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

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class World
{
    Object3D* objects[50000];
    GLfloat vertexes[1000000];
    int objectCount = 0;
    Object3D** oldPrimitives;
    Object3D* lightSource;
    float x = 0;
    float y = 0;
    float z = 0;
    float rotationX = 0;
    float rotationY = -90;
    float rotationZ = 0;
    float fov = 45;
    float nearPane = 0.02f;
    float farPane = 1000.0f;
   
public:
    glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  0.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f,  -1.0f);
    glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

    void addObject(Object3D* obj)
    {
        objects[objectCount] = obj;
        objectCount++;
    }
    Object3D** getObjects()
    {
        return objects;
    }
    int getNumberOfObjects()
    {
        return objectCount;
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
            delete[] objPrimitives;
        }
        return primitives;
    }
    
    GLfloat* getVertexes()
    {
        Object3D** objects = this->getPrimitives();
        int size = this->getNumberOfPrimitives();
        //int vertexNumber = this->getVertexNumber();
        GLfloat* vertexes = this->vertexes; //new GLfloat[vertexNumber*(sizeof(GLfloat)*5)];
        int z = 0;
        for(int i =0; i < size; i++)
        {
            Object3D* obj = objects[i];
            int vertexCount = obj->getSizeOf()/sizeof(GLfloat);
            GLfloat* vertexArr = obj->getWorldVertexes();
            
            for(int j =0;j < vertexCount; j++)
            {
                vertexes[z++] = vertexArr[j];
            }
        }
        delete[] objects;
        return vertexes;
    }
    
    int getVertexNumber()
    {
        //int vertexNumber = this->getNumberOfPrimitives()*3;
        int vertexNumber = 0;
        int size = this->getNumberOfPrimitives();
        Object3D** objects = this->getPrimitives();
        //int vertexSize = 0;
        for(int i =0; i < size; i++)
        {
            Object3D* obj = objects[i];
           // vertexSize += obj->getSizeOf();
            vertexNumber += obj->getNumberOfPoints();
        }
        delete[] objects;
        return vertexNumber;
    }
    int getVertexSize()
    {
        int size = getVertexNumber()*sizeof(GLfloat)*12;
        return size;
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
    }
    void moveY(float value)
    {
        this->y += value;
    }
    void moveZ(float value)
    {
        this->z += value;
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
    
    void clear()
    {
        objectCount = 0;
    }
    void setLightSource(Object3D* lightSource)
    {
        this->lightSource = lightSource;
    }
    Object3D* getLightSource()
    {
        return lightSource;
    }
};
#endif /* World_hpp */
