//
//  Controls.hpp
//  test
//
//  Created by Freddis on 24.09.2018.
//  Copyright © 2018 Home-studio. All rights reserved.
//

#ifndef Controls_hpp
#define Controls_hpp

#include <stdio.h>
#include <iostream>
#include "World.hpp"
#include "GLFW/glfw3.h"

class Controls
{
    World* world;
    float movementDegree = 1;
    float movementLength = 0.001;
    bool left = false;
    bool right = false;
    bool up = false;
    bool down = false;
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
    Controls(World* world)
    {
        this->world = world;
    }
    
    void processKeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
        std::cout << "Key: " << key << ", action: " << action << "\n";
        processArrows(key,action);
    }
    
    void process()
    {
        if(left)
        {
            moveLeft();
        }
        if(right)
        {
            moveRight();
        }
        if(up)
        {
        if(world->getRotationX() < 90)
        {
            world->rotateX(1);
            std::cout << "Rotation X: " << world->getRotationX() << std::endl;
        }
           // moveUp();
        }
        if(down)
        {
            if(world->getRotationX() > -90)
            {
              world->rotateX(-1);
              std::cout << "Rotation X: " << world->getRotationX() << std::endl;
            }
        }
        if(q)
        {
            world->moveZ(-movementLength);
        }
        if(w)
        {
            world->moveZ(movementLength);
        }
        if(z)
        {
            world->setFov(world->getFov() - movementDegree);
            std::cout << "fov: " << world->getFov() << std::endl;
        }
        if(x)
        {
            world->setFov(world->getFov() + movementDegree);
            std::cout << "fov: " << world->getFov() << std::endl;
        }
        if(b)
        {
            world->setFarPane(world->getFarPane() + 1);
            std::cout << "Far pane: " << world->getFarPane() << std::endl;
        }
        if(n)
        {
            world->setFarPane(world->getFarPane() - 1);
            std::cout << "Far Pane: " << world->getFarPane()<< std::endl;
        }
        if(c)
        {
            world->setNearPane(world->getNearPane() + 0.01);
            std::cout << "Near pane: " << world->getNearPane() << std::endl;
        }
        if(v)
        {
            world->setNearPane(world->getNearPane() - 0.01);
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
#endif /* Controls_hpp */
