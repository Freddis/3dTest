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

float ShadowCalculationPCF(vec4 fragPosLightSpace)
{
    float shadow = 0.0;
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    if(projCoords.z > 1.0)
    {
        return shadow;
    }
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    float biasVal = 0.00005;
    float bias = max(biasVal * (1.0 - dot(Normal, normalize(lightPos - FragPos))),biasVal);
    
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    int rad = 1;
    for(int x = -rad; x <= rad; ++x)
    {
        for(int y = -rad; y <= rad; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= rad*10.0;
    return shadow;
}
float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    if(projCoords.z > 1.0)
    {
        return 0.0f;
    }
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    
//    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;
    float biasVal = 0.00005;
    float bias = max(0.05 * (1.0 - dot(Normal, normalize(lightPos - FragPos))), 0.005);
//    float bias = 0.00005;
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    return shadow;
}

void main()
{
    vec4 objectColor = texture(ourTexture, TexCoord*1) * mycolor;
    
    float constant = 1.0f;
    float linear = 0.39f;
    float quadratic = 1.32f;
    float distance    = length(lightPos - FragPos);
    float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));
    
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
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
    vec3 specular = specularStrength * spec * lightColor;
    
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    
    float shadow = ShadowCalculationPCF(FragPosLightSpace);
//    vec4 result =  vec4(objectColor.xyz * (1-shadow),1.0f);
    vec4 result =  objectColor * vec4(ambient+(1-shadow)*(specular + diffuse),1.0f);
    color = result;
}
