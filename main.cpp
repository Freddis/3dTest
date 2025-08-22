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

#include "Window.h"
#include "World.h"
#include "GameCycle.h"
#include "Stage1.h"
#include "TestStage.h"

int main(int argc, char * argv[]) {
    
    bool fullscreen = true;
    Window* windowWrapper = new Window(fullscreen);
    World* world = new World();
    auto stage = new Stage1(world,windowWrapper);
//    auto stage = new TestStage(world,windowWrapper);
    GameCycle* cycle = new GameCycle(stage,windowWrapper);
    
    cycle->run();
    exit(windowWrapper->terminate());
}

