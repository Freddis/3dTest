#version 330 core
in vec2 TexCoord;
out vec4 color;
uniform sampler2D ourTexture;
void main()
{
    float depthValue = texture(ourTexture, TexCoord).r;
    color = vec4(vec3(depthValue), 1.0);
}
