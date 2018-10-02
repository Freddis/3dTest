//
//  TypeWriter.hpp
//  test
//
//  Created by Сарычев Алексей on 02.10.2018.
//  Copyright © 2018 Home-studio. All rights reserved.
//

#ifndef TypeWriter_hpp
#define TypeWriter_hpp

#include <stdio.h>
#include <iostream>
//#include "GL/glew.h"
//#include "GLFW/glfw3.h"
#include <OpenGL/gl3.h>

#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>

class TypeWriter
{
    GLuint gVAO = 0;
    GLuint gVBO = 0;
    FT_Library ft;
    FT_Face face;
    GLint shaders;
public:
    TypeWriter(const char* font, int size)
    {
       // FT_Library ft;
        if(FT_Init_FreeType(&ft)) {
            fprintf(stderr, "Could not init freetype library\n");
            //return 1;
        }
        else {
          //  std::cout << "Freetype loaded" << std::endl;
        }
        //FT_Face face;
        if(FT_New_Face(ft, font, 0, &face)) {
            fprintf(stderr, "Could not open font\n");
            //return 1;
        }
        FT_Set_Pixel_Sizes(face, 0, size);
        
        
        //loading shaders
        shaders = this->createShaders();
        
    }
    void print(const char *text, float x, float y, float sx, float sy) {
        GLuint tex;
        glActiveTexture(GL_TEXTURE0);
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        //        glUniform1i(uniform_tex, 0);
        glUseProgram(shaders);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //
        
        // make and bind the VAO
        glGenVertexArrays(1, &gVAO);
        glBindVertexArray(gVAO);
        // make and bind the VBO
        glGenBuffers(1, &gVBO);
        glBindBuffer(GL_ARRAY_BUFFER, gVBO);
        // Put the three triangle verticies into the VBO
        
        //        GLfloat vertexes[16] = {
        //                        -0.8,-0.8 , 0, 0,
        //                        -0.8,0.8 , 1, 0,
        //                        0.8,-0.8, 0, 1,
        //                        0.8,0.8, 1, 1,
        //                    };
        //        int vertexSize = sizeof(vertexes);
        //        glBufferData(GL_ARRAY_BUFFER,vertexSize,vertexes, GL_STATIC_DRAW);
        //delete[] vertexes;
        // connect the xyz to the "vert" attribute of the vertex shader
        
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE,4 * sizeof(GLfloat), 0);
        glEnableVertexAttribArray(0);
        //        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,4 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        //        glEnableVertexAttribArray(1);
        // unbind the VBO and VAO
        
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        //        glClearColor(0.5, 0.5, 0.5, 0.5);
        //        glClear(GL_COLOR_BUFFER_BIT);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        
        GLint fontColor = glGetUniformLocation(shaders, "fontcolor");
        GLfloat color[4] = {1, 1, 1, 1};
        glUniform4fv(fontColor, 1, color);
//        float sx = 2.0 / windowWrapper->getWidth();
//        float sy = 2.0 / windowWrapper->getHeight();
//        // glDrawArrays(GL_TRIANGLES, 0, 3);
//
//
//        std::string str = "FPS: " + std::to_string(fps);
//        const char* text =  str.c_str();
        printText(text, -1 + 8 * sx,   1 - 50 * sy,    sx, sy);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glDeleteVertexArrays(1, &gVAO);
    }
    
protected:
    void printText(const char *text, float x, float y, float sx, float sy) {
        const char *p;
        FT_GlyphSlot g = face->glyph;
        for(p = text; *p; p++) {
            if(FT_Load_Char(face, *p, FT_LOAD_RENDER))
                continue;
            
            glTexImage2D(
                         GL_TEXTURE_2D,
                         0,
                         GL_RED,
                         g->bitmap.width,
                         g->bitmap.rows,
                         0,
                         GL_RED,
                         GL_UNSIGNED_BYTE,
                         g->bitmap.buffer
                         );
            
            float x2 = x + g->bitmap_left * sx;
            float y2 = -y - g->bitmap_top * sy;
            float w = g->bitmap.width * sx;
            float h = g->bitmap.rows * sy;
            
            GLfloat box[4][4] = {
                {x2,     -y2    , 0, 0},
                {x2 + w, -y2    , 1, 0},
                {x2,     -y2 - h, 0, 1},
                {x2 + w, -y2 - h, 1, 1},
            };
            
            //        GLfloat box[4][4] = {
            //            {-0.8,-0.8 , 0, 0},
            //            {-0.8,0.8 , 1, 0},
            //            {0.8,-0.8, 0, 1},
            //            {0.8,0.8, 1, 1},
            //        };
            
            glBufferData(GL_ARRAY_BUFFER, sizeof box, box, GL_DYNAMIC_DRAW);
            
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            
            x += (g->advance.x/64) * sx;
            y += (g->advance.y/64) * sy;
        }
    }
    
    GLint createShaders()
    {
        const char* vertexShaderSource =
        "#version 330\n"
        "layout (location = 0) in vec4 coord;"
        "out vec2 texcoord;"
        "void main(void) {"
        "gl_Position = vec4(coord.x,coord.y, 0, 1);"
        "texcoord = vec2(coord.z,coord.w);"
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
        "#version 330\n"
        "in vec2 texcoord;"
        "uniform sampler2D tex;"
        "uniform vec4 fontcolor;"
        "out vec4 color;"
        "void main(void) {"
        //        "gl_FragColor = vec4(1, 1, 1, texture2D(tex, texcoord).r) * color;"
        //        "color = texture(tex, texcoord); "
        //        "color = vec4(1.0f,1.0f,1.0f,1.0f);"
        //        "color = vec4(1, 1, 1, texture(tex, texcoord).r) * vec4(1.0f,1.0f,1.0f,1.0f);"
        "color = vec4(1, 1, 1, texture(tex, texcoord).r) * fontcolor;"
        
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
        //    const char* unif_name = "mycolor";
        //    Unif_color = glGetUniformLocation(shaderProgram, unif_name);
        //    if(Unif_color == -1) { std::cout << "could not bind uniform " << unif_name << std::endl; return 0; }
        
        glUseProgram(shaderProgram);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return shaderProgram;
    }
};
#endif /* TypeWriter_hpp */
