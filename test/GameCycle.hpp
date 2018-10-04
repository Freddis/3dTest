//
//  GameCycle.hpp
//  test
//
//  Created by Сарычев Алексей on 02.10.2018.
//  Copyright © 2018 Home-studio. All rights reserved.
//

#ifndef GameCycle_hpp
#define GameCycle_hpp

#include <stdio.h>
#include <iostream>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <OpenGL/gl3.h>
#include "Stage.hpp"
#include "Window.hpp"
#include "WorldPrinter.hpp"

class GameCycle
{
    Stage* currentStage;
    Window* windowWrapper;
    WorldPrinter* printer;
    int fps;
public:
    GameCycle(Stage* stage, Window* window)
    {
        currentStage = stage;
        windowWrapper = window;
        printer = new WorldPrinter();
    }
    
    void run()
    {
        int counter = 0;
        fps = 0;
        double timer =  0;
        double passed = 0;
        glfwSwapInterval(0);
        double frameStart = glfwGetTime();
        while (!glfwWindowShouldClose(windowWrapper->getWindow()))
        {
            currentStage->beforeProcessing(passed);
            glClearColor(0.5, 0.5, 0.5, 1);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            printer->draw(currentStage->getWorld(),windowWrapper->getWidth(),windowWrapper->getHeight());
            currentStage->process(this);
            glfwWaitEventsTimeout(1/3000);
            
            //end of the iteration
            double frameEnd = glfwGetTime();
            passed = frameEnd - frameStart;
            timer += passed;
            ++counter;
            if(timer >= 1)
            {
                fps = counter;
//                std::cout << "fps: " << fps << std::endl;
                timer = 0;
                counter = 0;
            }
            frameStart = glfwGetTime();
            glfwSwapBuffers(windowWrapper->getWindow());
        }
    }
    int getFPS()
    {
        return fps;
    }
};
#endif /* GameCycle_hpp */
