#version 330 core
in vec2 TexCoord;
out vec4 color;
in vec4 mycolor;
in vec3 FragPos;
in vec3 Normal;
void main()
{
    color = mycolor;
}
