//
//  Stage.hpp
//  test
//
//  Created by Freddis on 04.10.2018.
//  Copyright © 2018 Home-studio. All rights reserved.
//

#ifndef Stage_hpp
#define Stage_hpp

#include <stdio.h>

#include "World.hpp"
#include "Window.hpp"

class GameCycle;
class Stage
{
    World* world;
public:
    virtual void process(GameCycle* cycle) = 0;
    virtual void beforeProcessing(double timer) = 0;
    
    Stage(World* world,Window* window)
    {
        this->world = world;
    }

    World* getWorld()
    {
        return world;
    }
    virtual Color getClearColor()
    {
        Color color(0.5f,0.5f,0.5f);
        return color;
    }
};
#endif /* Stage_hpp */
