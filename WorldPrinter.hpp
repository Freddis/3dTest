//
//  WorldPrinter.hpp
//  test
//
//  Created by Сарычев Алексей on 02.10.2018.
//  Copyright © 2018 Home-studio. All rights reserved.
//

#ifndef WorldPrinter_hpp
#define WorldPrinter_hpp

#include <stdio.h>
#include <iostream>
#include <map>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <OpenGL/gl3.h>

#include "World.hpp"
#include "Object3D.hpp"
#include "Square.hpp"
#include "TextureLoader.hpp"
#include "ShaderLoader.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class WorldPrinter
{
    GLuint gVAO = 0;
    GLuint gVBO = 0;
    GLuint gEBO = 0;
    GLint defaultShaderProgram;
    GLint defaultLightsShaderProgram;
    GLint defaultLightsAndShadowsShaderProgram;
    GLint shadowsShaderProgram;
    GLint mirrorShaderProgram;
    TextureLoader* loader;
    GLuint defaultTexture;
    GLuint textures[1000000] = {};
    std::map<std::string,GLuint> loadedTextures;
    
public:
    WorldPrinter()
    {
        auto shaders = new ShaderLoader();
        shadowsShaderProgram = shaders->load("shaders/shadow");
        mirrorShaderProgram = shaders->load("shaders/mirror");
        defaultLightsShaderProgram = shaders->load("shaders/defaultLights");
        defaultLightsAndShadowsShaderProgram = shaders->load("shaders/defaultLightsShadows");
        defaultShaderProgram = shaders->load("shaders/default");
        
        loader = new TextureLoader();
        
        std::string defaultTexName = "textures/white.jpg";
        defaultTexture = loader->loadWithDevil(defaultTexName.c_str());
        std::pair<std::string,GLuint> pair(defaultTexName,defaultTexture);
        loadedTextures.insert(pair);
    }
    
    void draw(World* world,int windowWidth, int windowHeight)
    {
//        renderSimple(world,windowWidth,windowHeight);
//        renderWithLights(world,windowWidth,windowHeight);
        renderWithShadows(world,windowWidth,windowHeight);
    }
    
protected:
    void renderSimple(World* world,int windowWidth, int windowHeight)
    {
        glUseProgram(defaultShaderProgram);
        loadMatrixes(defaultShaderProgram,world,windowWidth,windowHeight);
        loadTextures(world);
        renderPerTexture(world);
    }
    void renderWithLights(World* world,int windowWidth, int windowHeight)
    {
        glUseProgram(defaultLightsShaderProgram);
        loadMatrixes(defaultLightsShaderProgram,world,windowWidth,windowHeight);
        loadLights(defaultLightsShaderProgram,world);
        loadTextures(world);
        renderPerTexture(world);
    }
    void renderWithShadows(World* world,int windowWidth, int windowHeight)
    {
        glUseProgram(shadowsShaderProgram);
        unsigned int depthMapFBO;
        glGenFramebuffers(1, &depthMapFBO);
        //const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
        int multi = 2;
        const unsigned int SHADOW_WIDTH = windowWidth*multi, SHADOW_HEIGHT = windowHeight*multi;
        
        GLuint depthMap;
        glGenTextures(1, &depthMap);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                     SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
        //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, depthMap, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        
        //rendering shadows
        loadLightMatrixes(shadowsShaderProgram,world,windowWidth,windowHeight);
        //glCullFace(GL_FRONT);
        renderAllVertexes(world);
        //glCullFace(GL_BACK);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
//        glViewport(0, 0, windowWidth*2, windowHeight*2);
        //return;
        
        //rendering world
        glUseProgram(defaultLightsAndShadowsShaderProgram);
        loadTextures(world);
        loadLightMatrixes(defaultLightsAndShadowsShaderProgram,world,windowWidth,windowHeight);
        loadMatrixes(defaultLightsAndShadowsShaderProgram,world,windowWidth,windowHeight);
        loadLights(defaultLightsAndShadowsShaderProgram,world);
        
        
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        GLint shadowMapUniform = glGetUniformLocation(defaultLightsAndShadowsShaderProgram, "shadowMap");
        glUniform1i(shadowMapUniform, 1);
        glActiveTexture(GL_TEXTURE0);
        renderPerTexture(world);
        
        //display shadows on screen
//        float pixelSizeX = 2.0 / windowWidth;
//        float pixelSizeY = 2.0 / windowHeight;
//        this->renderMirror(world,depthMap,-1+pixelSizeX*10,-1 + pixelSizeY*10,-0.3,-0.3);
        
        glDeleteTextures(1, &depthMap);
        glDeleteFramebuffers(1,&depthMapFBO);
    }
    void loadLightMatrixes(GLint shaderProgram,World* world,int windowWidth, int windowHeight)
    {
        
        float near_plane = 0.01f, far_plane = 100.5f;
        glm::mat4 projection = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, near_plane, far_plane);
//        projection = glm::perspective(glm::radians(world->getFov()), (float)windowWidth / windowHeight, world->getNearPane(), world->getFarPane());
        auto light = world->getLightSource();
        auto target = light->getDirection();
        auto position = light->getPosition();
//        hs::Point position(0,0.5,0);
    
        target.add(&position);
        //target.y -= 0.3;
        
//        glm::mat4 view = glm::lookAt(glm::vec3(0,0,0),position.toVec3(),world->cameraUp);
        glm::mat4 view = glm::lookAt(position.toVec3(),target.toVec3(),world->cameraUp);
        //view = glm::lookAt(world->cameraPos, world->cameraPos + world->cameraFront, world->cameraUp);
        
        glm::vec3 worldpos = glm::vec3(world->getX(),world->getY(),world->getZ());
        glm::mat4 model = glm::mat4(1.0);
        model = glm::translate(model,worldpos);
        
        glm::mat4 lightspace = projection * view;
        
        GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        GLint viewLoc = glGetUniformLocation(shaderProgram, "lightspace");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(lightspace));
    }
    void renderMirror(World* world, GLuint depthMap,float x1, float y1,float x2, float y2)
    {
        GLfloat vertexes[] = {
            //bottom
            x1,y1,0.0f, 0,0, 1.0f,1.0f,1.0f,1.0f, 0.0f,0.0f,1.0f,
            x2,y1,0.0f,  1,0, 1.0f,1.0f,1.0f,1.0f, 0.0f,0.0f,1.0f,
            x2,y2,0.0f,   1,1, 1.0f,1.0f,1.0f,1.0f, 0.0f,0.0f,1.0f,
            //top
            x1,y1,0.0f, 0,0, 1.0f,1.0f,1.0f,1.0f, 0.0f,0.0f,1.0f,
            x1,y2,0.0f,  0,1, 1.0f,1.0f,1.0f,1.0f, 0.0f,0.0f,1.0f,
            x2,y2,0.0f,   1,1, 1.0f,1.0f,1.0f,1.0f, 0.0f,0.0f,1.0f
        };
        
        glUseProgram(mirrorShaderProgram);
        glGenVertexArrays(1, &gVAO);
        glBindVertexArray(gVAO);
        // make and bind the VBO
        glGenBuffers(1, &gVBO);
        glBindBuffer(GL_ARRAY_BUFFER, gVBO);
        // Put the three triangle verticies into the VBO
        
        GLfloat* vpointer = vertexes;
        glBufferData(GL_ARRAY_BUFFER,sizeof(vertexes),vpointer, GL_DYNAMIC_DRAW);
        
        this->assignVertexAttributes();
        
        // unbind the VBO and VAO
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        
        glDisable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        //glDepthFunc(GL_LESS);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBindVertexArray(gVAO);
        glBindBuffer(GL_ARRAY_BUFFER, gVBO);
        
        glBindTexture(GL_TEXTURE_2D,depthMap);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        // unbind the VAO
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDeleteVertexArrays(1,&gVAO);
        glDeleteBuffers(1,&gVBO);
    }
    
    void loadLights(GLint program,World* world)
    {
        auto  shaderLightUniform = glGetUniformLocation(program, "lightPos");
        //Setting light position
        auto source = world->getLightSource();
        float light[3] = {0.0f,0.0f,0.0f};
        if(source != nullptr)
        {
            light[0] = source->getX();
            light[1] = source->getY();
            light[2] = source->getZ();
        }
        glUniform3fv(shaderLightUniform,1,light);
        
        //Setting view position
        auto shaderViewUniform = glGetUniformLocation(program, "viewPos");
        float camera[3] = {world->cameraPos.x,world->cameraPos.y,world->cameraPos.z};
        glUniform3fv(shaderViewUniform,1,camera);
    }
    void loadObjects(World* world)
    {
        
        //    GLfloat vertices[] = {
        //        // Positions           // Texture Coords
        //        0.5f,  0.5f, 0.0f,      1.0f, 1.0f, // Top Right
        //        0.5f, -0.5f, 0.0f,    1.0f, 0.0f, // Bottom Right
        //        -0.5f, -0.5f, 0.0f,      0.0f, 0.0f, // Bottom Left
        //        -0.5f,  0.5f, 0.0f,    0.0f, 1.0f  // Top Left
        //    };
        //    GLuint indices[] = {  // Note that we start from 0!
        //        0, 1, 3, // First Triangle
        //        1, 2, 3  // Second Triangle
        //    };
        //   // GLuint VBO, VAO, EBO;
        //    glGenVertexArrays(1, &gVAO);
        //    glGenBuffers(1, &gVBO);
        //    glGenBuffers(1, &gEBO);
        //
        //    glBindVertexArray(gVAO);
        //
        //    glBindBuffer(GL_ARRAY_BUFFER, gVBO);
        //    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        //
        //    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gEBO);
        //    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        //
        //    // Position attribute
        //    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
        //    glEnableVertexAttribArray(0);
        //    // TexCoord attribute
        //    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        //    glEnableVertexAttribArray(1);
        //
        //    glBindVertexArray(0); // Unbind VAO
        
        
        
        
        // make and bind the VAO
        glGenVertexArrays(1, &gVAO);
        glBindVertexArray(gVAO);
        // make and bind the VBO
        glGenBuffers(1, &gVBO);
        glBindBuffer(GL_ARRAY_BUFFER, gVBO);
        // Put the three triangle verticies into the VBO
        
        GLfloat* vertexes  = world->getVertexes();
        long vertexesSize = world->getVertexSize();
        glBufferData(GL_ARRAY_BUFFER,vertexesSize,vertexes, GL_DYNAMIC_DRAW);
        //delete[] vertexes;
        
        assignVertexAttributes();
        
        // unbind the VBO and VAO
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    
    void renderPerTexture(World* world)
    {
        GLfloat* vertexes  = world->getVertexes();
        long vertexNumber = world->getVertexNumber();
        int vertexVarCount = 12;
        //GLfloat* sorted = new GLfloat[vertexNumber*vertexVarCount];
        GLfloat sorted[vertexNumber*vertexVarCount];
        long numbersOfObjects[loadedTextures.size()];
        GLuint sortedTextures[loadedTextures.size()];
        int textureIndex = 0;
        long sortedIndex = 0;
        auto size = world->getNumberOfPrimitives();
        for(const auto &[key,value] : loadedTextures)
        {
            int numberOfObjects = 0;
            for(int j =0; j < size; j++)
            {
               if(textures[j] == value)
               {
                   //std::cout << "Prim:" << j << " Tex:" << key.c_str() << std::endl;
                   for(int k =0; k < 3; k++)
                   {
                       ++numberOfObjects;
                       for(int z = 0; z < vertexVarCount; z++)
                       {
                           int index = (j*vertexVarCount*3)+(k*vertexVarCount)+z;
                           sorted[sortedIndex++] = vertexes[index];
                          //std::cout << index << ": "<< vertexes[index] << std::endl;
                       }
                   }
               }
            }
            numbersOfObjects[textureIndex] = numberOfObjects;
            sortedTextures[textureIndex++] = value;
        }
        
        //binding data
        // make and bind the VAO
        glGenVertexArrays(1, &gVAO);
        glBindVertexArray(gVAO);
        // make and bind the VBO
        glGenBuffers(1, &gVBO);
        glBindBuffer(GL_ARRAY_BUFFER, gVBO);
        // Put the three triangle verticies into the VBO
        
        long vertexesSize = world->getVertexSize();
        GLfloat* vpointer = sorted;
        glBufferData(GL_ARRAY_BUFFER,vertexesSize,vpointer, GL_DYNAMIC_DRAW);
        
        this->assignVertexAttributes();
        
        // unbind the VBO and VAO
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        
        //glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        //glDepthFunc(GL_LESS);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBindVertexArray(gVAO);
        
        
        //Render
        int offset = 0;
        for(int i =0; i < loadedTextures.size(); i++)
        {
            glBindTexture(GL_TEXTURE_2D, sortedTextures[i]);
            glDrawArrays(GL_TRIANGLES, offset, numbersOfObjects[i]);
            offset+= numbersOfObjects[i];
        }
        
        // unbind the VAO
        glBindVertexArray(0);
        glDeleteVertexArrays(1,&gVAO);
        glDeleteBuffers(1,&gVBO);
        //delete[] sorted;
    }
    
    void assignVertexAttributes()
    {
        // connect the xyz to the "vert" attribute of the vertex shader
        int vertexSize = 12 * sizeof(GLfloat);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,vertexSize, 0);
        glEnableVertexAttribArray(0);
        //texture coords
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,vertexSize, (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
        //color
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE,vertexSize, (GLvoid*)(5 * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);
        //normal
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE,vertexSize, (GLvoid*)(9 * sizeof(GLfloat)));
        glEnableVertexAttribArray(3);
    }
    void renderAllVertexes(World* world)
    {
        loadObjects(world);
        int size = world->getNumberOfPrimitives();
        glBindTexture(GL_TEXTURE_2D, defaultTexture);
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        //glDepthFunc(GL_LESS);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        glBindVertexArray(gVAO);
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        // draw the VAO
        int wholeSize = size*3;
        glDrawArrays(GL_TRIANGLES,0,wholeSize);
        
        glDisable(GL_CULL_FACE);
        glBindVertexArray(0);
        glDeleteVertexArrays(1,&gVAO);
        glDeleteBuffers(1,&gVBO);
    }
    
    void renderPerVertex(World* world)
    {
        loadObjects(world);
        Object3D** objects = world->getPrimitives();
        int size = world->getNumberOfPrimitives();
//            std::cout << "polygons: " << primitivesSize << std::endl;
    
        // clear everything
//            glClearColor(0.5, 0.5, 0.5, 1); // fill color
    
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        //glDepthFunc(GL_LESS);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
        glBindVertexArray(gVAO);
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
        int offset = 0;
        for(int i =0; i < size; i++)
        {
            glBindTexture(GL_TEXTURE_2D, textures[i]);
            Object3D* obj = objects[i];
            
            // draw the VAO
            int vertexes = obj->getNumberOfPoints();
            // std::cout << "offset: " << offset << " ,vertexes: " << vertexes << std::endl;
            glDrawArrays(GL_TRIANGLES, offset,vertexes);
            offset+= vertexes;
        }
    
        // unbind the VAO
        glBindVertexArray(0);
        glDeleteVertexArrays(1,&gVAO);
        glDeleteBuffers(1,&gVBO);
        // swap the display buffers (displays what was just drawn)
        //    glfwSwapBuffers(windowWrapper->getWindow());
        delete[] objects;
    }
    
    void loadTextures(World* world)
    {
        auto size = world->getNumberOfPrimitives();
        auto objects = world->getPrimitives();
        for(int i = 0; i < size; i++)
        {
            auto object = objects[i];
            if(object->getTextureName().empty())
            {
                textures[i] = defaultTexture;
                continue;
            }
            auto iterator = loadedTextures.find(object->getTextureName());
            bool notLoaded = iterator == loadedTextures.end();
            GLuint texture;
            if(notLoaded)
            {
                texture = loader->loadWithDevil(object->getTextureName().c_str());
                std::pair<std::string,GLuint> pair(object->getTextureName(),texture);
                loadedTextures.insert(pair);
            }
            else {
                texture = loadedTextures.at(object->getTextureName());
            }
            textures[i] = texture;
        }
        delete[] objects;
    }
    
    void loadMatrixes(GLint shaderProgram,World* world,int windowWidth, int windowHeight)
    {
        //loading transformations
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(world->getFov()), (float)windowWidth / windowHeight, world->getNearPane(), world->getFarPane());
        //        projection = glm::mat4(1.0);
        
        glm::vec3 worldpos = glm::vec3(world->getX(),world->getY(),world->getZ());
        glm::mat4 model = glm::mat4(1.0);
        model = glm::translate(model,worldpos);
        //model = glm::rotate(model, glm::radians(world->getRotationX()), glm::vec3(1.0, 0.0, 0.0));
        //model = glm::rotate(model, glm::radians(world->getRotationY()), glm::vec3(0.0, 1.0, 0.0));
        
        
        glm::mat4 view;
        view = glm::lookAt(world->cameraPos, world->cameraPos + world->cameraFront, world->cameraUp);
        //        view = glm::mat4(1.0);
        // Такими бы трансформации перспективы выглядят на видюхе. Через координату W
        //    glm::mat4 mvp =  projection * view * model;
        //    for(int i =0; i < vertexNumber; i+=3)
        //    {
        //        int x = i;
        //        int y = i+1;
        //        int z = i+2;
        //        glm::vec4 pos = glm::vec4(vertexes[x],vertexes[y],vertexes[z],1.0);
        //        glm::vec4 newpos = projection * view * model * pos;
        //        vertexes[x] = newpos.x/newpos.w;
        //        vertexes[y] = newpos.y/newpos.w;
        //        vertexes[z] = newpos.z/newpos.w;
        //    }
        
        GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        GLint projLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
    }
};
#endif /* WorldPrinter_hpp */
