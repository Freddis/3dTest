//
//  LightSource.hpp
//  test
//
//  Created by Freddis on 11.10.2018.
//  Copyright © 2018 Home-studio. All rights reserved.
//

#ifndef LightSource_hpp
#define LightSource_hpp

#include <stdio.h>

#include "Cube.hpp"

class LightSource : public Cube
{
public:
    LightSource(float side) : Cube(side)
    {
        
    }
    
    hs::Point getDirection()
    {
        hs::Point front(0,0,0);
        float pitch = getRotationX();
        float yaw = -1*getRotationY();
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.normalize();
        
        return front;
    }
};
#endif /* LightSource_hpp */
