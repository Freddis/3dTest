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
    GLint shadowsShaderProgram;
    TextureLoader* loader;
    GLuint defaultTexture;
    GLuint shaderColorUniform;
    GLuint shaderLightUniform;
    GLuint shaderViewUniform;
    GLuint textures[1000000] = {};
    std::map<std::string,GLuint> loadedTextures;
    
public:
    WorldPrinter()
    {
       
        shadowsShaderProgram = loadShadowShaders();
        shaderProgram = loadShaders();
        loader = new TextureLoader();
        //somehow png is not blending correctly
        std::string defaultTexName = "textures/white.jpg";
        defaultTexture = loader->loadWithDevil(defaultTexName.c_str());
        std::pair<std::string,GLuint> pair(defaultTexName,defaultTexture);
        loadedTextures.insert(pair);
    }
    
    void draw(World* world,int windowWidth, int windowHeight)
    {
        renderShadows(world,windowWidth,windowHeight);
//        glUseProgram(shaderProgram);
//        loadMatrixes(world,windowWidth,windowHeight);
//        loadTextures(world);
        
        //renderAllVertexes(world);
        //renderPerVertex(world);
        //renderPerTexture(world);
        
    }
protected:
    void renderShadows(World* world,int windowWidth, int windowHeight)
    {
        glUseProgram(shadowsShaderProgram);
        
        unsigned int depthMapFBO;
        glGenFramebuffers(1, &depthMapFBO);
        const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
        
        unsigned int depthMap;
        glGenTextures(1, &depthMap);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                     SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        
        loadMatrixes(world,windowWidth,windowHeight);
        loadTextures(world);
        renderAllVertexes(world);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        
        glUseProgram(shaderProgram);
        renderAllVertexes(world);
        
    }
    
    void loadObjects(World* world)
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
        
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        //glDepthFunc(GL_LESS);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBindVertexArray(gVAO);
        
        
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
        float camera[3] = {world->cameraPos.x,world->cameraPos.y,world->cameraPos.z};
        glUniform3fv(shaderViewUniform,1,camera);
        
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
    
    GLint loadShadowShaders()
    {
        const char* vertexShaderSource =
        "#version 330\n"
        "layout (location = 0) in vec3 position;"
        "layout (location = 1) in vec2 textCoord;"
        "layout (location = 2) in vec4 color;"
        "layout (location = 3) in vec3 normal;"
        "uniform mat4 model;"
        "uniform mat4 view;"
        "uniform mat4 projection;"
        "out vec2 TexCoord;"
        "out vec4 mycolor;"
        "out vec3 FragPos;"
        "out vec3 Normal;"
        "void main() { "
        "gl_Position = projection * view * model * vec4(position, 1.0f);"
        "FragPos = vec3(model * vec4(position, 1.0f));"
        "TexCoord = textCoord;"
        "mycolor = color;"
        "Normal = normal;"
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
        "in vec4 mycolor;"
        "in vec3 FragPos;"
        "in vec3 Normal;"
        "void main()"
        "{"
        //"color =  vec4(1.0f,1.0f,1.0f,1.0f);"
         "color = mycolor;"
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
        
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return shaderProgram;
    }
    GLint loadShaders()
    {
        const char* vertexShaderSource =
        "#version 330\n"
        "layout (location = 0) in vec3 position;"
        "layout (location = 1) in vec2 textCoord;"
        "layout (location = 2) in vec4 color;"
        "layout (location = 3) in vec3 normal;"
        "uniform mat4 model;"
        "uniform mat4 view;"
        "uniform mat4 projection;"
        "out vec2 TexCoord;"
        "out vec4 mycolor;"
        "out vec3 FragPos;"
        "out vec3 Normal;"
        "void main() { "
        "gl_Position = projection * view * model * vec4(position, 1.0f);"
        "FragPos = vec3(model * vec4(position, 1.0f));"
        "TexCoord = textCoord;"
        "mycolor = color;"
        "Normal = normal;"
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
        "in vec4 mycolor;"
        "in vec3 FragPos;"
        "in vec3 Normal;"
        "uniform sampler2D ourTexture;"
        "uniform vec3 lightPos;"
        "uniform vec3 viewPos;"
        "void main()"
        "{"
        //"color =  vec4(1.0f,1.0f,1.0f,1.0f);"
       // "color = mycolor;"
        "vec4 objectColor = texture(ourTexture, TexCoord*1) * mycolor;"
        
        "vec3 lightColor =  vec3(1.0f,1.0f,1.0f);"
        "float ambientStrength = 0.001f;"
        "vec3 ambient = ambientStrength * lightColor;"
        
        "vec3 norm = Normal;"
        "vec3 lightDir = normalize(lightPos - FragPos);"
        "float diff = max(dot(norm, lightDir), 0.0);"
        "vec3 diffuse = diff * lightColor;"
        
        
        "float specularStrength = 10.0f;"
        "vec3 viewDir = normalize(viewPos - FragPos);"
        "vec3 reflectDir = reflect(-lightDir, norm);"
        "float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);"
        "vec3 specular = specularStrength * spec * lightColor;"
        
        "vec4 result =  objectColor * vec4(ambient+diffuse+specular,1.0f);"
        //"vec4 objectColor = texture(ourTexture, TexCoord*1) * mycolor;"
        //"vec4 result = ambient * objectColor;
        "color = result; "
   //     "color = texture(ourTexture, TexCoord*1) *vec4(1.0f,1.0f,1.0f,1.0f);"
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
        shaderLightUniform = glGetUniformLocation(shaderProgram, "lightPos");
        if(shaderLightUniform == -1) {
            std::cout << "could not bind uniform for color" << std::endl;
            //return 0;
        }
        shaderViewUniform = glGetUniformLocation(shaderProgram, "viewPos");
        if(shaderViewUniform == -1) {
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
    void loadMatrixes(World* world,int windowWidth, int windowHeight)
    {
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
