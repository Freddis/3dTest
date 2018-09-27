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
#include <stdlib.h>
#include <unistd.h>

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <OpenGL/gl3.h>

#include "Triangle.hpp"
#include "Square.hpp"
#include "Cube.hpp"
#include "World.hpp"
#include "Controls.cpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

GLuint gVAO = 0;
GLuint gVBO = 0;
GLuint programId;
GLFWwindow* window = NULL;
GLFWmonitor* monitor = NULL;
Controls* controls;

// ! ID юниформ переменной цвета
GLint Unif_color;

//const glm::vec2 SCREEN_SIZE(800, 600);



static void loadObjects(Object3D** objects, int size, World* world) {
    
    // make and bind the VAO
    glGenVertexArrays(1, &gVAO);
    glBindVertexArray(gVAO);
    // make and bind the VBO
    glGenBuffers(1, &gVBO);
    glBindBuffer(GL_ARRAY_BUFFER, gVBO);
    // Put the three triangle verticies into the VBO
    

   
    int vertexNumber = 0;
    int vertexSize = 0;
    for(int i =0; i < size; i++)
    {
        Object3D* obj = objects[i];
        vertexSize += obj->getSizeOf();
        vertexNumber += obj->getSizeOf()/sizeof(GLfloat);
    }
    GLfloat vertexes[vertexNumber];
    int z = 0;
    for(int i =0; i < size; i++)
    {
        Object3D* obj = objects[i];
        int vertexCount = obj->getSizeOf()/sizeof(GLfloat);
        GLfloat* vertexArr = obj->getWorldVertexes();
      
        int k =0;
        for(int j =0;j < vertexCount; j++)
        {
            if(k == 0)
            {
                vertexArr[j] += world->getX();
            }
            else if(k == 1)
            {
                vertexArr[j] += world->getY();
            }
            else if(k == 2)
            {
                vertexArr[j] += world->getZ();
            }
            else {
                vertexArr[j] += world->getX();
                k = 0;
            }
            k++;
            vertexes[z++] = vertexArr[j];
        }
    }
    
    int screenWidth = 800;
    int screenHeight = 600;
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(world->getFov()), (float)screenWidth / screenHeight, world->getNearPane(), world->getFarPane());
    glm::vec3 worldpos = glm::vec3(world->getX(),world->getY(),world->getZ());
    glm::mat4 trans = glm::mat4(1.0);
   // trans = glm::translate(trans,worldpos);
    trans = glm::rotate(trans, glm::radians(world->getRotationX()), glm::vec3(1.0, 0.0, 0.0));
    //trans = glm::translate(trans,worldpos);
    glm::mat4 mvp =  trans * projection;
    
    for(int i =0; i < vertexNumber; i+=3)
    {
        int x = i;
        int y = i+1;
        int z = i+2;
        glm::vec4 pos = glm::vec4(vertexes[x],vertexes[y],vertexes[z],1.0);
        glm::vec4 newpos = pos * mvp;
        vertexes[x] = newpos.x/newpos.w;
        vertexes[y] = newpos.y/newpos.w;
        vertexes[z] = newpos.z/newpos.w;
    }
    
    //GLfloat* vertexes  = t->getVertexArray();
    
    glBufferData(GL_ARRAY_BUFFER,vertexSize,vertexes, GL_STREAM_DRAW);
    // connect the xyz to the "vert" attribute of the vertex shader
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    // unbind the VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glBindVertexArray(0);
    
    const char* vertexShaderSource =
    "#version 330\n"
    "layout (location = 0) in vec3 position;"
    "void main() { "
    "gl_Position = vec4(position.x, position.y, position.z, 1.0);}";
    
    
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
    "uniform vec4 mycolor;"
    "out vec4 color;"
    "void main()"
    "{"
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
    }
    const char* unif_name = "mycolor";
    Unif_color = glGetUniformLocation(shaderProgram, unif_name);
    if(Unif_color == -1) { std::cout << "could not bind uniform " << unif_name << std::endl; return; }
    
    glUseProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
}



// draws a single frame

static void Render(Object3D** objects, int size) {
    
    // clear everything
    glClearColor(0.5, 0.5, 0.5, 1); // fill color
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    //glEnable(GL_CULL_FACE);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    
    glBindVertexArray(gVAO);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    int offset = 0;
    for(int i =0; i < size; i++)
    {
        
        Object3D* obj = objects[i];
        Color* color = obj->getColor();
        //color->g+=0.001;
        float converted[4] = {color->r,color->g,color->b,1.0f};
        float* myColor  = converted;
        
        // std::cout << "r:" << myColor[0] << ", g:" << myColor[1] << ", b:" << myColor[2] << std::endl;
        // ! Передаем юниформ в шейдер
        glUniform4fv(Unif_color, 1,myColor);
        
        // draw the VAO
        int vertexes = obj->getSizeOf()/sizeof(GLfloat)/3;
        // std::cout << "offset: " << offset << " ,vertexes: " << vertexes << std::endl;
        glDrawArrays(GL_TRIANGLES, offset,offset+vertexes);
        offset+= vertexes;
    }
    //glUniform4fv(Unif_color, 1,color2);
    //glColor3b(0, 250, 0);
    //glDrawArrays(GL_TRIANGLES, 3, 6);
    // unbind the VAO
    glBindVertexArray(0);
    // swap the display buffers (displays what was just drawn)
    glfwSwapBuffers(window);
    
}


void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void processKeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    controls->processKeyCallBack(window, key, scancode, action, mods);
}

int main(int argc, char * argv[]) {
    
    // GLFWwindow* window;
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);
    
    glfwWindowHint(GLFW_SAMPLES, 16); // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    //glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE );
    window = glfwCreateWindow(800, 600, "Simple example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    //glfwSetKeyCallback(window, key_callback);
    
    
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
    world->moveZ(-0.9);
    
    
    //Square* square = new Square(0.02);
    Cube* cube = new Cube(0.05);
    cube->moveZ(0.01);
    world->addObject(cube);
    Cube* cube2 = new Cube(0.05);
    cube->moveX(-0.07);
    world->addObject(cube2);
    
    Square* floor = new Square(0.09);
    floor->setColor(Color::getWhite());
    floor->moveX(0.0);
    floor->moveY(-0.001);
    floor->getC()->z = floor->getSideSize();
    floor->getB()->z = floor->getSideSize();
    floor->getC()->y = floor->getA()->x;
    floor->getB()->y = floor->getA()->x;
    world->addObject(floor);
//    Cube* cube2 = new Cube(square2);
//    cube2->moveX(0.7);
//    cube2->moveY(0.7);
//    cube2->rotateY(45);
//    world->addObject(cube2);
//
//    Cube* cube3 = new Cube(0.2);
//    cube3->moveX(0.3);
//    cube3->moveY(0.5);
//    cube3->moveZ(0.2);
//    cube3->rotateY(45);
//    cube3->rotateZ(45);
//    world->addObject(cube3);
//
//    Cube* cube4 = new Cube(0.2);
//    cube4->moveX(-0.7);
//    cube4->moveY(-0.7);
//    cube4->moveZ(-0.5);
//    cube4->rotateY(45);
//    world->addObject(cube4);
//    Cube* cube5 = new Cube(0.2);
//    cube5->moveX(-0.5);
//    cube5->moveY(-0.7);
//    cube5->moveZ(0.5);
//    cube5->rotateY(45);
//    world->addObject(cube5);
    
    controls = new Controls(world);
    glfwSetKeyCallback(window,processKeyCallBack);
    
    Object3D** primitives = world->getPrimitives();
    while (!glfwWindowShouldClose(window))
    {
       // std::cout << world->getZ() << " :: " << cube->getZ()  << std::endl;
        cube->rotateZ(1);
        cube->rotateY(1);
       // cube2->moveX(-0.001);
       // cube2->rotateX(2);
        //Point* center = cube->getCenter();
        
        
//        for(int i = 0; i < cube->getNumberOfPoints(); i++)
//        {
//            glm::vec3 worldPos = glm::vec3(world->getX(),world->getY(),world->getZ());
//            Point* a = points[i];
////            a->x -= worldPos.x;
////            a->x -= worldPos.y;
////            a->z -= worldPos.z;
//            glm::mat4 trans = glm::mat4(1.0);
//            glm::vec4 pos = glm::vec4(a->x,a->y,a->z,1.0);
//
//
//           // glm::vec3 worldPos = glm::vec3(2.0f,1.0f,1.0f);
//           // trans = glm::translate(trans,glm::vec3(0.1,0,0));
//            trans = glm::rotate(trans, glm::radians(1.0f), glm::vec3(1.0, 0.0, 0.0));
//           // a->rotateX(1);
//            //trans = glm::translate(trans,worldPos);
//            glm::mat4 mvp = trans;// * proj;
//            glm::vec4 newpos = pos * mvp;
//            a->x = newpos.x;
//            a->y = newpos.y;
//            a->z = newpos.z;
////
////            a->x += worldPos.x;
////            a->x += worldPos.y;
////            a->z += worldPos.z;
//        }
        
        controls->process();
        loadObjects(primitives,world->getNumberOfPrimitives(),world);
        Render(primitives,world->getNumberOfPrimitives());
        glfwWaitEventsTimeout(1/30);
        //glfwPostEmptyEvent();
        //glfwPollEvents();
        //glfwWaitEvents();
    }
    
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
    
}
