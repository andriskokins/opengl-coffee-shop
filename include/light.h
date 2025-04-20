#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "shadow.h"

#define SH_MAP_WIDTH 2048
#define SH_MAP_HEIGHT 2048

// Predefined colours
#define RED glm::vec3(1, 0, 0)
#define GREEN glm::vec3(0, 1, 0)
#define BLUE glm::vec3(0, 0, 1)

enum LightType {
    DIRECTIONAL,
    POSITIONAL,
    SPOT
};

struct Light {
    bool isOn = TRUE;
    int type;
    glm::vec3 position = glm::vec3(0, 3.5, 0);
    glm::vec3 direction = glm::vec3(0);
    glm::vec3 colour = glm::vec3(1);
    float intensity = 1.f;
    ShadowStruct shadow;
};

std::vector<Light> lights;

void addDirectionalLight(glm::vec3 direction, glm::vec3 colour, float intensity)
{
    Light directionalLight;
    directionalLight.type = DIRECTIONAL;
    directionalLight.direction = direction;
    directionalLight.colour = colour;
    directionalLight.intensity = intensity;
    directionalLight.shadow = setup_shadowmap(SH_MAP_WIDTH, SH_MAP_HEIGHT);

    lights.push_back(directionalLight);
    printf("Success: added directional light\n");
}

void addPositionalLight(glm::vec3 position, glm::vec3 colour, float intensity)
{
    Light positionalLight;
    positionalLight.type = POSITIONAL;
    positionalLight.position = position;
    positionalLight.colour = colour;
    positionalLight.intensity = intensity;
    positionalLight.shadow = setup_shadow_cubemap(SH_MAP_WIDTH, SH_MAP_HEIGHT);

    lights.push_back(positionalLight);
    printf("Success: added positional light\n");
}

void addSpotLight(glm::vec3 direction, glm::vec3 position, glm::vec3 colour, float intensity)
{
    Light spotLight;
    spotLight.type = SPOT;
    spotLight.direction = direction;
    spotLight.position = position;
    spotLight.colour = colour;
    spotLight.intensity = intensity;
    spotLight.shadow = setup_shadowmap(SH_MAP_WIDTH, SH_MAP_HEIGHT);

    lights.push_back(spotLight);
    printf("Success: added spot light\n");
}

//void addLight(LightType type, glm::vec3 position = glm::vec3(0), glm::vec3 direction = glm::vec3(0), glm::vec3 colour = glm::vec3(1),
//    float intensity = 1.f, float constant = 1.f, float linear = 0.09f, float quadratic = 0.032f, float cutOff = 15.f, float outerCutOff = 25.f)
//{
//    Light light;
//    light.type = type;
//    light.position = position;
//    light.direction = direction;
//    light.colour = colour;
//    light.intensity = intensity;
//    light.constant = constant;
//    light.linear = linear;
//    light.quadratic = quadratic;
//    light.cutOff = glm::cos(glm::radians(cutOff));
//    light.outerCutOff = glm::cos(glm::radians(outerCutOff));
//
//    lights.push_back(light);
//}

std::vector<Light> getLights()
{
    return lights;
}

glm::vec3 rgb2vec(int r, int g, int b)
{
    return glm::vec3((float) r / 255.f, (float)g / 255.f, (float)b / 255.f);
}