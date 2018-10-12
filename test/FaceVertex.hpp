//
//  FaceVertex.hpp
//  test
//
//  Created by Freddis on 12.10.2018.
//  Copyright © 2018 Home-studio. All rights reserved.
//

#ifndef FaceVertex_hpp
#define FaceVertex_hpp

#include <stdio.h>

#include "Point.hpp"

class FaceVertex
{
    hs::Point* vertex;
    hs::Point* normal;
    hs::Point* textureCoords;
public:
   
    FaceVertex(hs::Point* vertex, hs::Point* textureCoords,hs::Point* normal)
    {
        this->vertex = vertex;
        this->normal = normal;
        this->textureCoords = textureCoords;
    }

    hs::Point* getVertex()
    {
        return vertex;
    }
    hs::Point* getNormal()
    {
        return normal;
    }
    float getTextureX()
    {
        return textureCoords->getX();
    }
    float getTextureY()
    {
        return textureCoords->getY();
    }
};

#endif /* FaceVertex_hpp */
