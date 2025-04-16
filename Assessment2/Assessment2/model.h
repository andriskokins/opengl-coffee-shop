#pragma once
#include <vector>
#include <GL/glcorearb.h>
#include "object_parser.h"

struct PBRTextures {
    GLuint albedo = 0;  // (diffuse)
    GLuint normal = 0;
    GLuint metallic = 0;
    GLuint roughness = 0;
    GLuint ao = 0;
    GLuint opacity = 0;

    // Track which textures actually exist
    float textureScale = 1.f;
    bool hasAlbedo = false;
    bool hasNormal = false;
    bool hasMetallic = false;
    bool hasRoughness = false;
    bool hasAO = false;
    bool hasOpacity = false;
};

struct model
{
    // Geometry
    std::vector<triangle> triangles;
    std::vector<float> vertices;

    // Transformations
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 rotation = glm::vec3(0.0f);
    glm::vec3 scale = glm::vec3(1.0f);

    int bufferIndex;

    // Textures
    PBRTextures textures;
};
 
std::vector<model> models;

/**
 * Load a model from a .obj file 
 *
 *@return id of the newly added model
 */
int load(const std::string& objPath,
    const std::string& albedoPath,
    const std::string& normalPath = "",
    const std::string& roughnessPath = "",
    const std::string& metallicPath = "",
    const std::string& aoPath = "",
    const std::string& opacityPath = "")
	{

    model model;
    // Load model geometry
    obj_parse(objPath.c_str(), &model.triangles);
    model.vertices = triangleToVertices(model.triangles);
    model.bufferIndex = models.size();

    // Load model textures
    // Albedo texture is required
    model.textures.albedo = setup_texture(albedoPath.c_str());
    model.textures.hasAlbedo = true;

    if (!normalPath.empty())
    {
        model.textures.normal = setup_texture(normalPath.c_str());
        model.textures.hasNormal = true;
    }
    if (!roughnessPath.empty())
    {
        model.textures.roughness = setup_texture(roughnessPath.c_str());
        model.textures.hasRoughness = true;
    }
    if (!metallicPath.empty())
    {
        model.textures.metallic = setup_texture(metallicPath.c_str());
        model.textures.hasMetallic = true;
    }
    if (!aoPath.empty())
    {
        model.textures.ao = setup_texture(aoPath.c_str());
        model.textures.hasAO = true;
    }
    if (!opacityPath.empty())
    {
        model.textures.opacity = setup_texture(opacityPath.c_str());
        model.textures.hasOpacity = true;
    }

    // Initialise default parameters
    model.position = glm::vec3(0);
    model.rotation = glm::vec3(0);
    model.scale = glm::vec3(1);

    // Add model to the models vector
    models.push_back(model);
    return model.bufferIndex;
}

/**
 * Add a model using a vector of vertices

 * @return The buffer index of the model
 */
int addModel(std::vector<float> vertices, 
             const std::string& albedoPath,
             const std::string& normalPath = "",
             const std::string& roughnessPath = "",
             const std::string& metallicPath = "",
             const std::string& aoPath = "")
{
    model model;
    // Load model geometry
    model.vertices = vertices;
    model.bufferIndex = models.size();

    // Load model textures
    // Albedo texture is required
    model.textures.albedo = setup_texture(albedoPath.c_str());
    model.textures.hasAlbedo = true;

    if (!normalPath.empty())
    {
        model.textures.normal = setup_texture(normalPath.c_str());
        model.textures.hasNormal = true;
    }
    if (!roughnessPath.empty())
    {
        model.textures.roughness = setup_texture(roughnessPath.c_str());
        model.textures.hasRoughness = true;
    }
    if (!metallicPath.empty())
    {
        model.textures.metallic = setup_texture(metallicPath.c_str());
        model.textures.hasMetallic = true;
    }
    if (!aoPath.empty())
    {
        model.textures.ao = setup_texture(aoPath.c_str());
        model.textures.hasAO = true;
    }

    // Initialise default parameters
    model.position = glm::vec3(0);
    model.rotation = glm::vec3(0);
    model.scale = glm::vec3(1);

    // Add model to the models vector
    models.push_back(model);

    return models.back().bufferIndex;
}

void setTranformations(int id, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
{
    models.at(id).position = position;
    models.at(id).rotation = rotation;
    models.at(id).scale = scale;
}

/**
 * Makes a copy of a model and adds it to the vector of models
 *
 *@param id of model to be duplicated
 *@return id of the duplicated model
 */
int duplicateModel(int id)
{
    model duplicateModel = models.at(id);
    models.push_back(duplicateModel);

    return models.size() - 1;
}