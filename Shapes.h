//
//  Shapes.h
//  COMP410-PROJECT
//
//  Created by Ecenaz Eğri on 28.04.2025.
//

#ifndef SHAPES_H
#define SHAPES_H

#include "Angel.h"
#include <GLFW/glfw3.h>
#include <vector>

typedef vec4 point4;
typedef vec4 color4;

void createGround(vec3 center, float size, std::vector<point4>& outVertices, std::vector<vec3>& normals);
void createCube(vec3 center, float size, std::vector<point4>& outVertices, std::vector<vec3>& normals);
void createPyramid(vec3 center, float size, std::vector<point4>& outVertices, std::vector<vec3>& normals);
void createRectanglePrism(vec3 center, float length, float width, float height, std::vector<point4>& outVertices, std::vector<vec3>& normals);
void createHalfCube(vec3 center, float size, std::vector<point4>& outVertices, std::vector<vec3>& normals);
void createTriangularPrism(vec3 center, float size, std::vector<point4>& outVertices, std::vector<vec3>& normals);
void createCylinder(vec3 center, float radius, float height,std::vector<point4>& outVertices, std::vector<vec3>& normals);
void createSphere(vec3 center, float radius, std::vector<point4>& outVertices, std::vector<vec3>& normals);
void createHalfCylinderPrism(vec3 center, float radius, float height, std::vector<point4>& outVertices, std::vector<vec3>& normals);
void createBridgeShape(vec3 center, float width, float height, float length, float radius, std::vector<point4>& outVertices, std::vector<vec3>& normals);

#endif // SHAPES_H
