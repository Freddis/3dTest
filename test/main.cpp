//
//  main.cpp
//  test
//
//  Created by Freddis on 17.01.17.
//  Copyright © 2017 Home-studio. All rights reserved.
//

#include <iostream>
#include <stdlib.h>
#include <string>
#include <unistd.h>

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <OpenGL/gl3.h>

#include "TextureLoader.hpp"
#include "Triangle.hpp"
#include "Square.hpp"
#include "Cube.hpp"
#include "World.hpp"
#include "Stage1.hpp"
#include "Controls.cpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <IL/il.h>
#include <IL/ilu.h>
using namespace std;

GLuint gVAO = 0;
GLuint gVBO = 0;
GLuint gEBO = 0;
GLuint programId;
GLFWwindow* window = NULL;
GLFWmonitor* monitor = NULL;
Controls* controls;
int screenWidth = 800;
int screenHeight = 600;

// ! ID юниформ переменной цвета
GLint Unif_color;

GLuint LoadTexture( const char * filename, int width, int height);




static void loadObjects(World* world, GLuint shaderProgram) {
    
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
    int vertexSize = world->getVertexNumber()*sizeof(GLfloat)*5;
    glBufferData(GL_ARRAY_BUFFER,vertexSize,vertexes, GL_STATIC_DRAW);
    delete[] vertexes;
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
    projection = glm::perspective(glm::radians(world->getFov()), (float)screenWidth / screenHeight, world->getNearPane(), world->getFarPane());
    //projection = glm::mat4(1.0);
    
    glm::vec3 worldpos = glm::vec3(world->getX(),world->getY(),world->getZ());
    glm::mat4 model = glm::mat4(1.0);
    model = glm::translate(model,worldpos);
    //model = glm::rotate(model, glm::radians(world->getRotationX()), glm::vec3(1.0, 0.0, 0.0));
    //model = glm::rotate(model, glm::radians(world->getRotationY()), glm::vec3(0.0, 1.0, 0.0));
    
    
    
    glm::mat4 view;
//    view = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.5f),
//                       glm::vec3(0.0f, 0.0f, 0.0f),
//                       glm::vec3(0.0f, 1.0f, 0.0f));
    //view = glm::mat4(1.0);
    
    view = glm::lookAt(world->cameraPos, world->cameraPos + world->cameraFront, world->cameraUp);
    
    
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



// draws a single frame

static void Render(Object3D** objects, int size, GLuint texture) {
    
    // clear everything
    glClearColor(0.5, 0.5, 0.5, 1); // fill color
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    //glDepthFunc(GL_LESS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glEnable(GL_CULL_FACE);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(gVAO);
   // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    int offset = 0;
    for(int i =0; i < size; i++)
    {
        
        Object3D* obj = objects[i];
        Color* color = obj->getColor();
        //color->g+=0.001;
        float converted[4] = {color->r,color->g,color->b,color->a};
//        if(color->a < 1)
//        {
//            glDepthFunc(GL_ALWAYS);
//        }
//        else {
//            glDepthFunc(GL_LEQUAL);
//        }
        float* myColor  = converted;
        
        // std::cout << "r:" << myColor[0] << ", g:" << myColor[1] << ", b:" << myColor[2] << std::endl;
        // ! Передаем юниформ в шейдер
        glUniform4fv(Unif_color, 1,myColor);
        
        // draw the VAO
        int vertexes = obj->getNumberOfPoints();
        // std::cout << "offset: " << offset << " ,vertexes: " << vertexes << std::endl;
        glDrawArrays(GL_TRIANGLES, offset,offset+vertexes);
        offset+= vertexes;
    }
    //glUniform4fv(Unif_color, 1,color2);
    //glColor3b(0, 250, 0);
    //glDrawArrays(GL_TRIANGLES, 0, 10);
   // glBindVertexArray(VAO);
    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    //glDrawArrays(GL_TRIANGLES, 3, 6);
    // unbind the VAO
   // glBindVertexArray(0);
    // swap the display buffers (displays what was just drawn)
    glfwSwapBuffers(window);
    
    
    
    
}

GLuint loadShaders()
{
    //    const char* vertexShaderSource =
    //    "#version 330\n"
    //    "layout (location = 0) in vec3 position;"
    //    "void main() { "
    //    "gl_Position = vec4(position, 1.0f);}";
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
    "void main()"
    "{"
    //"color =  vec4(1.0f,1.0f,1.0f,1.0f);"
    //"color = vec4(mycolor, 1.0f);"
    "color = texture(ourTexture, TexCoord) * mycolor; "
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
    
    // ! Вытягиваем ID юниформ const
    
    
    
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
    const char* unif_name = "mycolor";
    Unif_color = glGetUniformLocation(shaderProgram, unif_name);
    if(Unif_color == -1) { std::cout << "could not bind uniform " << unif_name << std::endl; return 0; }
    
    glUseProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return shaderProgram;
}


void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

int main(int argc, char * argv[]) {
    
    
    // GLFWwindow* window;
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);
    
//    glfwWindowHint(GLFW_SAMPLES, 2); // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    //glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE );

    bool fullscreen = true;
    GLFWmonitor* monitor = NULL;
    if(fullscreen)
    {
        monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode * mode = glfwGetVideoMode(monitor);
        screenWidth = mode->width;
        screenHeight = mode->height;
        
    }
    window = glfwCreateWindow(screenWidth, screenHeight, "Simple example",monitor, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
   
    glfwMakeContextCurrent(window);
    if(fullscreen)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
   
    // initialise GLEW
    glewExperimental = GL_TRUE; //stops glew crashing on OSX :-/
    if(glewInit() != GLEW_OK)
        throw std::runtime_error("glewInit failed");
    
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    
    std::cout << "GLFW: " << glfwGetVersionString() << std::endl;
    
    World* world = new World();
    Stage1* stage = new Stage1(world);
    
    const char* filename = "textures/texture2.jpg";
    TextureLoader* loader = new TextureLoader();
    GLuint texture = loader->loadWithDevil(filename);
    
    controls = new Controls(world,window);
    glfwSetKeyCallback(window,[](GLFWwindow* window, int key, int scancode, int action, int mods){
        controls->processKeyCallBack(window, key, scancode, action, mods);
    });
    
    
    if(fullscreen)
    {
        glfwSetCursorPosCallback(window,[](GLFWwindow* window ,double x,double y){
                controls->processCursorPosition(window, x, y);
        });
    }
    
    world->rotateY(-90);
    controls->updateWorldRotation();
    
    Object3D** primitives = world->getPrimitives();
    int primitivesSize = world->getNumberOfPrimitives();
     std::cout << "polygons: " << primitivesSize << std::endl;
    GLuint shaderProgram = loadShaders();
    double counter = 0;
    double timer =  0;
    double passed = 0;
     glfwSwapInterval(0);
    while (!glfwWindowShouldClose(window))
    {
        double frameStart = glfwGetTime();
        loadObjects(world,shaderProgram);
        Render(primitives,primitivesSize,texture);
        glfwWaitEventsTimeout(1/3000);
        double frameEnd = glfwGetTime();
        passed = frameEnd - frameStart;
        timer += passed;
        ++counter;
        if(timer >= 1)
        {
            std::cout << "fps: " << counter << std::endl;
            timer = 0;
            counter = 0;
        }
        stage->process(passed);
        controls->process(passed);
    }
    
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
    
}

