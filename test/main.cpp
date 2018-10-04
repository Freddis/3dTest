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
#include "GameCycle.hpp"
#include "Stage1.hpp"
#include "TestStage.hpp"

int main(int argc, char * argv[]) {
    
    bool fullscreen = false;
    Window* windowWrapper = new Window(fullscreen);
    World* world = new World();
    auto stage = new Stage1(world,windowWrapper);
//    auto stage = new TestStage(world,windowWrapper);
    GameCycle* cycle = new GameCycle(stage,windowWrapper);
    
    cycle->run();
    exit(windowWrapper->terminate());
}

