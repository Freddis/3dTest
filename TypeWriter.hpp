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
#include <sstream>
#include <algorithm>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <OpenGL/gl3.h>

#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>
#define MAXWIDTH 1024

class TypeWriter
{
    GLuint gVAO = 0;
    GLuint gVBO = 0;
    FT_Library ft;
    FT_Face face;
    GLint shaders;
    int currentLine = 1;
    int windowWidth;
    int windowHeight;
    int fontsize;
    int prevLetterCount = 0;
    int letterCount = 0;
    bool useAtlas = true;
    GLuint uniform_tex;
    int atlasWidth;
    int atlasHeight;
    GLuint atlasTexture;
    unsigned int w;            // width of texture in pixels
    unsigned int h;            // height of texture in pixels
    int vertexNumber = 0;
    int sizeOfVertexes = 0;
    struct character_info {
        float ax; // advance.x
        float ay; // advance.y
        
        float bw; // bitmap.width;
        float bh; // bitmap.rows;
        
        float bl; // bitmap_left;
        float bt; // bitmap_top;
        
        float tx; // x offset of glyph in texture coordinates
        float ty;    // y offset of glyph in texture coordinates
    } c[128];
    struct point {
        GLfloat x;
        GLfloat y;
        GLfloat s;
        GLfloat t;
    } coords[2000];
public:
    TypeWriter(const char* font, int size,int windowWidth, int windowHeight)
    {
        this->windowWidth = windowWidth;
        this->windowHeight = windowHeight;
        fontsize = size;
        if(FT_Init_FreeType(&ft)) {
            fprintf(stderr, "Could not init freetype library\n");
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
        if(useAtlas)
        {
            createAtlas();
        }
    }

    int getNumberOfLetters()
    {
        return prevLetterCount;
    }
    
    void printLine(std::string string)
    {
        
        float pixelSizeX = 2.0 / windowWidth;
        float pixelSizeY = 2.0 / windowHeight;
        float xoffset = 10*pixelSizeX;
        float yoffset = 10*pixelSizeY + currentLine*fontsize*pixelSizeY;
        float xcoord = -1 + xoffset;
        float ycoord = 1 - yoffset;
        const char* cstr = string.c_str();
        letterCount += strlen(cstr);
        if(useAtlas)
        {
            addToStorageWithAtlas(cstr,xcoord,ycoord,pixelSizeX,pixelSizeY);
        }
        else
        {
            printEachCharacter(cstr,xcoord,ycoord,pixelSizeX,pixelSizeY);
        }
        currentLine++;
    }
    void show()
    {
        clear();
        if(!useAtlas)
        {
            return;
        }
        glUseProgram(shaders);
        /* Use the texture containing the atlas */
        glBindTexture(GL_TEXTURE_2D, atlasTexture);
        glUniform1i(uniform_tex, 0);
        
        // make and bind the VAO
        glGenVertexArrays(1, &gVAO);
        glBindVertexArray(gVAO);
        // make and bind the VBO
        glGenBuffers(1, &gVBO);
        glBindBuffer(GL_ARRAY_BUFFER, gVBO);
        
        /* Set up the VBO for our vertex data */
        
        // glBindBuffer(GL_ARRAY_BUFFER, gVBO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE,0, 0);
        
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_ALWAYS);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        GLint fontColor = glGetUniformLocation(shaders, "fontcolor");
        GLfloat color[4] = {1, 1, 1, 1};
        glUniform4fv(fontColor, 1, color);
        
        glBufferData(GL_ARRAY_BUFFER, sizeOfVertexes, coords, GL_DYNAMIC_DRAW);
        glDrawArrays(GL_TRIANGLES, 0, vertexNumber);
        
        //cleanup
        glDisableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glDeleteVertexArrays(1, &gVAO);
        glDeleteBuffers(1,&gVBO);
        vertexNumber = 0;
        sizeOfVertexes = 0;
    }
protected:
    void addToStorageWithAtlas(const char *text, float x, float y, float sx, float sy) {
        
//        struct point {
//            GLfloat x;
//            GLfloat y;
//            GLfloat s;
//            GLfloat t;
//        } coords[6 * strlen(text)];
        
        int c = vertexNumber;
        const uint8_t *p;
        /* Loop through all characters */
        for (p = (const uint8_t *)text; *p; p++) {
            /* Calculate the vertex and texture coordinates */
            float x2 = x + this->c[*p].bl * sx;
            float y2 = -y - this->c[*p].bt * sy;
            float w = this->c[*p].bw * sx;
            float h = this->c[*p].bh * sy;
            
            /* Advance the cursor to the start of the next character */
            x += this->c[*p].ax * sx;
            y += this->c[*p].ay * sy;
            
            /* Skip glyphs that have no pixels */
            if (!w || !h)
                continue;
            
            //trngl1
            coords[c++] = (point) {
                x2, -y2 - h, this->c[*p].tx, this->c[*p].ty + this->c[*p].bh / this->h};
            
            coords[c++] = (point) {
                x2 + w, -y2, this->c[*p].tx + this->c[*p].bw / this->w, this->c[*p].ty};
            coords[c++] = (point) {
                x2, -y2, this->c[*p].tx, this->c[*p].ty}; 
       
            
            //trngl2
            coords[c++] = (point) {
                x2 + w, -y2, this->c[*p].tx + this->c[*p].bw / this->w, this->c[*p].ty};
            coords[c++] = (point) {
                x2, -y2 - h, this->c[*p].tx, this->c[*p].ty + this->c[*p].bh / this->h};
            coords[c++] = (point) {
                x2 + w, -y2 - h, this->c[*p].tx + this->c[*p].bw / this->w, this->c[*p].ty + this->c[*p].bh / this->h};
        }
        sizeOfVertexes = c*4*sizeof(GLfloat);
        vertexNumber = c;
    }
    void printLineWithAtlas(const char *text, float x, float y, float sx, float sy) {
        
        glUseProgram(shaders);
        /* Use the texture containing the atlas */
        glBindTexture(GL_TEXTURE_2D, atlasTexture);
        glUniform1i(uniform_tex, 0);
        
        // make and bind the VAO
        glGenVertexArrays(1, &gVAO);
        glBindVertexArray(gVAO);
        // make and bind the VBO
        glGenBuffers(1, &gVBO);
        glBindBuffer(GL_ARRAY_BUFFER, gVBO);
        
        /* Set up the VBO for our vertex data */
      
       // glBindBuffer(GL_ARRAY_BUFFER, gVBO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE,0, 0);
        
        
        struct point {
            GLfloat x;
            GLfloat y;
            GLfloat s;
            GLfloat t;
        } coords[6 * strlen(text)];
        
        int c = 0;
        const uint8_t *p;
        /* Loop through all characters */
        for (p = (const uint8_t *)text; *p; p++) {
            /* Calculate the vertex and texture coordinates */
            float x2 = x + this->c[*p].bl * sx;
            float y2 = -y - this->c[*p].bt * sy;
            float w = this->c[*p].bw * sx;
            float h = this->c[*p].bh * sy;
            
            /* Advance the cursor to the start of the next character */
            x += this->c[*p].ax * sx;
            y += this->c[*p].ay * sy;
            
            /* Skip glyphs that have no pixels */
            if (!w || !h)
                continue;
            
            coords[c++] = (point) {
                x2, -y2, this->c[*p].tx, this->c[*p].ty};
            coords[c++] = (point) {
                x2 + w, -y2, this->c[*p].tx + this->c[*p].bw / this->w, this->c[*p].ty};
            coords[c++] = (point) {
                x2, -y2 - h, this->c[*p].tx, this->c[*p].ty + this->c[*p].bh / this->h};
            coords[c++] = (point) {
                x2 + w, -y2, this->c[*p].tx + this->c[*p].bw / this->w, this->c[*p].ty};
            coords[c++] = (point) {
                x2, -y2 - h, this->c[*p].tx, this->c[*p].ty + this->c[*p].bh / this->h};
            coords[c++] = (point) {
                x2 + w, -y2 - h, this->c[*p].tx + this->c[*p].bw / this->w, this->c[*p].ty + this->c[*p].bh / this->h};
        }
        
//        coords[0] =(point) {0.9,0.9,1,1};
//        coords[1] =(point) {0.9,-0.9,1,0};
//        coords[2] =(point) {-0.9,-0.9,0,0};
//        coords[3] =(point) {-0.9,0.9,0,1};
//        coords[4] =(point) {-0.9,-0.9,0,0};
//        coords[5] =(point) {0.9,0.9,1,1};
        
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_ALWAYS);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        GLint fontColor = glGetUniformLocation(shaders, "fontcolor");
        GLfloat color[4] = {1, 1, 1, 1};
        glUniform4fv(fontColor, 1, color);
        
        glBufferData(GL_ARRAY_BUFFER, sizeof coords, coords, GL_DYNAMIC_DRAW);
        glDrawArrays(GL_TRIANGLES, 0, c);
        
        //cleanup
        glDisableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glDeleteVertexArrays(1, &gVAO);
        glDeleteBuffers(1,&gVBO);
    }
    void printEachCharacter(const char *text, float x, float y, float sx, float sy) {
//        return;
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
        
        
        // make and bind the VAO
        glGenVertexArrays(1, &gVAO);
        glBindVertexArray(gVAO);
        // make and bind the VBO
        glGenBuffers(1, &gVBO);
        glBindBuffer(GL_ARRAY_BUFFER, gVBO);
        // connect the xyz to the "vert" attribute of the vertex shader
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE,4 * sizeof(GLfloat), 0);
        glEnableVertexAttribArray(0);

        //inmortant line for Freetype
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_ALWAYS);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        GLint fontColor = glGetUniformLocation(shaders, "fontcolor");
        GLfloat color[4] = {1, 1, 1, 1};
        glUniform4fv(fontColor, 1, color);
        printText(text,x,y,sx,sy);

        //cleanup
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glDeleteVertexArrays(1, &gVAO);
        glDeleteBuffers(1,&gVBO);
        glDeleteTextures(1, &tex);
    }
    
    void clear()
    {
        currentLine = 1;
        prevLetterCount = letterCount;
        letterCount = 0;
    }
    void printText(const char *text, float x, float y, float sx, float sy) {
        const char *p;
        letterCount += strlen(text);
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
            
            glBufferData(GL_ARRAY_BUFFER, sizeof box, box, GL_STATIC_DRAW);

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
//            "gl_FragColor = vec4(1, 1, 1, texture2D(tex, texcoord).r) * color;"
//            "color = vec4(1.0f,1.0f,1.0f,1.0f);"
//            "color = texture(tex, texcoord); "
//            "color = vec4(1, 1, 1, texture(tex, texcoord).r) * vec4(1.0f,1.0f,1.0f,1.0f);"
            "color = vec4(1, 1, 1, texture(tex, texcoord).r) * fontcolor;"
           // gl_FragColor = vec4(1, 1, 1, texture2D(tex, texpos).a) * color;
        
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
        uniform_tex = glGetUniformLocation(shaderProgram, "tex");
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return shaderProgram;
    }
    void createAtlas()
    {
        FT_GlyphSlot g = face->glyph;
        unsigned int roww = 0;
        unsigned int rowh = 0;
        w = 0;
        h = 0;
        memset(c, 0, sizeof c);
        
        /* Find minimum size for a texture holding all visible ASCII characters */
        for (int i = 32; i < 128; i++) {
            if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
                fprintf(stderr, "Loading character %c failed!\n", i);
                continue;
            }
            if (roww + g->bitmap.width + 1 >= MAXWIDTH) {
                w = std::max(w, roww);
                h += rowh;
                roww = 0;
                rowh = 0;
            }
            roww += g->bitmap.width + 1;
            rowh = std::max(rowh, g->bitmap.rows);
        }
        
        w = std::max(w, roww);
        h += rowh;
        
        GLuint tex;
        /* Create a texture that will be used to hold all ASCII glyphs */
        glActiveTexture(GL_TEXTURE0);
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        glUniform1i(uniform_tex, 0);
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, w, h, 0, GL_RED, GL_UNSIGNED_BYTE, 0);
        
        /* We require 1 byte alignment when uploading texture data */
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        
        /* Clamping to edges is important to prevent artifacts when scaling */
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        
        /* Linear filtering usually looks best for text */
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        
        /* Paste all glyph bitmaps into the texture, remembering the offset */
        int ox = 0;
        int oy = 0;
        
        rowh = 0;
        
        for (int i = 32; i < 128; i++) {
            if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
                fprintf(stderr, "Loading character %c failed!\n", i);
                continue;
            }
            
            if (ox + g->bitmap.width + 1 >= MAXWIDTH) {
                oy += rowh;
                rowh = 0;
                ox = 0;
            }
            
            glTexSubImage2D(GL_TEXTURE_2D, 0, ox, oy, g->bitmap.width, g->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);
            c[i].ax = g->advance.x >> 6;
            c[i].ay = g->advance.y >> 6;
            
            c[i].bw = g->bitmap.width;
            c[i].bh = g->bitmap.rows;
            
            c[i].bl = g->bitmap_left;
            c[i].bt = g->bitmap_top;
            
            c[i].tx = ox / (float)w;
            c[i].ty = oy / (float)h;
            
            rowh = std::max(rowh, g->bitmap.rows);
            ox += g->bitmap.width + 1;
        }
        fprintf(stderr, "Generated a %d x %d (%d kb) texture atlas\n", w, h, w * h / 1024);
        atlasTexture = tex;
    }
};
#endif /* TypeWriter_hpp */
