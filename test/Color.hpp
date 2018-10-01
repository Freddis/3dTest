//
//  Color.hpp
//  test
//
//  Created by Freddis on 23.09.2018.
//  Copyright © 2018 Home-studio. All rights reserved.
//

#ifndef Color_hpp
#define Color_hpp

#include <stdio.h>

class Color
{    
public:
    float r,g,b,a;
    
    Color(float r, float g, float b) : Color(r,g,b,1)
    {
        this->a = 1;
    }
    Color(float r, float g, float b,float a)
    {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;
    }
    Color * copy()
    {
        return new Color(r,g,b,a);
    }
    
    static Color* getRed()
    {
        return new Color(1,0,0);
    }
    static Color* getGreen()
    {
        return new Color(0,1,0);
    }
    static Color* getBlue()
    {
        return new Color(0,0,1);
    }
    static Color* getCyan()
    {
        return new Color(0,1,1);
    }
    static Color* getMagenta()
    {
        return new Color(1,0,1);
    }
    static Color* getYellow()
    {
        return new Color(1,1,0);
    }
    static Color* getBlack()
    {
        return new Color(0,0,0);
    }
    static Color* getWhite()
    {
        return new Color(1,1,1);
    }


};


#endif /* Color_hpp */
