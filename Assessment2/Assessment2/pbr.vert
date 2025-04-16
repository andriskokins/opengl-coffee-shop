#version 450 core


layout (location = 0) in vec3 vPos;
layout (location = 1) in vec4 vCol;
layout (location = 2) in vec3 vNor;
layout (location = 3) in vec2 vTexCoords;

#define MAX_LIGHTS 7

out vec4 col;
out vec3 nor;
out vec3 FragPosWorldSpace;
out vec2 TexCoords;
out vec4 FragPosLightSpace[MAX_LIGHTS]; // Array for multiple light spaces

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrices[MAX_LIGHTS]; // Array of light space matrices
uniform int numLights;

void main()
{
    FragPosWorldSpace = vec3(model * vec4(vPos, 1.0));
    
    // Calculate fragment position in each light's space
    for(int i = 0; i < numLights; i++) {
        FragPosLightSpace[i] = lightSpaceMatrices[i] * vec4(FragPosWorldSpace, 1.0);
    }
    
    col = vCol;
    nor = mat3(transpose(inverse(model))) * vNor;
    TexCoords = vTexCoords;
    
    gl_Position = projection * view * model * vec4(vPos, 1.0);
}