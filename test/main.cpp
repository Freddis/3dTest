//
//  main.cpp
//  test
//
//  Created by Freddis on 17.01.17.
//  Copyright © 2017 Home-studio. All rights reserved.
//

#include <iostream>
#include <stdlib.h>
#include <string>

#include "Window.hpp"
#include "World.hpp"
#include "Stage1.hpp"
#include "Controls.cpp"
#include "TypeWriter.hpp"
#include "GameCycle.hpp"

Controls* controls;

int main(int argc, char * argv[]) {
    
    bool fullscreen = true;
    Window* windowWrapper = new Window(fullscreen);
    
    World* world = new World();
    controls = new Controls(world,windowWrapper->getWindow());
    Stage1* stage = new Stage1(world,controls,windowWrapper);

    
    glfwSetKeyCallback(windowWrapper->getWindow(),[](GLFWwindow* window, int key, int scancode, int action, int mods) -> void {
        controls->processKeyCallBack(window, key, scancode, action, mods);
    });
    
    if(windowWrapper->isFullscreen())
    {
        glfwSetCursorPosCallback(windowWrapper->getWindow(),[](GLFWwindow* window ,double x,double y){
                controls->processCursorPosition(window, x, y);
        });
    }
    
    GameCycle* cycle = new GameCycle(stage,windowWrapper);
    cycle->run();
    
    exit(windowWrapper->terminate());
}

