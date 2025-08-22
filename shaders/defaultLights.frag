#version 330 core
in vec2 TexCoord;
out vec4 color;
in vec4 mycolor;
in vec3 FragPos;
in vec3 Normal;
uniform sampler2D ourTexture;
uniform vec3 lightPos;
uniform vec3 viewPos;
void main()
{
    vec4 objectColor = texture(ourTexture, TexCoord*1) * mycolor;

    vec3 lightColor =  vec3(1.0f,1.0f,1.0f);
    float ambientStrength = 0.05f;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = Normal;
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    float specularStrength = 1.0f;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec4 result =  objectColor * vec4(ambient+diffuse+specular,1.0f);
    color = result;
}
