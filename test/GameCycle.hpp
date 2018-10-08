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
    double startTime;
    long frames = 0;
public:
    GameCycle(Stage* stage, Window* window)
    {
        currentStage = stage;
        windowWrapper = window;
        printer = new WorldPrinter();
    }
    
    void run()
    {
        startTime = glfwGetTime();
        int counter = 0;
        fps = 0;
        double timer =  0;
        double passed = 0;
        glfwSwapInterval(0);
        double frameStart = glfwGetTime();
        while (!glfwWindowShouldClose(windowWrapper->getWindow()))
        {
            currentStage->beforeProcessing(passed);
            auto color = currentStage->getClearColor();
            glClearColor(color.r,color.g,color.b, 1);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            printer->draw(currentStage->getWorld(),windowWrapper->getWidth(),windowWrapper->getHeight());
            currentStage->process(this);
            glfwWaitEventsTimeout(1/3000);
            
            //end of the iteration
            double frameEnd = glfwGetTime();
            passed = frameEnd - frameStart;
            timer += passed;
            ++counter;
            ++frames;
            if(timer >= 1)
            {
                fps = counter;
                //std::cout << "fps: " << fps << std::endl;
                timer = 0;
                counter = 0;
            }
            frameStart = glfwGetTime();
            glfwSwapBuffers(windowWrapper->getWindow());
        }
    }
    float getFrameTime()
    {
        double frameEnd = glfwGetTime();
        double passed = frameEnd - startTime;
        float frameTime = passed/ frames;
        return frameTime;
    }
    int getFPS()
    {
        return fps;
    }
};
#endif /* GameCycle_hpp */
