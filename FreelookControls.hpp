//
//  Controls.hpp
//  test
//
//  Created by Freddis on 24.09.2018.
//  Copyright © 2018 Home-studio. All rights reserved.
//

#ifndef FreelookControls_hpp
#define FreelookControls_hpp

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <OpenGL/gl3.h>

#include <stdio.h>
#include <math.h>
#include <iostream>
#include "World.hpp"
#include "Object3D.hpp"
#include "Controls.hpp"

class FreelookControls : public Controls
{
    float movementDegree = 100;
    float movementLength = 1;
    double mouseX;
    double mouseY;
    float mouseSensivity;
    
    bool left = false;
    bool right = false;
    bool up = false;
    bool down = false;
    bool space = false;
    bool q = false;
    bool w = false;
    bool a = false;
    bool s = false;
    bool d = false;
    bool z = false;
    bool x = false;
    bool c = false;
    bool v = false;
    bool b = false;
    bool n = false;
    
public:
    FreelookControls(World* world, GLFWwindow *window) : Controls(world,window)
    {
        
    }
    void processCursorPosition(GLFWwindow* window ,double x,double y)
    {
        if(!this->mouseEnabled)
        {
            return;
        }
        // std:: cout << "x: " << x << ", y: " << y << std::endl;
        float xoffset = x - prevMouseX;
        float yoffset = prevMouseY - y;
        prevMouseX = x;
        prevMouseY = y;
        
        GLfloat sensitivity = 0.1;
        xoffset *= sensitivity;
        yoffset *= sensitivity;
        
        if((world->getRotationX()+yoffset > 90 && yoffset > 0 )|| (world->getRotationX()+yoffset <= -90 && yoffset < 0))
        {
            yoffset = 0;
        }
        world->rotateX(yoffset);
        world->rotateY(xoffset);
        // std:: cout << "rotation x: " << world->getRotationX() << ", rotation y: " << world->getRotationY() << std::endl;
        updateWorldRotation();
    }
    void processKeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
        // std::cout << "Key: " << key << ", action: " << action << "\n";
        processArrows(key,action);
    }
    void updateWorldRotation()
    {
        float pitch = world->getRotationX();
        float yaw = world->getRotationY();
        world->cameraFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        world->cameraFront.y = sin(glm::radians(pitch));
        world->cameraFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        world->cameraFront = glm::normalize(world->cameraFront);
        
    }
    void process(double timepassed)
    {
        //std::cout << "timer: " << timepassed << std::endl;
        float timer = timepassed;
        if(up)
        {
            float value = movementDegree*timer;
            if(world->getRotationX() +value >= 90)
            {
                value = 89.999 - world->getRotationX();
            }
            world->rotateX(value);
            updateWorldRotation();
            // moveUp();
        }
        if(down)
        {
            float value = movementDegree*timer;
            if(world->getRotationX() - value <= -90)
            {
                value = 89.999 + world->getRotationX();
                //  std::cout << "Rotation X: " << world->getRotationX() << std::endl;
            }
            world->rotateX(-value);
            updateWorldRotation();
        }
        if(left)
        {
            
            world->rotateY(-1*timer*movementDegree);
            //    std::cout << "Rotation Y: " << world->getRotationY() << std::endl;
            updateWorldRotation();
            
        }
        if(right)
        {
            
            world->rotateY(1*timer*movementDegree);
            //     std::cout << "Rotation Y: " << world->getRotationY() << std::endl;
            updateWorldRotation();
            
        }
        if(space)
        {
            std::cout << "Update world rotation" << std::endl;
            updateWorldRotation();
        }
        if(w)
        {
            //             world->moveZ(movementLength);
            //             std::cout << "z: " << world->getZ() << std::endl;
            world->cameraPos += movementLength*timer * world->cameraFront;
        }
        if(s)
        {
            // world->moveZ(-movementLength);
            // std::cout << "z: " << world->getZ() << std::endl;
            world->cameraPos -= movementLength*timer * world->cameraFront;
        }
        if(a)
        {
            world->cameraPos -= glm::normalize(glm::cross(world->cameraFront, world->cameraUp)) * movementLength*timer;
        }
        if(d)
        {
            world->cameraPos += glm::normalize(glm::cross(world->cameraFront, world->cameraUp)) * movementLength*timer;
        }
        if(z)
        {
            world->setFov(world->getFov() - movementDegree*timer);
            std::cout << "fov: " << world->getFov() << std::endl;
        }
        if(x)
        {
            world->setFov(world->getFov() + movementDegree*timer);
            std::cout << "fov: " << world->getFov() << std::endl;
        }
        if(b)
        {
            world->setFarPane(world->getFarPane() + timer);
            std::cout << "Far pane: " << world->getFarPane() << std::endl;
        }
        if(n)
        {
            world->setFarPane(world->getFarPane() - timer);
            std::cout << "Far Pane: " << world->getFarPane()<< std::endl;
        }
        if(c)
        {
            world->setNearPane(world->getNearPane() + 0.01*timer);
            std::cout << "Near pane: " << world->getNearPane() << std::endl;
        }
        if(v)
        {
            world->setNearPane(world->getNearPane() - 0.01*timer);
            std::cout << "Near Pane: " << world->getNearPane() << std::endl;
        }
        
    }
   
    
protected:
    void processArrows(int keyCode, int action)
    {
        int arrows[] = {GLFW_KEY_RIGHT,GLFW_KEY_LEFT,GLFW_KEY_UP, GLFW_KEY_DOWN };
        
        bool inArray = std::find(arrows, std::end(arrows),keyCode);
        bool value = action == 0 ? false : true;
        if(inArray)
        {
            switch(keyCode)
            {
                case GLFW_KEY_LEFT :
                    left = value;
                    break;
                case GLFW_KEY_RIGHT :
                    right = value;
                    break;
                case GLFW_KEY_UP :
                    up = value;
                    break;
                case GLFW_KEY_DOWN :
                    down = value;
                    break;
                case GLFW_KEY_Q:
                    q = value;
                    break;
                case GLFW_KEY_W:
                    w = value;
                    break;
                case GLFW_KEY_A:
                    a = value;
                    break;
                case GLFW_KEY_S:
                    s = value;
                    break;
                case GLFW_KEY_D:
                    d = value;
                    break;
                case GLFW_KEY_X:
                    x = value;
                    break;
                case GLFW_KEY_Z:
                    z = value;
                    break;
                case GLFW_KEY_C:
                    c = value;
                    break;
                case GLFW_KEY_V:
                    v = value;
                    break;
                case GLFW_KEY_B:
                    b = value;
                    break;
                case GLFW_KEY_N:
                    n = value;
                    break;
                case GLFW_KEY_SPACE:
                    space = value;
                    break;
            }
        }
        
    }
    
    void moveRight()
    {
        //   world->rotateY(+movementStep);
        world->moveX(-movementLength);
    }
    void moveDown()
    {
        world->moveY(+movementLength);
    }
    void moveUp()
    {
        world->moveY(-movementLength);
    }
    void moveLeft()
    {
        //world->rotateY(-movementStep);
        world->moveX(movementLength);
    }
    
    
};
#endif /* FreelookControls_hpp */
