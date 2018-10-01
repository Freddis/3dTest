//
//  TextureLoader.hpp
//  test
//
//  Created by Сарычев Алексей on 01.10.2018.
//  Copyright © 2018 Home-studio. All rights reserved.
//

#ifndef TextureLoader_hpp
#define TextureLoader_hpp

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <OpenGL/gl3.h>
#include <IL/il.h>
#include <IL/ilu.h>

class TextureLoader
{
    bool ilinited = false;
    public:
    TextureLoader()
    {
        
    }
    GLuint loadWithDevil(const char* filename)
    {
        if(!ilinited)
        {
            ilInit();
            ilinited = true;
        }
        
        ILuint ImageName;
        ilGenImages(1, &ImageName);
        ilBindImage(ImageName);
        
        // ILHANDLE handle = fopen(filename,"rb");
        ILboolean textureLoaded = ilLoadImage(filename);
        if(!textureLoaded)
        {
            ILenum error = ilGetError();
            std::cout << "Failed to load '" << filename << "'." << error << std::endl;
            return 0;
        }
        GLsizei width  = ilGetInteger(IL_IMAGE_WIDTH);
        GLsizei height = ilGetInteger(IL_IMAGE_HEIGHT);
        GLuint texture = this->generateTexture(ilGetData(), width, height);
        ilDeleteImages(1, &ImageName);
        return texture;
    }
    
    GLuint loadSimpleBmp(const char* filename,int width, int height)
    {
        //int width, height;
        
        unsigned char * data;
        
        FILE * file;
        
        file = fopen( filename, "rb" );
        
        if ( file == NULL ) return 0;
        //    width = 1024;
        //    height = 512;
        data = (unsigned char *)malloc( width * height * 3 );
        //int size = fseek(file,);
        fread( data, width * height * 3, 1, file );
        fclose( file );
        
        for(int i = 0; i < width * height ; ++i)
        {
            int index = i*3;
            unsigned char B,R;
            B = data[index];
            R = data[index+2];
            
            data[index] = R;
            data[index+2] = B;
            
        }
        
        GLuint texture = this->generateTexture(data, width, height);
        free(data);
        return texture;
    }
protected:
    GLuint generateTexture(const GLvoid *data,GLsizei width, GLsizei height)
    {
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture); // All upcoming GL_TEXTURE_2D operations now have effect on this texture object
        // Set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);    // Set texture wrapping to GL_REPEAT (usually basic wrapping method)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // Set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        return texture;
    }
};

#endif /* TextureLoader_hpp */
