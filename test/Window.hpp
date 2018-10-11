//
//  Window.hpp
//  test
//
//  Created by Сарычев Алексей on 01.10.2018.
//  Copyright © 2018 Home-studio. All rights reserved.
//

#ifndef Window_hpp
#define Window_hpp

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <OpenGL/gl3.h>

class Window
{
    GLFWwindow* window = NULL;
    GLFWmonitor* monitor = NULL;
    bool fullscreen;
    int screenWidth = 0;
    int screenHeight = 0;
public:
    
    Window(bool fullscreen)
    {
        this->fullscreen = fullscreen;
        glfwSetErrorCallback([](int error, const char* description)
        {
            fprintf(stderr, "Error: %s\n", description);
            exit(EXIT_FAILURE);
        });
        
        if (!glfwInit())
            exit(EXIT_FAILURE);
        
        glfwWindowHint(GLFW_SAMPLES, 2); //antialiasing x samples
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
        //glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE );
        
        GLFWmonitor* monitor = NULL;
        monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode * mode = glfwGetVideoMode(monitor);
        screenWidth = mode->width;
        screenHeight = mode->height;
        const char* title = "Test 3D";
        if(fullscreen)
        {
            window = glfwCreateWindow(screenWidth, screenHeight,title,monitor, NULL);
        }
        else {
            screenWidth = (int)screenWidth/1.5;
            screenHeight = (int)screenHeight/1.5;
            window = glfwCreateWindow(screenWidth,screenHeight, title, NULL, NULL);
            
        }
        if (!window)
        {
            glfwTerminate();
            exit(EXIT_FAILURE);
        }
        
        glfwMakeContextCurrent(window);
        if(fullscreen)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        
        // initialise GLEW
        glewExperimental = GL_TRUE; //stops glew crashing on OSX :-/
        if(glewInit() != GLEW_OK)
            throw std::runtime_error("glewInit failed");
        
        std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
        std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
        std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
        std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
        std::cout << "GLFW: " << glfwGetVersionString() << std::endl;
    }
    
    GLFWwindow* getWindow()
    {
        return window;
    }
    
    bool isFullscreen()
    {
        return this->fullscreen;
    }
    
    int terminate()
    {
        glfwDestroyWindow(window);
        glfwTerminate();
        return EXIT_SUCCESS;
    }
    
    int getWidth()
    {
        return screenWidth;
    }
    int getHeight()
    {
        return screenHeight;
    }
};
#endif /* Window_hpp */
