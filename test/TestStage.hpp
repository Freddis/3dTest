//
//  TestStage.hpp
//  test
//
//  Created by Freddis on 04.10.2018.
//  Copyright © 2018 Home-studio. All rights reserved.
//

#ifndef TestStage_hpp
#define TestStage_hpp

#include <stdio.h>
#include "Stage.hpp"
#include "GameCycle.hpp"
#include "FreelookControls.hpp"
#include "TypeWriter.hpp"
#include "Cube.hpp"
#include "Square.hpp"
#include "Timer.hpp"
#include <algorithm>
#include <iterator>

class TestStage : public Stage
{
    TypeWriter* typewriter;
    FreelookControls* controls;
    Cube* center;
    Timer* focusTimer;
    Object3D** bottom;
    Object3D** middle;
    Object3D** top;
    Object3D** all;
    bool freelookMode = false;
    int currentObj = 8+8+8-1;
public:
    TestStage(World* world,Window* window) : Stage(world,window)
    {
        const char* font = "fonts/OpenSans-Regular.ttf";
        typewriter = new TypeWriter(font,20,window->getWidth(),window->getHeight());
        
        this->controls = new FreelookControls(getWorld(),window->getWindow());
        this->controls->activate();
        if(!window->isFullscreen())
        {
            this->controls->disableMouse();
        }
        focusTimer = new Timer(2000);
        //    controls->updateWorldRotation();
        
        Color shade(0.3,0.3,0.3);
        float compasSide = 0.02;
        float compasRadius = 0.05;
        center = new Cube(compasSide);
        center->setColor(Color::getGrey());
        center->getColor()->mix(&shade);
        auto centerTop = center->copy();
        centerTop->moveY(+compasRadius);
        centerTop->getColor()->add(&shade);
        world->addObject(centerTop);
        auto centerBottom = center->copy();
        centerBottom->moveY(-compasRadius);
        centerBottom->getColor()->subtract(&shade);
        world->addObject(centerBottom);
        
        
        world->addObject(center);
        Cube* south = new Cube(compasSide);
        south->setColor(Color::getYellow());
        south->moveZ(+compasRadius);
        world->addObject(south);
        Cube* southTop = south->copy();
        southTop->moveY(+compasRadius);
        southTop->getColor()->add(&shade);
        world->addObject(southTop);
        Cube* southBottom = south->copy();
        southBottom->moveY(-compasRadius);
        southBottom->getColor()->subtract(&shade);
        world->addObject(southBottom);
        
        
        Cube* north = new Cube(compasSide);
        north->setColor(Color::getBlue());
        north->moveZ(-compasRadius);
        world->addObject(north);
        Cube* northTop = north->copy();
        northTop->moveY(+compasRadius);
        northTop->getColor()->add(&shade);
        world->addObject(northTop);
        Cube* northBottom = north->copy();
        northBottom->moveY(-compasRadius);
        northBottom->getColor()->subtract(&shade);
        world->addObject(northBottom);
        
        Cube* west = new Cube(compasSide);
        west->setColor(Color::getGreen());
        west->moveX(-compasRadius);
        world->addObject(west);
        Cube* westTop = west->copy();
        westTop->moveY(+compasRadius);
        westTop->getColor()->add(&shade);
        world->addObject(westTop);
        Cube* westBottom = west->copy();
        westBottom->moveY(-compasRadius);
        westBottom->getColor()->subtract(&shade);
        world->addObject(westBottom);
        
        Cube* east = new Cube(compasSide);
        east->setColor(Color::getRed());
        east->moveX(+compasRadius);
        world->addObject(east);
        Cube* eastTop = east->copy();
        eastTop->moveY(+compasRadius);
        eastTop->getColor()->add(&shade);
        world->addObject(eastTop);
        Cube* eastBottom = east->copy();
        eastBottom->moveY(-compasRadius);
        eastBottom->getColor()->subtract(&shade);
        world->addObject(eastBottom);
        
        
        
        Cube* southWest = new Cube(compasSide);
        southWest->moveX(west->getX()+south->getX());
        southWest->moveZ(west->getZ()+south->getZ());
        southWest->setColor(west->getColor()->copy());
        southWest->getColor()->mix(south->getColor());
        world->addObject(southWest);
        Cube* southWestTop = southWest->copy();
        southWestTop->setColor(southWest->getColor()->copy());
        southWestTop->getColor()->add(&shade);
        southWestTop->moveY(+compasRadius);
        world->addObject(southWestTop);
        Cube* southWestBottom = southWest->copy();
        southWestBottom->setColor(southWest->getColor()->copy());
        southWestBottom->getColor()->subtract(&shade);
        southWestBottom->moveY(-compasRadius);
        world->addObject(southWestBottom);
        
        Cube* northWest = new Cube(compasSide);
        northWest->moveX(west->getX()+north->getX());
        northWest->moveZ(west->getZ()+north->getZ());
        northWest->setColor(west->getColor()->copy());
        northWest->getColor()->mix(north->getColor());
        world->addObject(northWest);
        Cube* northWestTop = northWest->copy();
        northWestTop->getColor()->add(&shade);
        northWestTop->moveY(+compasRadius);
        world->addObject(northWestTop);
        Cube* northWestBottom = northWest->copy();
        northWestBottom->getColor()->subtract(&shade);
        northWestBottom->moveY(-compasRadius);
        world->addObject(northWestBottom);
        
        Cube* southEast = new Cube(compasSide);
        southEast->moveX(east->getX()+south->getX());
        southEast->moveZ(east->getZ()+south->getZ());
        southEast->setColor(east->getColor()->copy());
        southEast->getColor()->mix(south->getColor());
        world->addObject(southEast);
        Cube* southEastTop = southEast->copy();
        southEastTop->getColor()->add(&shade);
        southEastTop->moveY(+compasRadius);
        world->addObject(southEastTop);
        Cube* southEastBottom = southEast->copy();
        southEastBottom->getColor()->subtract(&shade);
        southEastBottom->moveY(-compasRadius);
        world->addObject(southEastBottom);
        
        Cube* northEast = new Cube(compasSide);
        northEast->moveX(east->getX()+north->getX());
        northEast->moveZ(east->getZ()+north->getZ());
        northEast->setColor(east->getColor()->copy());
        northEast->getColor()->mix(north->getColor());
        world->addObject(northEast);
        Cube* northEastTop = northEast->copy();
        northEastTop->getColor()->add(&shade);
        northEastTop->moveY(+compasRadius);
        world->addObject(northEastTop);
        Cube* northEastBottom = northEast->copy();
        northEastBottom->getColor()->subtract(&shade);
        northEastBottom->moveY(-compasRadius);
        world->addObject(northEastBottom);
        
        //
//        Square* floor = new Square(1);
//        floor->setTexture("textures/texture2.jpg",5);
//        floor->setColor(Color::getWhite());
//        floor->moveX(0.0);
//        //floor->moveZ(-0.2);
//        float size = floor->getSideSize();
//        floor->getC()->z += size;
//        floor->getC()->y = floor->getA()->y;
//        floor->getB()->z += size;
//        floor->getB()->y = floor->getA()->y;
//        floor->moveY(floor->getSideSize()/2);
//        world->addObject(floor);
//        floor->moveY(-0.1);
//
        Object3D** arr = new Object3D*[8];
        arr[0] = north;
        arr[1] = northWest;
        arr[2] = west;
        arr[3] = southWest;
        arr[4] = south;
        arr[5] = southEast;
        arr[6] = east;
        arr[7] = northEast;
        middle = arr;
        
        arr = new Object3D*[8];
        arr[0] = northTop;
        arr[1] = northWestTop;
        arr[2] = westTop;
        arr[3] = southWestTop;
        arr[4] = southTop;
        arr[5] = southEastTop;
        arr[6] = eastTop;
        arr[7] = northEastTop;
        top = arr;
        arr = new Object3D*[8];
        arr[0] = northBottom;
        arr[1] = northWestBottom;
        arr[2] = westBottom;
        arr[3] = southWestBottom;
        arr[4] = southBottom;
        arr[5] = southEastBottom;
        arr[6] = eastBottom;
        arr[7] = northEastBottom;
        bottom = arr;
        
        arr = new Object3D*[8+8+8+2];
        memcpy(&arr[0],middle,8*sizeof(Object3D*));
        memcpy(&arr[8],top,8*sizeof(Object3D*));
        memcpy(&arr[8+8],bottom,8*sizeof(Object3D*));
        arr[8+8+8] = centerBottom;
        arr[8+8+8+1] = centerTop;
        all = arr;
        
        //controls->focusOn(center,centerBottom);
    }
    
    void process(GameCycle* cycle)
    {
        auto world = getWorld();
        typewriter->clear();
        typewriter->printLine("FPS: " + std::to_string(cycle->getFPS()));
        typewriter->printLine("Polygons: " + std::to_string(world->getNumberOfPrimitives()));
        typewriter->printLine("Fov: " + std::to_string((int)world->getFov()));
        //    std::cout << "fps:" << cycle->getFPS() << std::endl;
        typewriter->printLine("Position x: " + std::to_string((float)world->cameraPos.x) + ", y: "  + std::to_string(world->cameraPos.y) + ", z: "  + std::to_string(world->cameraPos.z));
        typewriter->printLine("Rotation x: " + std::to_string(world->getRotationX()) + ", y: "  + std::to_string(world->getRotationY()) + ", z: "  + std::to_string(world->getRotationZ()));
        typewriter->printLine("Object: " + std::to_string(currentObj));
        typewriter->printLine("Timer: " + std::to_string(focusTimer->getPassedTimeMs()));
    }
    
    void beforeProcessing(double timer)
    {
        controls->process(timer);
        if(!focusTimer->isReady() || freelookMode)
        {
            return;
        }
    
        auto objects = all;
        auto size = 8+8+8+2;
        for(int i = currentObj; i < size; i++)
        {
            if(i >=  size-1)
            {
                i = -1;
                continue;
            }
            if(dynamic_cast<Square*>(objects[i]) != nullptr)
            {
                continue;
            }
            if(objects[i] == center)
            {
                continue;
            }
            currentObj = i+1;
            controls->focusOn(center,objects[i]);
            focusTimer->start();
            break;
           //objects[i]->rotateX(timer*10*i);
           //objects[i]->rotateY(timer*10*i);
        }
        
    }
};
#endif /* TestStage_hpp */
