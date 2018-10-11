#version 330
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 textCoord;
layout (location = 2) in vec4 color;
layout (location = 3) in vec3 normal;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightspace;
out vec2 TexCoord;
out vec4 mycolor;
out vec3 FragPos;
out vec3 Normal;
out vec4 FragPosLightSpace;

void main() {
    gl_Position = projection * view * model * vec4(position, 1.0f);
    FragPos = vec3(model * vec4(position, 1.0f));
    TexCoord = textCoord;
    mycolor = color;
    Normal = normal;
    FragPosLightSpace = lightspace * vec4(FragPos, 1.0);
}
