#pragma once
#include "model.h"

void calculateAABB(model&);
AABB calculateWorldAABB(model&);

void calculateAABB(model& m)
{
    m.aabb.min = glm::vec3(std::numeric_limits<float>::max());
    m.aabb.max = glm::vec3(std::numeric_limits<float>::lowest());
    // Stride is 11 floats: pos(3), col(3), alpha(1), norm(3), tex(2)
    for (int i = 0; i < m.vertices.size(); i += 12)
    {
        glm::vec3 pos(m.vertices[i], m.vertices[i + 1], m.vertices[i + 2]);
        m.aabb.min = glm::min(m.aabb.min, pos);
        m.aabb.max = glm::max(m.aabb.max, pos);
    }
    printf("Collision : Successfully calculated local AABB for %d\n", m.bufferIndex);
}

AABB calculateWorldAABB(model& m)
{
    // Get the 8 corners of the local AABB
    glm::vec3 corners[8] = {
        glm::vec3(m.aabb.min.x, m.aabb.min.y, m.aabb.min.z),
        glm::vec3(m.aabb.max.x, m.aabb.min.y, m.aabb.min.z),
        glm::vec3(m.aabb.max.x, m.aabb.max.y, m.aabb.min.z),
        glm::vec3(m.aabb.min.x, m.aabb.max.y, m.aabb.min.z),
        glm::vec3(m.aabb.min.x, m.aabb.min.y, m.aabb.max.z),
        glm::vec3(m.aabb.max.x, m.aabb.min.y, m.aabb.max.z),
        glm::vec3(m.aabb.max.x, m.aabb.max.y, m.aabb.max.z),
        glm::vec3(m.aabb.min.x, m.aabb.max.y, m.aabb.max.z)
    };

    // Calculate the world transformation matrix 
    glm::mat4 modelMatrix = glm::mat4(1.f);
    modelMatrix = glm::translate(modelMatrix, m.position);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(m.rotation.x), glm::vec3(1.f, 0.f, 0.f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(m.rotation.y), glm::vec3(0.f, 1.f, 0.f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(m.rotation.z), glm::vec3(0.f, 0.f, 1.f));
    modelMatrix = glm::scale(modelMatrix, m.scale);

    AABB worldAABB;
    // Transform each corner
    for (glm::vec3 corner : corners)
    {
        glm::vec4 transformedCorner = modelMatrix * glm::vec4(corner, 1.f);
        worldAABB.min = glm::min(worldAABB.min, glm::vec3(transformedCorner));
        worldAABB.max = glm::max(worldAABB.max, glm::vec3(transformedCorner));
    }
    return worldAABB;
}

// https://en.wikipedia.org/wiki/Slab_method
bool intersectRayAABB(const glm::vec3& rayOrigin, const glm::vec3& rayDir, const AABB& box, float& intersectionDistance) {
    // Handle division by zero if ray direction component is zero
    glm::vec3 invDir;
    invDir.x = (rayDir.x == 0.0f) ? std::numeric_limits<float>::infinity() : 1.0f / rayDir.x;
    invDir.y = (rayDir.y == 0.0f) ? std::numeric_limits<float>::infinity() : 1.0f / rayDir.y;
    invDir.z = (rayDir.z == 0.0f) ? std::numeric_limits<float>::infinity() : 1.0f / rayDir.z;

    glm::vec3 tMin = (box.min - rayOrigin) * invDir;
    glm::vec3 tMax = (box.max - rayOrigin) * invDir;

    glm::vec3 t1 = glm::min(tMin, tMax);
    glm::vec3 t2 = glm::max(tMin, tMax);

    float tClose = glm::max(glm::max(t1.x, t1.y), t1.z);
    float tFar = glm::min(glm::min(t2.x, t2.y), t2.z);

    // Condition for intersection
    if (tClose > tFar || tFar < 0.0f) {
        return false; // No intersection or intersection is behind the ray origin
    }

    // If tNear is negative, the origin is inside the box, intersection is at origin (distance 0)
    // Otherwise, intersection is at tNear distance
    intersectionDistance = (tClose < 0.0f) ? 0.0f : tClose;
    return true;
}