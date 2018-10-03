//
//  Stage1.hpp
//  test
//
//  Created by Сарычев Алексей on 01.10.2018.
//  Copyright © 2018 Home-studio. All rights reserved.
//

#ifndef Stage1_hpp
#define Stage1_hpp

#include <stdio.h>
//#include "GameCycle.hpp"
#include "World.hpp"
#include "Window.hpp"
#include "Square.hpp"
#include "Cube.hpp"
#include "Object3D.hpp"
#include "Controls.hpp"
#include "TypeWriter.hpp"
class GameCycle;

class Stage1
{
    World* world;
    Window* window;
    Controls* controls;
    Object3D* rotatedObject;
    TypeWriter* typewriter;
public:
    Stage1(World* world,Window* window);
    void process(GameCycle* cycle);
    void beforeProcessing(double timer);
    World* getWorld();
};
#endif /* Stage1_hpp */
