//
//  Stage1.cpp
//  test
//
//  Created by Сарычев Алексей on 01.10.2018.
//  Copyright © 2018 Home-studio. All rights reserved.
//

#include "Stage1.hpp"
#include "Point.hpp"
#include "GameCycle.hpp"


Stage1::Stage1(World* world,Window* window) : Stage(world,window)
{
    const char* font = "fonts/OpenSans-Regular.ttf";
    typewriter = new TypeWriter(font,20,window->getWidth(),window->getHeight());
    
  
    
//    controls->updateWorldRotation();
    
    //    Square* floor = new Square(0.5);
    //    world->addObject(floor);
    
    
    Cube* cube = new Cube(0.05);
    rotatedObject = cube;
    cube->moveZ(0.2);
    cube->moveX(-0.07);
    cube->setTexture("textures/texture1.jpg");
    world->addObject(cube);
    
    
    car = new Cube(0.05);
    car->moveY(-0.070);
    //car->setTexture("textures/texture1.jpg");
    car->setColor(Color::getBlue());
    car->getFront()->setColor(Color::getCyan());
    //car->getFront()->moveZ(-0.05);
    world->addObject(car);
    this->controls = new ThirdPersonControls(car,world,window->getWindow());
    this->controls->activate();
    if(!window->isFullscreen())
    {
        this->controls->disableMouse();
    }
    
    camera = new Cube(0.025);
    camera->setColor(Color::getRed());
    world->addObject(camera);
    
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
    Square* floor = new Square(50);
    floor->setTexture("textures/texture3.bmp",100);
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
    floor->moveY(-0.1);
    
    Color* colorA = floor->getColor();
    Color* colorB = floor->getColor();
    float side = floor->getSideSize() + 0.01;
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
    
    Square* wall = new Square(1);
    wall->moveZ(- (wall->getSideSize()/2 + floor->getC()->z));
    wall->setColor(Color::getWhite());
    wall->moveY(wall->getSideSize()/2);
    world->addObject(wall);
    Square* rightWall = wall->copy();
    rightWall->setColor(Color::getBlack());
    rightWall->moveX(rightWall->getSideSize()+0.1);
    world->addObject(rightWall);
    Square* leftWall = wall->copy();
    leftWall->setColor(Color::getBlack());
    leftWall->moveX(-leftWall->getSideSize()-0.1);
    world->addObject(leftWall);
    
   // controls->focusOn(center);
}
    
void Stage1::process(GameCycle* cycle)
{
    auto world = getWorld();
    typewriter->clear();
    typewriter->printLine("FPS: " + std::to_string(cycle->getFPS()));
    typewriter->printLine("Polygons: " + std::to_string(world->getNumberOfPrimitives()));
    typewriter->printLine("Fov: " + std::to_string((int)world->getFov()));
//    std::cout << "fps:" << cycle->getFPS() << std::endl;
    
    typewriter->printLine("Car position x: " + std::to_string((float)car->getX()) + ", y: "  + std::to_string(car->getY()) + ", z: "  + std::to_string(car->getZ()));
    typewriter->printLine("Car rotation x: " + std::to_string((float)car->getRotationX()) + ", y: "  + std::to_string(car->getRotationX()) + ", z: "  + std::to_string(car->getRotationX()));
    typewriter->printLine("Cam position x: " + std::to_string((float)world->cameraPos.x) + ", y: "  + std::to_string(world->cameraPos.y) + ", z: "  + std::to_string(world->cameraPos.z));
    typewriter->printLine("Cam rotation x: " + std::to_string(world->getRotationX()) + ", y: "  + std::to_string(world->getRotationY()) + ", z: "  + std::to_string(world->getRotationZ()));
    std::string freelook = controls->isFreelook() ? "On" : "Off";
    typewriter->printLine("Freelook Mode: " + freelook);
       
}
    
void Stage1::beforeProcessing(double timer)
{
    auto world = getWorld();
    controls->process(timer);
    int step = 1;
    rotatedObject->rotateX(step*timer*100);
    rotatedObject->rotateY(step*timer*100);
    
    //display camera cube
    camera->moveX(-camera->getX() + controls->cameraPos.x);
    camera->moveY(-camera->getY() + controls->cameraPos.y);
    camera->moveZ(-camera->getZ() + controls->cameraPos.z);
    
//    car->moveX(-car->getX() + world->cameraPos.x +  world->cameraFront.x/3);
//    car->moveY(-car->getY() + world->cameraPos.y + world->cameraFront.y/3);
//    car->moveZ(-car->getZ() + world->cameraPos.z + world->cameraFront.z/3);
    
//    hs::Point p(world->cameraPos.x,world->cameraPos.y,world->cameraPos.z);
//    controls->focusOn(&p,car);
    
}

