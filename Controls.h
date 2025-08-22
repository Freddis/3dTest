//
//  Controls.h
//  test
//
//  Created by Freddis on 24.09.2018.
//  Copyright © 2018 Home-studio. All rights reserved.
//

#ifndef Controls_hpp
#define Controls_hpp

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <OpenGL/gl3.h>

#include <stdio.h>
#include <math.h>
#include <iostream>
#include "World.h"
#include "Object3D.h"


class Controls
{
    
protected:
    World* world;
    GLFWwindow *window;
    bool mouseEnabled = true;
public:
    double prevMouseX;
    double prevMouseY;
    static Controls* primaryControls;
    Controls(World* world, GLFWwindow *window)
    {
        this->world = world;
        this->window = window;
        glfwGetCursorPos(window, &this->prevMouseX, &this->prevMouseY);
        
    }
    void disableMouse()
    {
        mouseEnabled = false;
    }
    void enableMouse()
    {
        mouseEnabled = true;
    }
    void activate()
    {
        Controls* prevValue = Controls::primaryControls;
        Controls::primaryControls = this;
        if(prevValue != NULL)
        {
            return;
        }
        glfwSetKeyCallback(window,[](GLFWwindow* window, int key, int scancode, int action, int mods) -> void {
            Controls::primaryControls->processKeyCallBack(window, key, scancode, action, mods);
        });
        glfwSetCursorPosCallback(window,[](GLFWwindow* window ,double x,double y){
            Controls::primaryControls->processCursorPosition(window, x, y);
        });
    }
    
    void focusOn(glm::vec3 source,Object3D* target)
    {
        hs::Point sourceP(source.x,source.y,source.z);
        hs::Point targetP(target->getX(),target->getY(),target->getZ());
        focusOn(&sourceP,&targetP);
    }
    void focusOn(glm::vec3 source,hs::Point* target)
    {
        hs::Point sourceP(source.x,source.y,source.z);
        focusOn(&sourceP,target);
    }
    
    void focusOn(Object3D* source,hs::Point* target)
    {
        hs::Point sourceP(source->getX(),source->getY(),source->getZ());
        focusOn(&sourceP,target);
    }
    void focusOn(hs::Point* sourceP,Object3D* target)
    {
        hs::Point targetP(target->getX(),target->getY(),target->getZ());
        focusOn(sourceP,&targetP);
    }
    void focusOn(Object3D* source,Object3D* target)
    {
        hs::Point targetP(target->getX(),target->getY(),target->getZ());
        hs::Point sourceP(source->getX(),source->getY(),source->getZ());
        focusOn(&sourceP,&targetP);
    }
    void focusOn(hs::Point* object, hs::Point* target)
    {
        
        world->cameraPos.x = object->getX();
        world->cameraPos.y = object->getY();
        world->cameraPos.z = object->getZ();
        //Определяем направление
        world->cameraFront.x = target->getX() - object->getX() ;// + 0.001;
        world->cameraFront.y = target->getY() - object->getY();// + 0.001;
        world->cameraFront.z = target->getZ() - object->getZ();// + 0.001;
        if(target->getZ() == 0)
        {
            world->cameraFront.z += 0.001;
        }
        
        world->cameraFront = glm::normalize(world->cameraFront);;
        hs::Point delta(world->cameraFront.x,world->cameraFront.y, world->cameraFront.z);
        
        
        int axisDeltaX = 0;
        float tanX = delta.z / delta.x;
        float radX = std::atan(tanX);
        float degreeX = radX*180/M_PI;
        //Тут мы поворачиваем, так как тангенс повторяет значения в этой четверти
        axisDeltaX = delta.x < 0 ? -180 : 0;
        world->rotateY(-1*world->getRotationY() + axisDeltaX+degreeX);
        
        
        //В отличии от кручения по X. Вращение по Y использует все 3 координаты
        //Это связано с длиной катета, который находится между осей, а не на оси
        float tanY = delta.y / (sqrt(delta.x*delta.x + delta.z*delta.z));
        float radY = std::atan(tanY);
        float degreeY = radY*180/M_PI;
        world->rotateX(-1*world->getRotationX() + degreeY);
    }
    virtual void processCursorPosition(GLFWwindow* window ,double x,double y) = 0;
    virtual void processKeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods) = 0;
    virtual void process(double timepassed) = 0;
};
#endif /* Controls_hpp */
