//
//  ThirdPersonControls.hpp
//  test
//
//  Created by Freddis on 05.10.2018.
//  Copyright © 2018 Home-studio. All rights reserved.
//

#ifndef ThirdPersonControls_hpp
#define ThirdPersonControls_hpp

#include <stdio.h>
#include "Controls.hpp"
#include "Object3D.hpp"
#include "World.hpp"
#include "Window.hpp"

class ThirdPersonControls : public Controls
{
    float movementDegree = 100;
    float movementLength = 0.3;
    double mouseX;
    double mouseY;
    float mouseSensivity;
    
    glm::vec3 personPos;
    Object3D* person;
   
    glm::vec3 carFront;
    
    float cameraDistance = 0.4f;
    float cameraTopOffset = 0.1f;
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
public:
    glm::vec3 cameraPos;
    
    ThirdPersonControls(Object3D* person, World* world, GLFWwindow *window) : Controls(world,window)
    {
        this->person = person;
        
        carFront = glm::normalize(glm::vec3(0, 0, -0.1));
        
        updateCameraPosition();
        focusOn(cameraPos,person);
    }
    
    void processCursorPosition(GLFWwindow* window ,double x,double y)
    {
        //return;
        if(!this->mouseEnabled)
        {
          //  return;
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
    
    void updateCameraPosition()
    {
        glm::vec3 newCamPos;
        newCamPos.x = person->getX();
        newCamPos.y = person->getY()+cameraTopOffset;
        newCamPos.z = person->getZ();
        newCamPos += carFront*-cameraDistance;
        cameraPos = newCamPos;
        
        hs::Point cam(newCamPos.x,newCamPos.y,newCamPos.z);
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
//            float value = movementDegree*timer;
//            if(world->getRotationX() +value >= 90)
//            {
//                value = 89.999 - world->getRotationX();
//            }
//            world->rotateX(value);
//            updateWorldRotation();
            // moveUp();
            hs::Point prev(person->getX(),person->getY(),person->getZ());
            auto front = movementLength*timer * carFront;
            person->moveX(front.x);
            person->moveY(front.y);
            person->moveZ(front.z);
         
            updateCameraPosition();
         
          //  personPos += movementLength*timer * world->cameraFront;
          //  person->setPosition(personPos.x,personPos.y,personPos.z);
        }
        if(down)
        {
//            float value = movementDegree*timer;
//            if(world->getRotationX() - value <= -90)
//            {
//                value = 89.999 + world->getRotationX();
//                //  std::cout << "Rotation X: " << world->getRotationX() << std::endl;
//            }
//            world->rotateX(-value);
            auto front = movementLength*timer * carFront;
            person->moveX(-front.x);
            person->moveY(-front.y);
            person->moveZ(-front.z);
            updateCameraPosition();
//            updateWorldRotation();
        }
        if(left)
        {
           
//            world->rotateY(-1*timer*movementDegree);
            
            person->rotateY(-1*timer*movementDegree);
            float pitch = person->getRotationX();
            float yaw = person->getRotationY();
            carFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
            carFront.y = sin(glm::radians(pitch));
            carFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
            carFront = glm::normalize(carFront);
            updateCameraPosition();
         
            
            //    std::cout << "Rotation Y: " << world->getRotationY() << std::endl;
//            updateWorldRotation();
            
        }
        if(right)
        {
            person->rotateY(1*timer*movementDegree);
            float pitch = person->getRotationX();
            float yaw = person->getRotationY();
            carFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
            carFront.y = sin(glm::radians(pitch));
            carFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
            carFront = glm::normalize(carFront);
            updateCameraPosition();
//            world->rotateY(1*timer*movementDegree);
//            person->rotateY(1*timer*movementDegree);
            //     std::cout << "Rotation Y: " << world->getRotationY() << std::endl;
//            updateWorldRotation();
            
        }
        if(space)
        {
            std::cout << "Update camera rotation" << std::endl;
            updateCameraPosition();
            focusOn(cameraPos,person);
            
            //updateWorldRotation();
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
                case GLFW_KEY_SPACE:
                    space = value;
                    break;
            }
        }
        
    }
};
#endif /* ThirdPersonControls_hpp */
