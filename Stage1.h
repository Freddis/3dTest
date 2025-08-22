//
//  Stage1.h
//  test
//
//  Created by Сарычев Алексей on 01.10.2018.
//  Copyright © 2018 Home-studio. All rights reserved.
//

#ifndef Stage1_hpp
#define Stage1_hpp

#include <stdio.h>
#include "World.h"
#include "Window.h"
#include "Square.h"
#include "Cube.h"
#include "Object3D.h"
#include "FreelookControls.h"
#include "ThirdPersonControls.h"
#include "Controls.h"
#include "TypeWriter.h"
#include "Stage.h"
#include <map>

class Stage1 : public Stage
{
    ThirdPersonControls* controls;
    Object3D* rotatedObject;
    TypeWriter* typewriter;
    LightSource* car;
    Cube* camera;
    std::map<Triangle*,Object3D*> normals;
    
public:
    Stage1(World* world,Window* window);
    void process(GameCycle* cycle);
    void beforeProcessing(double timer);
    virtual Color getClearColor();
protected:
    void displayNormal(Triangle* trngl,Object3D* obj);
};
#endif /* Stage1_hpp */
