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

#include "World.hpp"
#include "Object3D.hpp"
#include "TextureLoader.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class WorldPrinter
{
    GLuint gVAO = 0;
    GLuint gVBO = 0;
    GLuint gEBO = 0;
    GLint shaderProgram;
    TextureLoader* loader;
    GLuint defaultTexture;
    GLuint shaderColorUniform;
    GLuint shaderTextureScaleUniform;
    GLuint textures[2000] = {};
    std::map<std::string,GLuint> loadedTextures;
    
public:
    WorldPrinter()
    {
        shaderProgram = loadShaders();
        loader = new TextureLoader();
        defaultTexture = loader->loadWithDevil("textures/white.png");
    }
    
    void draw(World* world,int windowWidth, int windowHeight)
    {
        glUseProgram(shaderProgram);
        loadTextures(world);
        loadObjects(world,windowWidth,windowHeight);
        render(world);
    }
protected:
    void loadObjects(World* world,int windowWidth, int windowHeight)
    {
        
        //    GLfloat vertices[] = {
        //        // Positions          // Colors           // Texture Coords
        //        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // Top Right
        //        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Bottom Right
        //        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // Bottom Left
        //        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // Top Left
        //    };
        //    GLfloat vertices[] = {
        //        // Positions          // Colors           // Texture Coords
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
        //    // Color attribute
        ////    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        ////    glEnableVertexAttribArray(1);
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
        long vertexSize = world->getVertexNumber()*sizeof(GLfloat)*5;
        glBufferData(GL_ARRAY_BUFFER,vertexSize,vertexes, GL_DYNAMIC_DRAW);
        //delete[] vertexes;
        
        // connect the xyz to the "vert" attribute of the vertex shader
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,5 * sizeof(GLfloat), 0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
        // unbind the VBO and VAO
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        
        
        //loading transformations
        //    int screenWidth = 800;
        //    int screenHeight = 600;
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(world->getFov()), (float)windowWidth / windowHeight, world->getNearPane(), world->getFarPane());
//        projection = glm::mat4(1.0);
        
        glm::vec3 worldpos = glm::vec3(world->getX(),world->getY(),world->getZ());
        glm::mat4 model = glm::mat4(1.0);
        model = glm::translate(model,worldpos);
        //model = glm::rotate(model, glm::radians(world->getRotationX()), glm::vec3(1.0, 0.0, 0.0));
        //model = glm::rotate(model, glm::radians(world->getRotationY()), glm::vec3(0.0, 1.0, 0.0));
        
        
        
        glm::mat4 view;
//        world->cameraPos.z += 0.01;
        view = glm::lookAt(world->cameraPos, world->cameraPos + world->cameraFront, world->cameraUp);
//        view = glm::mat4(1.0);
        
        //    glm::mat4 mvp =  projection * view * model;
        //    for(int i =0; i < vertexNumber; i+=3)
        //    {
        //        int x = i;
        //        int y = i+1;
        //        int z = i+2;
        //        glm::vec4 pos = glm::vec4(vertexes[x],vertexes[y],vertexes[z],1.0);
        //        glm::vec4 newpos = projection * view * model * pos;
        //        double xpos = newpos.x/newpos.w;
        //        double ypos = newpos.y/newpos.w;
        //        double zpos = newpos.z/newpos.w;
        ////        vertexes[x] = xpos;
        ////        vertexes[y] = ypos;
        ////        vertexes[z] = zpos;
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
    
    void render(World* world)
    {
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
                Color* color = obj->getColor();
                float converted[4] = {color->r,color->g,color->b,color->a};
                
                //Если есть текстура, то цвет не используем
                if(textures[i] != defaultTexture)
                {
                    converted[0] = 1;
                    converted[1] = 1;
                    converted[2] = 1;
                    converted[3] = 1;
                }
                float* myColor  = converted;
                // std::cout << "r:" << myColor[0] << ", g:" << myColor[1] << ", b:" << myColor[2] << std::endl;
                // ! Передаем юниформ в шейдер
                glUniform4fv(shaderColorUniform, 1,myColor);
                
                float scale = obj->getTextureScale();
                glUniform1f(shaderTextureScaleUniform,scale);
                
                // draw the VAO
                int vertexes = obj->getNumberOfPoints();
                // std::cout << "offset: " << offset << " ,vertexes: " << vertexes << std::endl;
//                glDrawArrays(GL_TRIANGLES, offset,offset+vertexes);
              
                int wholeSize = size*vertexes;
                glDrawArrays(GL_TRIANGLES,0,wholeSize);
                glBindVertexArray(0);
                glDeleteVertexArrays(1,&gVAO);
                glDeleteBuffers(1,&gVBO);
                delete[] objects;
                return;
                
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
    GLint loadShaders()
    {
        const char* vertexShaderSource =
        "#version 330\n"
        "layout (location = 0) in vec3 position;"
        // "layout (location = 1) in vec3 color;"
        "layout (location = 1) in vec2 textCoord;"
        "uniform mat4 model;"
        "uniform mat4 view;"
        "uniform mat4 projection;"
        "out vec2 TexCoord;"
        "void main() { "
        "gl_Position = projection * view * model * vec4(position, 1.0f);"
        "TexCoord = textCoord;"
        "}";
        
        GLuint vertexShader;
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);
        
        GLint success;
        GLchar infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        
        if(!success)
        {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
        
        
        const char* fragmentShaderSource =
        "#version 330 core\n"
        "in vec2 TexCoord;"
        "out vec4 color;"
        "uniform vec4 mycolor;"
        "uniform sampler2D ourTexture;"
        "uniform float scale;"
        "void main()"
        "{"
        //"color =  vec4(1.0f,1.0f,1.0f,1.0f);"
        //"color = vec4(mycolor, 1.0f);"
        "color = texture(ourTexture, TexCoord*scale) * mycolor; "
        "}";
        
        GLuint fragmentShader;
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);
        
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        
        if(!success)
        {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
        
        
        //Combine vertex and fragment shaders
        GLuint shaderProgram;
        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if(!success) {
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAMM::COMPILATION_FAILED\n" << infoLog << std::endl;
            return 0;
        }
        shaderColorUniform = glGetUniformLocation(shaderProgram, "mycolor");
        if(shaderColorUniform == -1) {
            std::cout << "could not bind uniform for color" << std::endl;
            return 0;
        }
        shaderTextureScaleUniform = glGetUniformLocation(shaderProgram, "scale");
        if(shaderTextureScaleUniform == -1) {
            std::cout << "could not bind uniform for texture scale " << std::endl;
            //return 0;
            
        }
        
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return shaderProgram;
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
};
#endif /* WorldPrinter_hpp */
