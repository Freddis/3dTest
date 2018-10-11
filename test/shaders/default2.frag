#version 330 core
in vec2 TexCoord;
out vec4 color;
in vec4 mycolor;
in vec3 FragPos;
in vec3 Normal;
in vec4 FragPosLightSpace;
uniform sampler2D ourTexture;
uniform sampler2D shadowMap;
uniform vec3 lightPos;
uniform vec3 viewPos;

float LinearizeDepth(float depth)
{
    float near_plane = 0.02f;
    float far_plane = 1000.0f;
    float z = depth * 2.0 - 1.0; // Back to NDC
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));
}

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    
//    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;
    //float bias = max(0.05 * (1.0 - dot(Normal, normalize(lightPos - FragPos))), 0.005);
    float bias = 0.00005;
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    
    if(projCoords.z > 1.0)
        shadow = 1.0;
    
    return shadow;
}

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
    
    float specularStrength = 2.0f;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 300);
    vec3 specular = specularStrength * spec * lightColor;
    
    float shadow = ShadowCalculation(FragPosLightSpace);
//    vec4 result =  vec4(objectColor.xyz * (1-shadow),1.0f);
    vec4 result =  objectColor * vec4(ambient+specular+ (1-shadow)*(diffuse),1.0f);
    color = result;
}
