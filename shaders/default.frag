#version 330 core
in vec2 TexCoord;
in vec4 mycolor;
uniform sampler2D ourTexture;
out vec4 color;

void main()
{
    color = texture(ourTexture, TexCoord*1) * mycolor;
}

