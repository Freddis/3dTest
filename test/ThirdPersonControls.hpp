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
    bool freelookMode = false;
    Object3D* character;
    glm::vec3 characterFront;
    
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
        this->character = person;
        characterFront = glm::normalize(glm::vec3(0, 0, -1));
        updateCameraPosition();
        focusOn(cameraPos,person);
    }
    
    void processCursorPosition(GLFWwindow* window ,double x,double y)
    {
        if(!this->freelookMode)
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
    
    void updateCameraPosition()
    {
        glm::vec3 newCamPos;
        newCamPos.x = character->getX();
        newCamPos.y = character->getY()+cameraTopOffset;
        newCamPos.z = character->getZ();
        newCamPos += characterFront*-cameraDistance;
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
            hs::Point prev(character->getX(),character->getY(),character->getZ());
            auto front = movementLength*timer * characterFront;
            character->moveX(front.x);
            character->moveY(front.y);
            character->moveZ(front.z);
            updateCameraPosition();

        }
        if(down)
        {

            auto front = movementLength*timer * characterFront;
            character->moveX(-front.x);
            character->moveY(-front.y);
            character->moveZ(-front.z);
            updateCameraPosition();
        }
        if(right)
        {
            
            character->rotateY(-1*timer*movementDegree);
            float pitch = character->getRotationX();
            float yaw = -character->getRotationY();
            characterFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
            characterFront.y = sin(glm::radians(pitch));
            characterFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
            characterFront = glm::normalize(characterFront);
            updateCameraPosition();
        }
        if(left)
        {
            character->rotateY(1*timer*movementDegree);
            float pitch = character->getRotationX();
            float yaw = -character->getRotationY();
            characterFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
            characterFront.y = sin(glm::radians(pitch));
            characterFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
            characterFront = glm::normalize(characterFront);
            updateCameraPosition();
        }
        if(space)
        {
            std::cout << "Update camera rotation" << std::endl;
           // updateCameraPosition();
            //updateWorldRotation();
        }
        if(!freelookMode)
        {
            focusOn(cameraPos,character);
            return;
        }
        
        if(w)
        {
            world->cameraPos += movementLength*timer * world->cameraFront;
        }
        if(s)
        {
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
    
    bool isFreelook()
    {
        return freelookMode;
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
                    if(action == 1)
                    {
                        freelookMode = !freelookMode;
                    }
                    
                    break;
            }
        }
        
    }
};
#endif /* ThirdPersonControls_hpp */
