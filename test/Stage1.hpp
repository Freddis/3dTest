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
#include "Controls.hpp"
#include "TypeWriter.hpp"


class Stage1
{
    World* world;
    Window* window;
    Controls* controls;
    Object3D* rotatedObject;
    TypeWriter* typewriter;
public:
    
    Stage1(World* world,Window* window)
    {
        const char* font = "fonts/OpenSans-Regular.ttf";
        typewriter = new TypeWriter(font,20,window->getWidth(),window->getHeight());
        
        this->world = world;
        this->controls = new Controls(this->world,window->getWindow());
        this->controls->activate();
        if(!window->isFullscreen())
        {
            this->controls->disableMouse();
        }
        
        world->rotateY(-90);
        controls->updateWorldRotation();
        
        //    Square* floor = new Square(0.5);
        //    world->addObject(floor);
        
        Cube* cube = new Cube(0.05);
        rotatedObject = cube;
        cube->moveZ(0.2);
        cube->moveX(-0.07);
        world->addObject(cube);
        
        
        Cube* cube2 = new Cube(0.05);
        world->addObject(cube2);
        //
        Cube* cube3 = new Cube(0.2);
        cube3->moveX(0.3);
        cube3->moveY(0.5);
        cube3->moveZ(0.2);
        cube3->rotateY(45);
        cube3->rotateZ(45);
        world->addObject(cube3);
        //
        int p = 0;
        Object3D** prims = cube3->getPrimitives(&p);
        for(int i =0; i < p; i++)
        {
            Color* color = prims[i]->getColor();
            color->a = 0.3;
            prims[i]->setColor(color);
        }
        delete[] prims;
        //
        Square* floor = new Square(15.5);
        floor->setColor(Color::getWhite());
        floor->moveX(0.0);
        //floor->moveZ(-0.2);
        float size = floor->getSideSize();
        floor->getC()->z += size;
        floor->getC()->y = floor->getA()->y;
        floor->getB()->z += size;
        floor->getB()->y = floor->getA()->y;
        floor->moveY(floor->getSideSize()/2);
        world->addObject(floor);
        floor->moveY(-0.5);
        
        Color* colorA = floor->getColor();
        Color* colorB = floor->getColor();
        float side = floor->getSideSize();// + 0.0001;
        int xsize = 3;
        int zsize = 3;
        
        for(int j = 0; j < zsize; j++)
        {
            for(int i =0; i < xsize; i++)
            {
                Color* color = i%2 != 0 ? colorA : colorB;
                if(j %2 == 0)
                {
                    color = color == colorA ? colorB : colorA;
                }
                if(i == 0 && j == 0)
                {
                    continue;
                }
                float movementX = side*(i);
                float movementZ = side*(j);
                Square* plane1 = floor->copy();
                plane1->setColor(color);
                plane1->moveX(movementX);
                plane1->moveZ(movementZ);
                //plane1->moveY(0.5);
                world->addObject(plane1);
                Square* plane2 = floor->copy();
                plane2->setColor(color);
                plane2->moveX(-movementX);
                plane2->moveZ(-movementZ);
                world->addObject(plane2);
                if(j == 0)
                {
                    continue;
                }
                if(i == 0)
                {
                    continue;
                }
                Square* plane3 = floor->copy();
                plane3->setColor(color);
                plane3->moveX(-movementX);
                plane3->moveZ(movementZ);
                world->addObject(plane3);
                Square* plane4 = floor->copy();
                plane4->setColor(color);
                plane4->moveX(movementX);
                plane4->moveZ(-movementZ);
                world->addObject(plane4);
                
            }
        }
        
        //    Square* wall = new Square(1);
        //    wall->moveZ(- (wall->getSideSize()/2 + floor->getC()->z));
        //    wall->setColor(Color::getWhite());
        //    world->addObject(wall);
        //    Square* rightWall = wall->copy();
        //    rightWall->setColor(Color::getBlack());
        //    rightWall->moveX(rightWall->getSideSize());
        //    world->addObject(rightWall);
        //    Square* leftWall = wall->copy();
        //    leftWall->setColor(Color::getBlack());
        //    leftWall->moveX(-leftWall->getSideSize());
        //    world->addObject(leftWall);
    }
    
    void process(int fps)
    {
        typewriter->clear();
        typewriter->printLine("FPS: " + std::to_string(fps));
//        typewriter->printLine("Position x: " + std::to_string((float)world->cameraPos.x) + ", y: "  + std::to_string(world->cameraPos.y) + ", z: "  + std::to_string(world->cameraPos.z));
//        typewriter->printLine("Rotation x: " + std::to_string(world->getRotationX()) + ", y: "  + std::to_string(world->getRotationY()) + ", z: "  + std::to_string(world->getRotationZ()));
    }

    void beforeProcessing(double timer,int fps)
    {
        controls->process(timer);
        int step = 1;
        rotatedObject->rotateX(step*timer*100);
        rotatedObject->rotateY(step*timer*100);
    }
    
    World* getWorld()
    {
        return world;
    }
};
#endif /* Stage1_hpp */
