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
#include "World.hpp"
#include "Window.hpp"
#include "Square.hpp"
#include "Cube.hpp"
#include "Object3D.hpp"
#include "FreelookControls.hpp"
#include "ThirdPersonControls.hpp"
#include "Controls.hpp"
#include "TypeWriter.hpp"
#include "Stage.hpp"
#include <map>

class Stage1 : public Stage
{
    ThirdPersonControls* controls;
    Object3D* rotatedObject;
    TypeWriter* typewriter;
    Cube* car;
    Cube* camera;
    std::map<Triangle*,Object3D*> normals;
    
public:
    Stage1(World* world,Window* window);
    void process(GameCycle* cycle);
    void beforeProcessing(double timer);
protected:
    void displayNormal(Triangle* trngl,Object3D* obj);
};
#endif /* Stage1_hpp */
