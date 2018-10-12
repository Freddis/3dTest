//
//  ModelLoader.hpp
//  test
//
//  Created by Freddis on 12.10.2018.
//  Copyright © 2018 Home-studio. All rights reserved.
//

#ifndef ModelLoader_hpp
#define ModelLoader_hpp

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <streambuf>
#include <vector>

#include "Object3D.hpp"
#include "Face.hpp"
#include "Mesh.hpp"


class ModelLoader
{
public:
    Object3D* loadFromObj(const char* path, const char* texture)
    {
        std::ifstream file(path);
        std::stringstream buffer;
        buffer << file.rdbuf();
        
        std::vector<hs::Point*> vertexes;
        std::vector<hs::Point*> normals;
        std::vector<hs::Point*> textureCoords;
        std::vector<Face> faces;
        
        char delimeter = '\n';
        std::string line;
        while(std::getline(buffer,line,delimeter))
        {
            std::stringstream stream(line);
            std::string chunk;
            std::string chunks[10];
            std::fill(chunks, chunks+10, "");
            int i = 0;
            while(std::getline(stream,chunk,' '))
            {
                if(chunk == "" || chunk == "\r")
                {
                    continue;
                }
                chunks[i++] = chunk;
            }
            auto name = chunks[0];
            if(name == "v")
            {
                auto vertex = parseVertex(chunks);
                vertexes.insert(vertexes.end(),vertex);
            }
            if (name == "vn")
            {
                auto normal = parseNormal(chunks);
                normals.insert(normals.end(),normal);
            }
            if (name == "vt")
            {
                auto texcoord = parseTextureCoords(chunks);
                textureCoords.insert(textureCoords.end(),texcoord);
            }
            if(name == "f")
            {
                auto face = parseFace(chunks,vertexes,normals,textureCoords);
                faces.insert(faces.end(),face);
            }
            
            continue;
            //std::cout << line << std::endl;
            
           
        }
        std::cout << "Vertexes: " << vertexes.size() << std::endl;
        
        auto object = new Mesh(&faces);
        object->setTexture(texture);
        return object;
        
    }
    Face parseFace(std::string* chunks,std::vector<hs::Point*> vertexes, std::vector<hs::Point*> normals, std::vector<hs::Point*> textureCoords)
    {
        int len = 3;
        auto d = chunks[4];
        if(d != "")
        {
            len = 4;
        }
        float noTextureCoords[5] = {0,0.000000000006,0.000000000006,0.000000000006,0.000000000006};
        Face face;
        for(int i =1; i < len+1; i ++)
        {
            std::stringstream stream(chunks[i]);
            std::string part;
            std::string parts[3];
            int j = 0;
            while(std::getline(stream,part,'/'))
            {
                parts[j++] = part;
            }
            auto vertexIndex = getRealIndex(&vertexes,stoi(parts[0]));
            auto vertex = vertexes.at(vertexIndex);
            
            hs::Point* texture;
            if(parts[1] != "")
            {
                auto textureIndex = getRealIndex(&textureCoords,stoi(parts[1]));
                texture = textureCoords.at(textureIndex);;
            }
            else {
                texture = new hs::Point(noTextureCoords[i],noTextureCoords[i],0);
            }
            auto normalIndex = getRealIndex(&normals,stoi(parts[2]));
            auto normal = normals.at(normalIndex);
            
            auto fvertex = new FaceVertex(vertex,texture,normal);
            face.addVertex(fvertex);
        }
        return face;
    }
    int getRealIndex(std::vector<hs::Point*>* list,int index)
    {
        if(index < 0)
        {
            auto size = list->size();
            return size + index;
        }
        return index-1;
    }
    hs::Point* parseTextureCoords(std::string* chunks)
    {
        return parseVertex(chunks);
    }
    hs::Point* parseNormal(std::string* chunks)
    {
        return parseVertex(chunks);
    }
    hs::Point* parseVertex(std::string* chunks)
    {
        float x = stof(chunks[1]);
        float y = stof(chunks[2]);
        float z = 0;
        if(chunks[3] != "")
        {
            z = stof(chunks[3]);
        }
        auto vertex = new hs::Point(x,y,z);
        return vertex;
    }
};
#endif /* ModelLoader_hpp */
