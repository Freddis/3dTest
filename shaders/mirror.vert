#version 330
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 textCoord;
layout (location = 2) in vec4 color;
layout (location = 3) in vec3 normal;
out vec2 TexCoord;
void main() {
    gl_Position =  vec4(position, 1.0f);
    TexCoord = textCoord;
}
