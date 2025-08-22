//
//  Mesh.h
//  test
//
//  Created by Freddis on 12.10.2018.
//  Copyright © 2018 Home-studio. All rights reserved.
//

#ifndef Mesh_hpp
#define Mesh_hpp

#include <stdio.h>
#include <vector>

#include "Point.h"
#include "Triangle.h"
#include "Object3D.h"
#include "Face.h"

class Mesh : public Object3D
{
    hs::Point** vertexes;
    Triangle** primitives;
    int numberOfVertexes;
public:
    Mesh(std::vector<Face>* faces)
    {
        int primitiveSize = 0;
        for(int i =0; i < faces->size(); i++)
        {
            primitiveSize += faces->at(i).getNumberOfTriangles();
        }
        
        primitives = new Triangle*[primitiveSize];
        numberOfVertexes = primitiveSize*3;
        int index = 0;
        for(int i =0; i < faces->size(); i++)
        {
            auto numberOfTriangles = faces->at(i).getNumberOfTriangles();
            auto triangles = faces->at(i).getTriangles();
            for(int j=0; j < numberOfTriangles; j++)
            {
                primitives[index++] = triangles[j];
            }
        }
//        numberOfVertexes = primitiveSize*3;
//        numberOfVertexes = vertexes->size();
//        this->vertexes = new hs::Point*[numberOfVertexes];
//
//        primitives = new Triangle*[numberOfVertexes/3];
//        for(int i =0; i < numberOfVertexes; i++)
//        {
//            this->vertexes[i] = vertexes->at(i);
//            this->vertexes[i]->multiply(0.0005);
//        }
//        int index = 0;
//        for(int j=0; j < numberOfVertexes; j+=3)
//        {
//            auto a = this->vertexes[j];
//            auto b = this->vertexes[j+1];
//            auto c = this->vertexes[j+2];
//            auto prim = new Triangle(a,b,c);
//            primitives[index++] = prim;
//        }
    }
    int getNumberOfPoints()
    {
        return numberOfVertexes;
    }
    int getNumberOfPrimitives()
    {
        return numberOfVertexes/3;
    }
    Object3D** getPrimitives(int* size)
    {
        auto numberOfPrims = getNumberOfPrimitives();
        *size = numberOfPrims;
        auto prims = new Object3D*[numberOfPrims];
        for(int i =0; i < numberOfPrims; i ++)
        {
            prims[i] = primitives[i];
        }
        return prims;
    }
    
    hs::Point** getPoints()
    {
        return vertexes;
    }
    int getSizeOf()
    {
        int result = numberOfVertexes*sizeof(GLfloat);
        return result;
    }
};
#endif /* Mesh_hpp */
