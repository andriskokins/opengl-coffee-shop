#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <windows.h>
#include <wingdi.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include <stdexcept>
#include <cstring>
#include <glm/glm.hpp>

#define PIXEL_W 1920
#define PIXEL_H 1080

float colour_buffer[PIXEL_W * PIXEL_H * 3];
float depth_buffer[PIXEL_W * PIXEL_H];

struct vertex
{
    glm::vec4 pos;
    glm::vec3 col;
    glm::vec3 nor;
    glm::vec2 tex;
};

struct triangle
{
public:
    vertex v1, v2, v3;
    bool reflect;
    int primID;
};

int obj_parse(const char* filename, std::vector<triangle>* io_tris)
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename))
    {
        std::cerr << "Warning: " << warn << std::endl;
        std::cerr << "Error: " << err << std::endl;
        throw std::runtime_error(warn + err);
    }

    std::vector<vertex> vertices;
    std::vector<uint32_t> indices;

    for (const auto& shape : shapes)
    {
        for (const auto& index : shape.mesh.indices)
        {
            vertex vert{};

            // Position
            vert.pos = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2],
                1.f
            };

            // Normal (if available)
            if (!attrib.normals.empty() && index.normal_index >= 0)
            {
                vert.nor = {
                    attrib.normals[3 * index.normal_index + 0],
                    attrib.normals[3 * index.normal_index + 1],
                    attrib.normals[3 * index.normal_index + 2]
                };
            }
            else {
                vert.nor = { 0, 0, 0 };
            }

            // Texture coordinates
            if (!attrib.texcoords.empty() && index.texcoord_index >= 0)
            {
                vert.tex = {
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    attrib.texcoords[2 * index.texcoord_index + 1]
                };
            }
            else {
                vert.tex = { 0, 0 };
            }

            // Color from OBJ if available; OBJ colors, if present, are stored in groups of 3 floats.
            if (!attrib.colors.empty() && (3 * index.vertex_index + 2) < attrib.colors.size())
            {
                vert.col = {
                    attrib.colors[3 * index.vertex_index + 0],
                    attrib.colors[3 * index.vertex_index + 1],
                    attrib.colors[3 * index.vertex_index + 2]
                };
            }
            else {
                vert.col = { 1, 1, 1 };
            }

            vertices.push_back(vert);
            indices.push_back(static_cast<uint32_t>(indices.size()));
        }
    }

    for (size_t i = 0; i < indices.size() / 3; i++)
    {
        triangle tri;
        tri.v1 = vertices[indices[(i * 3) + 0]];
        tri.v2 = vertices[indices[(i * 3) + 1]];
        tri.v3 = vertices[indices[(i * 3) + 2]];

        tri.reflect = false;
        tri.primID = static_cast<int>(i);

        io_tris->push_back(tri);
    }

    printf("successfully parsed %s and read %d triangles \n", filename, (int)io_tris->size());
    return 0;
}

std::vector<float> triangleToVertices(std::vector<triangle>& triangles)
{
    // Each vertex contains: 3 floats (position) + 3 floats (color) + 3 floats (normal) + 2 floats (texture) = 11 floats per vertex.
    size_t floatsPerVertex = 11;
    size_t floatsPerTriangle = 3 * floatsPerVertex;
    std::vector<float> vertices;
    vertices.reserve(triangles.size() * floatsPerTriangle);

    auto addVertex = [&vertices](const vertex& vert)
        {
            // Position
            vertices.push_back(vert.pos.x);
            vertices.push_back(vert.pos.y);
            vertices.push_back(vert.pos.z);
            // Color
            vertices.push_back(vert.col.x);
            vertices.push_back(vert.col.y);
            vertices.push_back(vert.col.z);
            // Normal
            vertices.push_back(vert.nor.x);
            vertices.push_back(vert.nor.y);
            vertices.push_back(vert.nor.z);
            // Texture coordinates
            vertices.push_back(vert.tex.x);
            vertices.push_back(vert.tex.y);
        };

    for (auto& tri : triangles)
    {
        addVertex(tri.v1);
        addVertex(tri.v2);
        addVertex(tri.v3);
    }
    return vertices;
}