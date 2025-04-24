#version 450 core

#define HASHSCALE3 vec3(.1031, .1030, .0973)

const float PI = 3.14159265359;
const int MAX_LIGHTS = 7;

layout (location = 0) out vec4 fColour;

in vec4 col;
in vec3 nor;
in vec3 FragPosWorldSpace;
in vec4 FragPosLightSpace[MAX_LIGHTS];
in vec2 TexCoords;

// Light type constants
const int DIRECTIONAL_LIGHT = 0;
const int POINT_LIGHT = 1;
const int SPOT_LIGHT = 2;

// Light structure
struct Light {
    bool isOn;
    int type;               // 0=directional, 1=point, 2=spot
    vec3 position;          // Used for point and spot lights
    vec3 direction;         // Used for directional and spot lights
    vec3 colour;            // Light colour
    float intensity;        // Light intensity multiplier
};

// Uniforms for lights
uniform Light lights[MAX_LIGHTS];
uniform int numLights;      // Actual number of lights

uniform sampler2D shadowMaps[MAX_LIGHTS];
uniform samplerCube shadowCubeMaps[MAX_LIGHTS];
uniform vec3 pointLightPositions[MAX_LIGHTS];
uniform mat4 lightSpaceMatrices[MAX_LIGHTS];
uniform float farPlane;
uniform float textureScale;
uniform vec3 camPos;

// material parameters
uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D aoMap;
uniform sampler2D opacityMap;

// Function declarations
vec3 fresnelSchlick(float, vec3);
float DistributionGGX(vec3, vec3, float);
float GeometrySmith(vec3, vec3, vec3, float);
vec3 calculatePBR(Light light, vec3 N, vec3 V, vec3 F0, int lightIndex);
vec3 getNormalFromMap();
float shadowOnFragment(vec4 fragPosLightSpace, int lightIndex);
float shadowCubeMapOnFragment(Light light, int lightIndex);

float roughness;
float metallic;
vec3 albedo;
vec3 normal;
float ao;

void main()
{
    vec3 N = normalize(nor);
    vec3 V = normalize(camPos - FragPosWorldSpace);

    float defaultRoughness = 0.5;
    float defaultMetallic = 0.5;
    float defaultAO = 1.0;
 
    // Full PBR texture mode
    vec2 scaledTexCoords = TexCoords * textureScale;

    albedo = pow(texture(albedoMap, scaledTexCoords).rgb, vec3(2.2));
    // Use texture or default value
    metallic = textureSize(metallicMap, 0).x > 1 ? texture(metallicMap, scaledTexCoords).r : defaultMetallic;
    roughness = textureSize(roughnessMap, 0).x > 1 ? texture(roughnessMap, scaledTexCoords).r : defaultRoughness;
    ao = textureSize(aoMap, 0).x > 1 ? texture(aoMap, scaledTexCoords).r : defaultAO;
            
    // Use normal map if available, otherwise use vertex normal
    normal = textureSize(normalMap, 0).x > 1 ? getNormalFromMap() : N;

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    // Initialize output color with ambient lighting
    vec3 ambient = vec3(0.03) * albedo * ao;
    vec3 Lo = vec3(0.0);
    
    // Process all lights
    for(int i = 0; i < numLights; i++) 
    {
        // Call only if light is switched on
        if(lights[i].isOn)
            Lo += calculatePBR(lights[i], N, V, F0, i);
    }
    
    vec3 color = ambient + Lo;
    
    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correction
    color = pow(color, vec3(1.0/2.2));  
    
    // use alpha value from colour input
    fColour = vec4(color, col.w);
}

// Main source for physical based lighting - https://learnopengl.com/PBR/Lighting
vec3 calculatePBR(Light light, vec3 N, vec3 V, vec3 F0, int lightIndex)
{
    vec3 L;
    float attenuation = 1.0;
    float constant = 1.0, linear = 0.09, quadratic = 0.032, cutOff, outerCutOff;
    cutOff = cos(radians(25.f));
    outerCutOff = cos(radians(45.f));
    
    if(light.type == DIRECTIONAL_LIGHT) {
        // For directional light, L is just the negative of the light direction
        L = normalize(-light.direction);
        attenuation = lights[lightIndex].intensity;
    } 
    else if(light.type == POINT_LIGHT || light.type == SPOT_LIGHT) {
        // For point and spot lights, calculate direction to the light
        vec3 lightDir = light.position - FragPosWorldSpace;
        float distance = length(lightDir);
        L = normalize(lightDir);
        
        // Calculate attenuation based on distance
        attenuation = 1.0 / (constant + linear * distance + quadratic * pow(distance, 2));
        
        // Additional spot light calculations
        if(light.type == SPOT_LIGHT) {
            float theta = dot(L, normalize(-light.direction));
            float epsilon = cutOff - outerCutOff;
            float spotIntensity = clamp((theta - outerCutOff) / epsilon, 0.0, 1.0);
            attenuation *= spotIntensity;
        }
    }

    // Early exit if the light has no effect (e.g., outside of spot light cone)
    if(attenuation < 0.001) return vec3(0.0);
    
    // Calculate shadow factor based on light type and index
    float shadow = 0.0;
    if(light.type == DIRECTIONAL_LIGHT || light.type == SPOT_LIGHT) {
        shadow = shadowOnFragment(FragPosLightSpace[lightIndex], lightIndex);
    }
    else if(light.type == POINT_LIGHT) {
        shadow = shadowCubeMapOnFragment(light, lightIndex);
    }
    
    // If completely in shadow, only return ambient contribution
    if(shadow >= 0.99) return vec3(0.0);
    
    // Rest of your PBR calculation remains the same
    vec3 H = normalize(V + L);
    
    // Calculate radiance
    vec3 radiance = light.colour * attenuation * light.intensity;
    
    // Cook-torrance BRDF
    float NDF = DistributionGGX(N, H, roughness);        
    float G = GeometrySmith(N, V, L, roughness);      
    vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);       
    
    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;    
    
    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
    vec3 specular = numerator / denominator;  
    
    // Add to outgoing radiance Lo
    float NdotL = max(dot(N, L), 0.0);                
    
    // Apply shadow factor to the direct lighting
    return (kD * albedo / PI + specular) * radiance * NdotL * (1.0 - shadow);
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;
    
    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * pow(denom, 2);
    
    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = pow(r, 2) / 8.0;

    float num = NdotV;
    float denom = NdotV * (1.0 - k) + k;
    
    return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);
        
    return ggx1 * ggx2;
}

vec3 getNormalFromMap()
{
    vec3 tangentNormal = texture(normalMap, TexCoords).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(FragPosWorldSpace);
    vec3 Q2  = dFdy(FragPosWorldSpace);
    vec2 st1 = dFdx(TexCoords);
    vec2 st2 = dFdy(TexCoords);

    vec3 N   = normalize(nor);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}


// Source - https://www.shadertoy.com/view/lldyDn
vec2 hash2d(vec2 p)
{
	vec3 p3 = fract(vec3(p.xyx) * HASHSCALE3);
    p3 += dot(p3, p3.yzx+19.19);
    return fract((p3.xx+p3.yz)*p3.zy);
}

// Main source for spot light shadows - https://www.youtube.com/watch?v=Q8w_z2Ye-Go&t=31s
float shadowOnFragment(vec4 fragPosLightSpace, int lightIndex)
{
    Light light = lights[lightIndex];
    // Perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    // Check if fragment is outside shadow map
    if(any(greaterThan(projCoords.xyz, vec3(1))) || any(lessThan(projCoords.xyz, vec3(0))))
        return 0.0; // Assume not in shadow

    float currentDepth = projCoords.z;
    
    vec3 lightDir = normalize(light.type == DIRECTIONAL_LIGHT ? -light.direction : light.position - FragPosWorldSpace);

    float bias;
    if(light.type == SPOT_LIGHT)
    {
        bias = max(0.0025f * (1.f - dot(normal, lightDir)), 0.00045f);
    } else
        bias = max(0.0025f * (1.f - dot(normal, lightDir)), 0.0005f);
    
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMaps[lightIndex], 0);
    
    // Generate a per-fragment random offset to jitter the sampling pattern
    // Source - https://www.youtube.com/watch?v=uueB2kVvbHo&t=262s
    vec2 randomOffset = hash2d(gl_FragCoord.xy) * texelSize * 0.5;
    
    // PCF with jittered samples for better quality
    const int PCF_SAMPLES = 9; // 3x3 grid
    for(int i = 0; i < PCF_SAMPLES; i++) {
        // Calculate grid position
        int x = i % 3 - 1;
        int y = i / 3 - 1;
        
        // Add random offset to break grid pattern
        vec2 offset = vec2(x, y) * texelSize + randomOffset;
        
        float pcfDepth = texture(shadowMaps[lightIndex], projCoords.xy + offset).r;
        shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
    }
    
    return shadow / float(PCF_SAMPLES);
}

// Source for positional light shadows - https://www.youtube.com/watch?v=Q8w_z2Ye-Go&t=157s
// shadowCubeMap.frag, shadowCubeMap.vert and shadowCubeMap.geom all based on the same video from @Victor Gordan on YouTube
float shadowCubeMapOnFragment(Light light, int lightIndex)
{
    float shadow = 0.0;
    
    vec3 fragToLight = FragPosWorldSpace - light.position;
    float currentDepth = length(fragToLight);
    
    vec3 lightDir = normalize(fragToLight);
    float bias = max(0.005 * (1.0 - dot(normal, lightDir)), 0.0005);
    
    // Point light shadow PCF technique source - https://learnopengl.com/Advanced-Lighting/Shadows/Point-Shadows
    int samples = 20;
    float viewDistance = length(camPos - FragPosWorldSpace);
    float diskRadius = (1.0 + (viewDistance / farPlane)) / 25.0;

    vec3 sampleOffsetDirections[20] = vec3[]
    (
       vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
       vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
       vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
       vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
       vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
    );

    for (int i = 0; i < samples; i++)
    {
        float closestDepth = texture(shadowCubeMaps[lightIndex], fragToLight + sampleOffsetDirections[i] * diskRadius).r;
        closestDepth *= farPlane;
        if(currentDepth - bias > closestDepth)
            shadow += 1.0;
    }
   
    shadow /= float(samples); 
    return shadow;
}