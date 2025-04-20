#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <vector>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

std::vector<float> generateTorus(float majorRadius, float minorRadius, int majorSegments, int minorSegments)
{
    std::vector<float> vertices;
    // Each triangle needs 3 vertices, each vertex has 11 floats, and each grid cell has 2 triangles
    vertices.reserve(majorSegments * minorSegments * 2 * 3 * 11);

    float sectorStep = 2 * M_PI / majorSegments;
    float sideStep = 2 * M_PI / minorSegments;

    // For each grid cell in the parametric surface, create two triangles
    for (int i = 0; i < minorSegments; i++)
    {
        float sideAngle1 = i * sideStep;
        float sideAngle2 = (i + 1) * sideStep;

        for (int j = 0; j < majorSegments; j++)
        {
            float sectorAngle1 = j * sectorStep;
            float sectorAngle2 = (j + 1) * sectorStep;

            // Calculate the 4 corners of the grid cell
            // (i, j), (i+1, j), (i, j+1), (i+1, j+1)
            //
            //  j--j+1
            //  |  / |
            //  | /  |
            //  i--i+1

            // Helper function to generate a vertex
            auto generateVertex = [majorRadius, minorRadius, &vertices](float sideA, float sectorA) {
                float r = 1.0f, g = 1.0f, b = 1.0f; // color
                float xy = minorRadius * cosf(sideA);
                float z = minorRadius * sinf(sideA);

                float x = xy * cosf(sectorA);
                float y = xy * sinf(sectorA);

                float lengthInv = 1.0f / minorRadius;
                float nx = x * lengthInv;
                float ny = y * lengthInv;
                float nz = z * lengthInv;

                x += majorRadius * cosf(sectorA);
                y += majorRadius * sinf(sectorA);

                float s = sectorA / (2 * M_PI);
                float t = sideA / (2 * M_PI);

                vertices.insert(vertices.end(), { x, y, z, r, g, b, nx, ny, nz, s, t });
                };

            // Triangle 1: (i,j) -> (i+1,j) -> (i,j+1)
            generateVertex(sideAngle1, sectorAngle1);
            generateVertex(sideAngle2, sectorAngle1);
            generateVertex(sideAngle1, sectorAngle2);

            // Triangle 2: (i+1,j) -> (i+1,j+1) -> (i,j+1)
            generateVertex(sideAngle2, sectorAngle1);
            generateVertex(sideAngle2, sectorAngle2);
            generateVertex(sideAngle1, sectorAngle2);
        }
    }

    return vertices;
}
