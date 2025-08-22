//
//  Face.h
//  test
//
//  Created by Freddis on 12.10.2018.
//  Copyright © 2018 Home-studio. All rights reserved.
//

#ifndef Face_hpp
#define Face_hpp

#include <stdio.h>
#include <vector>

#include "FaceVertex.h"
#include "Triangle.h"
class Face
{
    std::vector<FaceVertex*> vertexes;
public:
    void addVertex(FaceVertex* vertex)
    {
        vertexes.insert(vertexes.end(),vertex);
    }
    int getNumberOfTriangles()
    {
        if(vertexes.size() == 4)
        {
            return 2;
        }
        return 1;
    }
    Triangle** getTriangles()
    {
        auto size = getNumberOfTriangles();
        auto result = new Triangle*[size];
        result[0] = new Triangle(vertexes.at(0)->getVertex(),vertexes.at(1)->getVertex(),vertexes.at(2)->getVertex());
        result[0]->setTextureCoordsForVertex(0, vertexes.at(0)->getTextureX(),vertexes.at(0)->getTextureY());
        result[0]->setTextureCoordsForVertex(1, vertexes.at(1)->getTextureX(),vertexes.at(1)->getTextureY());
        result[0]->setTextureCoordsForVertex(2, vertexes.at(2)->getTextureX(),vertexes.at(2)->getTextureY());
        if(size > 1)
        {
            result[1] = new Triangle(vertexes.at(0)->getVertex(),vertexes.at(3)->getVertex(),vertexes.at(2)->getVertex());
            result[1]->setTextureCoordsForVertex(0, vertexes.at(0)->getTextureX(),vertexes.at(0)->getTextureY());
            result[1]->setTextureCoordsForVertex(1, vertexes.at(3)->getTextureX(),vertexes.at(3)->getTextureY());
            result[1]->setTextureCoordsForVertex(2, vertexes.at(2)->getTextureX(),vertexes.at(2)->getTextureY());
        }
        return result;
    }
};
#endif /* Face_hpp */
