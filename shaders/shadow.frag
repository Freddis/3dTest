#version 330 core
in vec2 TexCoord;
out vec4 color;
in vec4 mycolor;
void main()
{
    color = mycolor;
}
