//
//  ShaderLoader.hpp
//  test
//
//  Created by Freddis on 10.10.2018.
//  Copyright © 2018 Home-studio. All rights reserved.
//

#ifndef ShaderLoader_hpp
#define ShaderLoader_hpp

#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <streambuf>

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <OpenGL/gl3.h>

class ShaderLoader
{
    public:
    GLuint load(const char* path)
    {
        std::string vertexFile = std::string(path) + ".vert";
    
        std::ifstream file(vertexFile.c_str());
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string content = buffer.str();
        const char* vertexShaderSource = content.c_str();
        
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
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"<< vertexFile << "\n" << infoLog << std::endl;
        }
        
        std::string fragmentFile = std::string(path) + ".frag";
        std::ifstream file2(fragmentFile.c_str());
        std::stringstream buffer2;
        buffer2 << file2.rdbuf();
        std::string content2 = buffer2.str();
        const char* fragmentShaderSource = content2.c_str();
        
        GLuint fragmentShader;
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);
        
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        
        if(!success)
        {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << fragmentFile << "\n" << infoLog << std::endl;
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
    
    GLuint getUniform(const char* name)
    {
        return 0;
    }
    
};
#endif /* ShaderLoader_hpp */
