//
//  Shapes.cpp
//  COMP410-PROJECT
//
//  Created by Ecenaz Eğri on 28.04.2025.
//
#include "Angel.h"
#include <GLFW/glfw3.h>
#include <vector>


typedef vec4 point4;
typedef vec4  color4;

// Create a flat ground quad (2 triangles) centered at (x, y, z) with given size (length of one side)
/*
void createGround(vec3 center, float size, std::vector<point4>& outVertices)
{
    float hs = size / 2.0;
    float x = center.x;
    float y = center.y;
    float z = center.z;

    point4 v[4] = {
        point4(x - hs, y, z - hs, 1.0), // bottom left
        point4(x + hs, y, z - hs, 1.0), // bottom right
        point4(x + hs, y, z + hs, 1.0), // top right
        point4(x - hs, y, z + hs, 1.0)  // top left
    };

    // Two triangles forming a quad
    outVertices.push_back(v[0]);
    outVertices.push_back(v[1]);
    outVertices.push_back(v[2]);

    outVertices.push_back(v[0]);
    outVertices.push_back(v[2]);
    outVertices.push_back(v[3]);
}
*/

void createGround(vec3 center, float size, std::vector<point4>& outVertices, std::vector<vec3>& normals)
{
    float hs = size / 2.0f;
    float x = center.x;
    float y = center.y;
    float z = center.z;

    point4 v[4] = {
        point4(x - hs, y, z - hs, 1.0f), // bottom left
        point4(x + hs, y, z - hs, 1.0f), // bottom right
        point4(x + hs, y, z + hs, 1.0f), // top right
        point4(x - hs, y, z + hs, 1.0f)  // top left
    };

    // Yer düzlemi için yukarı doğru normal (Y ekseni)
    vec3 normal = vec3(0.0f, 1.0f, 0.0f);

    // İki üçgen (quad) için vertex ve normal ekle
    outVertices.push_back(v[0]);
    normals.push_back(normal);
    outVertices.push_back(v[1]);
    normals.push_back(normal);
    outVertices.push_back(v[2]);
    normals.push_back(normal);

    outVertices.push_back(v[0]);
    normals.push_back(normal);
    outVertices.push_back(v[2]);
    normals.push_back(normal);
    outVertices.push_back(v[3]);
    normals.push_back(normal);
}


/*
// Create a cube centered at (x,y,z) with given size
void createCube(vec3 center, float size, std::vector<point4>& outVertices)
{
    float hs = size / 2.0;
    float x = center.x;
    float y = center.y;
    float z = center.z;

    point4 v[8] = {
        point4(x - hs, y - hs, z + hs, 1.0),
        point4(x - hs, y + hs, z + hs, 1.0),
        point4(x + hs, y + hs, z + hs, 1.0),
        point4(x + hs, y - hs, z + hs, 1.0),
        point4(x - hs, y - hs, z - hs, 1.0),
        point4(x - hs, y + hs, z - hs, 1.0),
        point4(x + hs, y + hs, z - hs, 1.0),
        point4(x + hs, y - hs, z - hs, 1.0)
    };

    // Each face is two triangles
    int faces[6][6] = {
        { 1, 0, 3, 1, 3, 2 }, // Front
        { 2, 3, 7, 2, 7, 6 }, // Right
        { 3, 0, 4, 3, 4, 7 }, // Bottom
        { 6, 5, 1, 6, 1, 2 }, // Top
        { 4, 5, 6, 4, 6, 7 }, // Back
        { 5, 4, 0, 5, 0, 1 }  // Left
    };

    for (int i = 0; i < 6; i++)
        for (int j = 0; j < 6; j++)
            outVertices.push_back(v[faces[i][j]]);
    
}
*/

// 1
void createCube(vec3 center, float size, std::vector<point4>& outVertices, std::vector<vec3>& normals)
{
    float hs = size / 2.0f;
    float x = center.x;
    float y = center.y;
    float z = center.z;

    point4 v[8] = {
        point4(x - hs, y - hs, z + hs, 1.0f),
        point4(x - hs, y + hs, z + hs, 1.0f),
        point4(x + hs, y + hs, z + hs, 1.0f),
        point4(x + hs, y - hs, z + hs, 1.0f),
        point4(x - hs, y - hs, z - hs, 1.0f),
        point4(x - hs, y + hs, z - hs, 1.0f),
        point4(x + hs, y + hs, z - hs, 1.0f),
        point4(x + hs, y - hs, z - hs, 1.0f)
    };

    int faces[6][6] = {
        { 1, 0, 3, 1, 3, 2 }, // Front
        { 2, 3, 7, 2, 7, 6 }, // Right
        { 3, 0, 4, 3, 4, 7 }, // Bottom
        { 6, 5, 1, 6, 1, 2 }, // Top
        { 4, 5, 6, 4, 6, 7 }, // Back
        { 5, 4, 0, 5, 0, 1 }  // Left
    };

    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j += 3) {
            // 3 vertex indexleri
            int i0 = faces[i][j];
            int i1 = faces[i][j + 1];
            int i2 = faces[i][j + 2];

            // Vertex pozisyonlarını vec3 olarak çıkaralım (x,y,z)
            vec3 p0(v[i0].x, v[i0].y, v[i0].z);
            vec3 p1(v[i1].x, v[i1].y, v[i1].z);
            vec3 p2(v[i2].x, v[i2].y, v[i2].z);

            // Yüzey normalini hesapla (çapraz çarpım)
            vec3 normal = normalize(cross(p1 - p0, p2 - p1));

            // Vertexleri ekle
            outVertices.push_back(v[i0]);
            outVertices.push_back(v[i1]);
            outVertices.push_back(v[i2]);

            // Normalleri her vertex için ekle
            normals.push_back(normal);
            normals.push_back(normal);
            normals.push_back(normal);
        }
    }
}

/*
// Create a pyramid with a square base centered at (x,y,z) with given size
void createPyramid(vec3 center, float size, std::vector<point4>& outVertices)
{
    float hs = size / 2.0; // Half size for easier calculations
    float x = center.x;
    float y = center.y;
    float z = center.z;

    // Define 4 base vertices for the square and 1 apex (top)
    point4 v[5] = {
        point4(x - hs, y - hs, z + hs, 1.0),  // Front-left
        point4(x + hs, y - hs, z + hs, 1.0),  // Front-right
        point4(x + hs, y - hs, z - hs, 1.0),  // Back-right
        point4(x - hs, y - hs, z - hs, 1.0),  // Back-left
        point4(x, y + hs, z, 1.0)             // Apex (the top point)
    };

    // 4 triangular faces for the sides and 2 triangles for the base
    int faces[8][3] = {
        { 0, 1, 4 },  // Front face (base: 0, 1, apex: 4)
        { 1, 2, 4 },  // Right face (base: 1, 2, apex: 4)
        { 2, 3, 4 },  // Back face (base: 2, 3, apex: 4)
        { 3, 0, 4 },  // Left face (base: 3, 0, apex: 4)
        { 0, 1, 2 },  // Front triangle of the base
        { 0, 2, 3 }   // Back triangle of the base
    };

    // Add all faces (sides and base) to outVertices
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 3; j++) {
            outVertices.push_back(v[faces[i][j]]);
        }
    }
}
*/

// 2
void createPyramid(vec3 center, float size, std::vector<point4>& outVertices, std::vector<vec3>& normals)
{
    float hs = size / 2.0f; // Yarı uzunluk
    float x = center.x;
    float y = center.y;
    float z = center.z;

    // Tabanın 4 köşesi ve tepe noktası
    point4 v[5] = {
        point4(x - hs, y - hs, z + hs, 1.0f),  // Ön-sol
        point4(x + hs, y - hs, z + hs, 1.0f),  // Ön-sağ
        point4(x + hs, y - hs, z - hs, 1.0f),  // Arka-sağ
        point4(x - hs, y - hs, z - hs, 1.0f),  // Arka-sol
        point4(x,      y + hs, z,      1.0f)   // Tepe noktası
    };

    // 4 yan yüz ve 2 taban üçgeni
    int faces[6][3] = {
        { 0, 1, 4 },  // Ön yüz
        { 1, 2, 4 },  // Sağ yüz
        { 2, 3, 4 },  // Arka yüz
        { 3, 0, 4 },  // Sol yüz
        { 2, 1, 0 },  // Taban üçgen 1
        { 3, 2, 0 }   // Taban üçgen 2
    };

    for (int i = 0; i < 6; i++) {
        int i0 = faces[i][0];
        int i1 = faces[i][1];
        int i2 = faces[i][2];

        // Vec3'ye dönüştür
        vec3 p0(v[i0].x, v[i0].y, v[i0].z);
        vec3 p1(v[i1].x, v[i1].y, v[i1].z);
        vec3 p2(v[i2].x, v[i2].y, v[i2].z);

        // Normal hesapla
        vec3 normal = normalize(cross(p1 - p0, p2 - p0));

        // Vertexleri ekle
        outVertices.push_back(v[i0]);
        outVertices.push_back(v[i1]);
        outVertices.push_back(v[i2]);

        // Her vertex için aynı normali ekle
        normals.push_back(normal);
        normals.push_back(normal);
        normals.push_back(normal);
    }
}


/*
// Create a rectangular prism centered at (x,y,z) with given size (length, width, height)
void createRectanglePrism(vec3 center, float length, float width, float height, std::vector<point4>& outVertices)
{
    // Calculate half the dimensions
    float hl = length / 2.0;
    float hw = width / 2.0;
    float hh = height / 2.0;

    float x = center.x;
    float y = center.y;
    float z = center.z;

    // Define the 8 vertices of the rectangular prism
    point4 v[8] = {
        point4(x - hl, y - hh, z + hw, 1.0),  // Front-bottom-left
        point4(x - hl, y + hh, z + hw, 1.0),  // Front-top-left
        point4(x + hl, y + hh, z + hw, 1.0),  // Front-top-right
        point4(x + hl, y - hh, z + hw, 1.0),  // Front-bottom-right
        point4(x - hl, y - hh, z - hw, 1.0),  // Back-bottom-left
        point4(x - hl, y + hh, z - hw, 1.0),  // Back-top-left
        point4(x + hl, y + hh, z - hw, 1.0),  // Back-top-right
        point4(x + hl, y - hh, z - hw, 1.0)   // Back-bottom-right
    };

    // Each face is two triangles (two triangles for each of the 6 faces)
    int faces[6][6] = {
        { 1, 0, 3, 1, 3, 2 }, // Front face
        { 2, 3, 7, 2, 7, 6 }, // Right face
        { 3, 0, 4, 3, 4, 7 }, // Bottom face
        { 6, 5, 1, 6, 1, 2 }, // Top face
        { 4, 5, 6, 4, 6, 7 }, // Back face
        { 5, 4, 0, 5, 0, 1 }  // Left face
    };

    // Push each face's vertices into the output vector
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            outVertices.push_back(v[faces[i][j]]);
        }
    }
}
*/

// 3
void createRectanglePrism(vec3 center, float length, float width, float height,
                          std::vector<point4>& outVertices,
                          std::vector<vec3>& normals)
{
    // Yarı boyutlar
    float hl = length / 2.0f;
    float hw = width / 2.0f;
    float hh = height / 2.0f;

    float x = center.x;
    float y = center.y;
    float z = center.z;

    // 8 köşe noktası
    point4 v[8] = {
        point4(x - hl, y - hh, z + hw, 1.0f),  // Front-bottom-left
        point4(x - hl, y + hh, z + hw, 1.0f),  // Front-top-left
        point4(x + hl, y + hh, z + hw, 1.0f),  // Front-top-right
        point4(x + hl, y - hh, z + hw, 1.0f),  // Front-bottom-right
        point4(x - hl, y - hh, z - hw, 1.0f),  // Back-bottom-left
        point4(x - hl, y + hh, z - hw, 1.0f),  // Back-top-left
        point4(x + hl, y + hh, z - hw, 1.0f),  // Back-top-right
        point4(x + hl, y - hh, z - hw, 1.0f)   // Back-bottom-right
    };

    // Her yüz için 2 üçgen (6 yüz, her yüz 6 indeks)
    int faces[6][6] = {
        { 1, 0, 3, 1, 3, 2 }, // Front face
        { 2, 3, 7, 2, 7, 6 }, // Right face
        { 3, 0, 4, 3, 4, 7 }, // Bottom face
        { 6, 5, 1, 6, 1, 2 }, // Top face
        { 4, 5, 6, 4, 6, 7 }, // Back face
        { 5, 4, 0, 5, 0, 1 }  // Left face
    };

    // Yüzlerin normalleri (dışa doğru)
    vec3 faceNormals[6] = {
        vec3(0.0f, 0.0f, 1.0f),   // Front
        vec3(1.0f, 0.0f, 0.0f),   // Right
        vec3(0.0f, -1.0f, 0.0f),  // Bottom
        vec3(0.0f, 1.0f, 0.0f),   // Top
        vec3(0.0f, 0.0f, -1.0f),  // Back
        vec3(-1.0f, 0.0f, 0.0f)   // Left
    };

    // Vertex ve normal ekle
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            outVertices.push_back(v[faces[i][j]]);
            normals.push_back(faceNormals[i]);
        }
    }
}


/*
void createHalfCube(vec3 center, float size, std::vector<point4>& outVertices)
{
    float hs = size / 2.0;
    float x = center.x;
    float y = center.y;
    float z = center.z;

    // 6 vertex: (3 ön yüz, 3 arka yüz)
    point4 v[6] = {
        point4(x - hs, y - hs, z - hs, 1.0), // 0: Front-bottom-left
        point4(x + hs, y - hs, z - hs, 1.0), // 1: Front-bottom-right
        point4(x - hs, y + hs, z - hs, 1.0), // 2: Front-top-left
        point4(x - hs, y - hs, z + hs, 1.0), // 3: Back-bottom-left
        point4(x + hs, y - hs, z + hs, 1.0), // 4: Back-bottom-right
        point4(x - hs, y + hs, z + hs, 1.0)  // 5: Back-top-left
    };

    // Yüzler (her yüz 2 veya 1 üçgen ile)
    int faces[][3] = {
        // Ön üçgen yüz
        {0, 1, 2},
        // Arka üçgen yüz
        {3, 5, 4},
        // Alt yüz (dikdörtgen gibi) (0-3-1-4)
        {0, 3, 1}, {1, 3, 4},
        // Sol yüz (dikdörtgen gibi) (0-2-3-5)
        {0, 2, 3}, {3, 2, 5},
        // Üst yüz (dikdörtgen gibi) (2-5-4-1)
        {2, 5, 4}, {2, 4, 1}
    };

    // Vertex'leri ekle
    for (int i = 0; i < sizeof(faces)/sizeof(faces[0]); i++) {
        outVertices.push_back(v[faces[i][0]]);
        outVertices.push_back(v[faces[i][1]]);
        outVertices.push_back(v[faces[i][2]]);
    }
}
*/

// 4
void createHalfCube(vec3 center, float size, std::vector<point4>& outVertices, std::vector<vec3>& normals)
{
    float hs = size / 2.0f;
    float x = center.x;
    float y = center.y;
    float z = center.z;

    // 6 vertex: (3 ön yüz, 3 arka yüz)
    point4 v[6] = {
        point4(x - hs, y - hs, z - hs, 1.0f), // 0: Ön-alt-sol
        point4(x + hs, y - hs, z - hs, 1.0f), // 1: Ön-alt-sağ
        point4(x - hs, y + hs, z - hs, 1.0f), // 2: Ön-üst-sol
        point4(x - hs, y - hs, z + hs, 1.0f), // 3: Arka-alt-sol
        point4(x + hs, y - hs, z + hs, 1.0f), // 4: Arka-alt-sağ
        point4(x - hs, y + hs, z + hs, 1.0f)  // 5: Arka-üst-sol
    };

    int faces[][3] = {
        {2, 1, 0},    // Ön üçgen
        {4, 5, 3},    // Arka üçgen
        {1, 3, 0}, {4, 3, 1},  // Alt yüz (dikdörtgen)
        {3, 2, 0}, {5, 2, 3},  // Sol yüz (dikdörtgen)
        {2, 5, 4}, {2, 4, 1}   // Üst yüz (dikdörtgen)
    };

    for (int i = 0; i < sizeof(faces) / sizeof(faces[0]); i++) {
        int i0 = faces[i][0];
        int i1 = faces[i][1];
        int i2 = faces[i][2];

        // Pozisyonları vec3 olarak çıkaralım
        vec3 p0(v[i0].x, v[i0].y, v[i0].z);
        vec3 p1(v[i1].x, v[i1].y, v[i1].z);
        vec3 p2(v[i2].x, v[i2].y, v[i2].z);

        // Yüzey normalini hesapla
        vec3 normal = normalize(cross(p1 - p0, p2 - p0));

        // Vertexleri ekle
        outVertices.push_back(v[i0]);
        outVertices.push_back(v[i1]);
        outVertices.push_back(v[i2]);

        // Her vertex için aynı normali ekle
        normals.push_back(normal);
        normals.push_back(normal);
        normals.push_back(normal);
    }
}

/*
void createTriangularPrism(vec3 center, float size, std::vector<point4>& outVertices)
{
    float hs = size / 2.0;
    float x = center.x;
    float y = center.y;
    float z = center.z;

    // 6 vertex: (3 ön üçgen, 3 arka üçgen)
    point4 v[6] = {
        point4(x - hs, y - hs, z + hs, 1.0), // 0: Front-bottom-left
        point4(x + hs, y - hs, z + hs, 1.0), // 1: Front-bottom-right
        point4(x,      y + hs, z + hs, 1.0), // 2: Front-top (orta üst)
        point4(x - hs, y - hs, z - hs, 1.0), // 3: Back-bottom-left
        point4(x + hs, y - hs, z - hs, 1.0), // 4: Back-bottom-right
        point4(x,      y + hs, z - hs, 1.0)  // 5: Back-top (orta üst)
    };

    // Yüzler (her yüz 2 üçgen veya tek üçgen)
    int faces[][3] = {
        // Ön yüz (üçgen)
        {0, 1, 2},
        // Arka yüz (üçgen)
        {5, 4, 3},
        // Alt yüz (dikdörtgen - 2 üçgen)
        {0, 3, 1},
        {1, 3, 4},
        // Sol yüz (dikdörtgen - 2 üçgen)
        {0, 2, 3},
        {3, 2, 5},
        // Sağ yüz (dikdörtgen - 2 üçgen)
        {1, 4, 2},
        {2, 4, 5}
    };

    // Vertex'leri ekle
    for (int i = 0; i < sizeof(faces)/sizeof(faces[0]); i++) {
        outVertices.push_back(v[faces[i][0]]);
        outVertices.push_back(v[faces[i][1]]);
        outVertices.push_back(v[faces[i][2]]);
    }
}
*/

// 5
void createTriangularPrism(vec3 center, float size, std::vector<point4>& outVertices, std::vector<vec3>& normals)
{
    float hs = size / 2.0f;
    float x = center.x;
    float y = center.y;
    float z = center.z;

    // 6 vertex: 3 ön üçgen, 3 arka üçgen
    point4 v[6] = {
        point4(x - hs, y - hs, z + hs, 1.0f), // 0: Ön-alt-sol
        point4(x + hs, y - hs, z + hs, 1.0f), // 1: Ön-alt-sağ
        point4(x,      y + hs, z + hs, 1.0f), // 2: Ön-üst-orta
        point4(x - hs, y - hs, z - hs, 1.0f), // 3: Arka-alt-sol
        point4(x + hs, y - hs, z - hs, 1.0f), // 4: Arka-alt-sağ
        point4(x,      y + hs, z - hs, 1.0f)  // 5: Arka-üst-orta
    };

    int faces[][3] = {
        {0, 1, 2},  // Ön üçgen
        {5, 4, 3},  // Arka üçgen
        {0, 3, 1}, {1, 3, 4}, // Alt yüz (dikdörtgen)
        {0, 2, 3}, {3, 2, 5}, // Sol yüz (dikdörtgen)
        {1, 4, 2}, {2, 4, 5}  // Sağ yüz (dikdörtgen)
    };

    for (int i = 0; i < sizeof(faces)/sizeof(faces[0]); i++) {
        int i0 = faces[i][0];
        int i1 = faces[i][1];
        int i2 = faces[i][2];

        // Vec3 pozisyonları çıkar
        vec3 p0(v[i0].x, v[i0].y, v[i0].z);
        vec3 p1(v[i1].x, v[i1].y, v[i1].z);
        vec3 p2(v[i2].x, v[i2].y, v[i2].z);

        // Normal vektör hesapla
        vec3 normal = normalize(cross(p1 - p0, p2 - p0));

        // Vertexleri ekle
        outVertices.push_back(v[i0]);
        outVertices.push_back(v[i1]);
        outVertices.push_back(v[i2]);

        // Normalleri her vertex için ekle
        normals.push_back(normal);
        normals.push_back(normal);
        normals.push_back(normal);
    }
}

/*
void createCylinder(vec3 center, float radius, float height, std::vector<point4>& outVertices)
{
    int slices = 36;
    const float angleStep = 2.0f * M_PI / slices;

    vec3 topCenter = center + vec3(0.0f, height / 2.0f, 0.0f);
    vec3 bottomCenter = center - vec3(0.0f, height / 2.0f, 0.0f);

    std::vector<point4> topVertices;
    std::vector<point4> bottomVertices;

    // Çember üst ve alt noktalarını hesapla
    for (int i = 0; i < slices; ++i) {
        float angle = i * angleStep;
        float x = radius * cos(angle);
        float z = radius * sin(angle);

        topVertices.push_back(point4(topCenter.x + x, topCenter.y, topCenter.z + z, 1.0f));
        bottomVertices.push_back(point4(bottomCenter.x + x, bottomCenter.y, bottomCenter.z + z, 1.0f));
    }


    // Kapak
    for (int i = 1; i < slices; ++i) {
        int next = (i + 1) % slices; // Son vertexten sonra başa dön
        
        // Üst kapak (her dilim için bir üçgen)
        outVertices.push_back(topVertices[0]);
        outVertices.push_back(topVertices[i]);
        outVertices.push_back(topVertices[next]);
        
        // Alt kapak (her dilim için bir üçgen - saat yönünün tersine)
        outVertices.push_back(bottomVertices[0]);
        outVertices.push_back(bottomVertices[next]);
        outVertices.push_back(bottomVertices[i]);
    }


    // Yan yüzler (her dilim için iki üçgen)
    for (int i = 0; i < slices; ++i) {
        int next = (i + 1) % slices;

        // 1. üçgen
        outVertices.push_back(topVertices[i]);
        outVertices.push_back(bottomVertices[i]);
        outVertices.push_back(bottomVertices[next]);

        // 2. üçgen
        outVertices.push_back(topVertices[i]);
        outVertices.push_back(bottomVertices[next]);
        outVertices.push_back(topVertices[next]);
    }
}
*/

// 6
void createCylinder(vec3 center, float radius, float height,
                    std::vector<point4>& outVertices, std::vector<vec3>& normals)
{
    int slices = 36;
    const float angleStep = 2.0f * M_PI / slices;

    vec3 topCenter = center + vec3(0.0f, height / 2.0f, 0.0f);
    vec3 bottomCenter = center - vec3(0.0f, height / 2.0f, 0.0f);

    std::vector<point4> topVertices;
    std::vector<point4> bottomVertices;

    // Çember üst ve alt vertexleri hesapla
    for (int i = 0; i < slices; ++i) {
        float angle = i * angleStep;
        float x = radius * cos(angle);
        float z = radius * sin(angle);

        topVertices.push_back(point4(topCenter.x + x, topCenter.y, topCenter.z + z, 1.0f));
        bottomVertices.push_back(point4(bottomCenter.x + x, bottomCenter.y, bottomCenter.z + z, 1.0f));
    }

    // Üst ve alt merkez noktaları (kapaklar için)
    point4 topCenterPoint(topCenter.x, topCenter.y, topCenter.z, 1.0f);
    point4 bottomCenterPoint(bottomCenter.x, bottomCenter.y, bottomCenter.z, 1.0f);

    vec3 upNormal(0.0f, 1.0f, 0.0f);
    vec3 downNormal(0.0f, -1.0f, 0.0f);

    // Üst kapak (fan şeklinde)
    for (int i = 0; i < slices; ++i) {
        int next = (i + 1) % slices;

        outVertices.push_back(topCenterPoint);
        outVertices.push_back(topVertices[i]);
        outVertices.push_back(topVertices[next]);

        normals.push_back(upNormal);
        normals.push_back(upNormal);
        normals.push_back(upNormal);
    }

    // Alt kapak (fan şeklinde, saat yönünün tersine)
    for (int i = 0; i < slices; ++i) {
        int next = (i + 1) % slices;

        outVertices.push_back(bottomCenterPoint);
        outVertices.push_back(bottomVertices[next]);
        outVertices.push_back(bottomVertices[i]);

        normals.push_back(downNormal);
        normals.push_back(downNormal);
        normals.push_back(downNormal);
    }

    // Yan yüzler (her dilim için 2 üçgen)
    for (int i = 0; i < slices; ++i) {
        int next = (i + 1) % slices;

        // İlk üçgen
        outVertices.push_back(topVertices[i]);
        outVertices.push_back(bottomVertices[i]);
        outVertices.push_back(bottomVertices[next]);

        // Normal için dışa bakan yönü hesapla (x,z düzleminde)
        vec3 normal0 = -normalize(vec3(topVertices[i].x - center.x, 0.0f, topVertices[i].z - center.z));
        vec3 normal1 = -normalize(vec3(bottomVertices[i].x - center.x, 0.0f, bottomVertices[i].z - center.z));
        vec3 normal2 = -normalize(vec3(bottomVertices[next].x - center.x, 0.0f, bottomVertices[next].z - center.z));

        normals.push_back(normal0);
        normals.push_back(normal1);
        normals.push_back(normal2);

        // İkinci üçgen
        outVertices.push_back(topVertices[i]);
        outVertices.push_back(bottomVertices[next]);
        outVertices.push_back(topVertices[next]);

        vec3 normal3 = -normalize(vec3(topVertices[next].x - center.x, 0.0f, topVertices[next].z - center.z));

        normals.push_back(normal0);
        normals.push_back(normal2);
        normals.push_back(normal3);
    }
}


/*
void createSphere(vec3 center, float radius, std::vector<point4>& outVertices)
{
    int slices = 36;
    int stacks = 36;

    // Yüzeyler için diziyi tanımlıyoruz
    std::vector<point4> sphereVertices;

    // Vertex'leri hesapla
    for (int i = 0; i <= stacks; ++i)
    {
        float phi = 3.14159265f * i / stacks; // 0'dan pi'ye
        float y = radius * cos(phi);
        float r = radius * sin(phi);

        for (int j = 0; j <= slices; ++j)
        {
            float theta = 2.0f * 3.14159265f * j / slices; // 0'dan 2pi'ye
            float x = r * cos(theta);
            float z = r * sin(theta);

            sphereVertices.push_back(point4(center.x + x, center.y + y, center.z + z, 1.0f));
        }
    }

    // Yüzeyleri oluştur (saf küre için üçgenler)
    for (int i = 0; i < stacks; ++i)
    {
        for (int j = 0; j < slices; ++j)
        {
            int first = i * (slices + 1) + j;
            int second = first + slices + 1;

            // İlk üçgen
            outVertices.push_back(sphereVertices[first]);
            outVertices.push_back(sphereVertices[second]);
            outVertices.push_back(sphereVertices[first + 1]);

            // İkinci üçgen
            outVertices.push_back(sphereVertices[second]);
            outVertices.push_back(sphereVertices[second + 1]);
            outVertices.push_back(sphereVertices[first + 1]);
        }
    }
}
*/

// 7
void createSphere(vec3 center, float radius,
                  std::vector<point4>& outVertices, std::vector<vec3>& normals)
{
    int slices = 36;
    int stacks = 36;

    std::vector<point4> sphereVertices;

    // Vertexleri hesapla
    for (int i = 0; i <= stacks; ++i)
    {
        float phi = 3.14159265f * i / stacks; // 0..pi
        float y = radius * cos(phi);
        float r = radius * sin(phi);

        for (int j = 0; j <= slices; ++j)
        {
            float theta = 2.0f * 3.14159265f * j / slices; // 0..2pi
            float x = r * cos(theta);
            float z = r * sin(theta);

            sphereVertices.push_back(point4(center.x + x, center.y + y, center.z + z, 1.0f));
        }
    }

    // Üçgenleri oluştur ve normalleri hesapla
    for (int i = 0; i < stacks; ++i)
    {
        for (int j = 0; j < slices; ++j)
        {
            int first = i * (slices + 1) + j;
            int second = first + slices + 1;

            // İlk üçgen vertexleri
            point4 v0 = sphereVertices[first];
            point4 v1 = sphereVertices[second];
            point4 v2 = sphereVertices[first + 1];

            outVertices.push_back(v0);
            outVertices.push_back(v1);
            outVertices.push_back(v2);

            // Normalleri hesapla (center'dan vertexe vektör)
            vec3 n0 = normalize(vec3(v0.x - center.x, v0.y - center.y, v0.z - center.z));
            vec3 n1 = normalize(vec3(v1.x - center.x, v1.y - center.y, v1.z - center.z));
            vec3 n2 = normalize(vec3(v2.x - center.x, v2.y - center.y, v2.z - center.z));

            normals.push_back(n0);
            normals.push_back(n1);
            normals.push_back(n2);

            // İkinci üçgen vertexleri
            v0 = sphereVertices[second];
            v1 = sphereVertices[second + 1];
            v2 = sphereVertices[first + 1];

            outVertices.push_back(v0);
            outVertices.push_back(v1);
            outVertices.push_back(v2);

            n0 = normalize(vec3(v0.x - center.x, v0.y - center.y, v0.z - center.z));
            n1 = normalize(vec3(v1.x - center.x, v1.y - center.y, v1.z - center.z));
            n2 = normalize(vec3(v2.x - center.x, v2.y - center.y, v2.z - center.z));

            normals.push_back(n0);
            normals.push_back(n1);
            normals.push_back(n2);
        }
    }
}

/*
void createHalfCylinderPrism(vec3 center, float radius, float length, std::vector<point4>& outVertices)
{
    int slices = 36;
    float angleStep = M_PI / slices; // Yarım daire: 0'dan pi'ye
    float halfLength = length / 2.0f;

    std::vector<point4> frontVertices;
    std::vector<point4> backVertices;

    // Ön ve arka yüzlerin noktalarını hesapla (XY düzleminde, Z eksenine göre derinlik)
    for (int i = 0; i <= slices; ++i) {
        float angle = i * angleStep;
        float x = radius * cos(angle);
        float y = radius * sin(angle);

        frontVertices.push_back(point4(center.x + x, center.y + y, center.z + halfLength, 1.0f));
        backVertices.push_back(point4(center.x + x, center.y + y, center.z - halfLength, 1.0f));
    }

    // Ön yüz (yarım daire)
    for (int i = 0; i < slices; ++i) {
        outVertices.push_back(point4(center.x, center.y, center.z + halfLength, 1.0f)); // merkez
        outVertices.push_back(frontVertices[i]);
        outVertices.push_back(frontVertices[i + 1]);
    }

    // Arka yüz (yarım daire - ters sıra)
    for (int i = 0; i < slices; ++i) {
        outVertices.push_back(point4(center.x, center.y, center.z - halfLength, 1.0f)); // merkez
        outVertices.push_back(backVertices[i + 1]);
        outVertices.push_back(backVertices[i]);
    }

    // Yan yüzler (dik kenarlar ve kavis)
    for (int i = 0; i < slices; ++i) {
        // Kavisli yüzey dilimi (iki üçgen)
        outVertices.push_back(frontVertices[i]);
        outVertices.push_back(backVertices[i]);
        outVertices.push_back(backVertices[i + 1]);

        outVertices.push_back(frontVertices[i]);
        outVertices.push_back(backVertices[i + 1]);
        outVertices.push_back(frontVertices[i + 1]);
    }

    // Düz alt kenar (kesilmiş düz yüzey)
    // En baştaki ve en sondaki noktalar düz kenarı oluşturuyor
    point4 fStart = frontVertices[0];
    point4 fEnd = frontVertices[slices];
    point4 bStart = backVertices[0];
    point4 bEnd = backVertices[slices];

    // Sol düz kenar
    outVertices.push_back(fStart);
    outVertices.push_back(bStart);
    outVertices.push_back(bEnd);

    outVertices.push_back(fStart);
    outVertices.push_back(bEnd);
    outVertices.push_back(fEnd);
}
 */

// 8
void createHalfCylinderPrism(vec3 center, float radius, float length,
                            std::vector<point4>& outVertices, std::vector<vec3>& normals)
{
    int slices = 36;
    float angleStep = M_PI / slices; // Yarım daire: 0..pi
    float halfLength = length / 2.0f;

    std::vector<point4> frontVertices;
    std::vector<point4> backVertices;

    // Ön ve arka yüzlerin noktalarını hesapla (XY düzleminde, Z eksenine göre derinlik)
    for (int i = 0; i <= slices; ++i) {
        float angle = i * angleStep;
        float x = radius * cos(angle);
        float y = radius * sin(angle);

        frontVertices.push_back(point4(center.x + x, center.y + y, center.z + halfLength, 1.0f));
        backVertices.push_back(point4(center.x + x, center.y + y, center.z - halfLength, 1.0f));
    }

    // Ön yüz (yarım daire)
    for (int i = 0; i < slices; ++i) {
        point4 centerFront(center.x, center.y, center.z + halfLength, 1.0f);
        outVertices.push_back(centerFront);
        outVertices.push_back(frontVertices[i]);
        outVertices.push_back(frontVertices[i + 1]);

        // Normaller - ön yüz normali düz -Z
        vec3 normalFront(0.0f, 0.0f, -1.0f);
        normals.push_back(normalFront);
        normals.push_back(normalFront);
        normals.push_back(normalFront);
    }

    // Arka yüz (yarım daire - ters sıra)
    for (int i = 0; i < slices; ++i) {
        point4 centerBack(center.x, center.y, center.z - halfLength, 1.0f);
        outVertices.push_back(centerBack);
        outVertices.push_back(backVertices[i + 1]);
        outVertices.push_back(backVertices[i]);

        // Normaller - arka yüz normali düz +Z
        vec3 normalBack(0.0f, 0.0f, +1.0f);
        normals.push_back(normalBack);
        normals.push_back(normalBack);
        normals.push_back(normalBack);
    }

    // Yan yüzler (kavisli yüzey dilimleri, iki üçgen)
    for (int i = 0; i < slices; ++i) {
        // Normal: Yarıçap vektöründen hesaplanır (XY düzleminde)
        vec3 normal0 = normalize(vec3(frontVertices[i].x - center.x, frontVertices[i].y - center.y, 0.0f));
        vec3 normal1 = normalize(vec3(backVertices[i].x - center.x, backVertices[i].y - center.y, 0.0f));
        vec3 normal2 = normalize(vec3(backVertices[i + 1].x - center.x, backVertices[i + 1].y - center.y, 0.0f));
        vec3 normal3 = normalize(vec3(frontVertices[i + 1].x - center.x, frontVertices[i + 1].y - center.y, 0.0f));

        // 1. üçgen
        outVertices.push_back(frontVertices[i]);
        outVertices.push_back(backVertices[i]);
        outVertices.push_back(backVertices[i + 1]);

        normals.push_back(normal0);
        normals.push_back(normal1);
        normals.push_back(normal2);

        // 2. üçgen
        outVertices.push_back(frontVertices[i]);
        outVertices.push_back(backVertices[i + 1]);
        outVertices.push_back(frontVertices[i + 1]);

        normals.push_back(normal0);
        normals.push_back(normal2);
        normals.push_back(normal3);
    }

    // Düz alt kenar (kesilmiş düz yüzey)
    // En baştaki ve en sondaki noktalar düz kenarı oluşturuyor
    point4 fStart = frontVertices[0];
    point4 fEnd = frontVertices[slices];
    point4 bStart = backVertices[0];
    point4 bEnd = backVertices[slices];

    // Normal düz alt kenar için -Y ekseni (alt taban aşağıda varsayımıyla)
    vec3 bottomNormal(0.0f, -1.0f, 0.0f);

    // Sol düz kenar (iki üçgen)
    outVertices.push_back(fStart);
    outVertices.push_back(bStart);
    outVertices.push_back(bEnd);

    normals.push_back(bottomNormal);
    normals.push_back(bottomNormal);
    normals.push_back(bottomNormal);

    outVertices.push_back(fStart);
    outVertices.push_back(bEnd);
    outVertices.push_back(fEnd);

    normals.push_back(bottomNormal);
    normals.push_back(bottomNormal);
    normals.push_back(bottomNormal);
}

/*
void createBridgeShape(vec3 center, float width, float height, float length, float radius, std::vector<point4>& outVertices)
{
    int slices = 36;
    float angleStep = M_PI / slices;
    float halfLength = length / 2.0f;
    float halfWidth = width / 2.0f;

    float x = center.x;
    float y = center.y;
    float z = center.z;

    // Üst dikdörtgen prizma kısmı (köprünün üstü)
    float topY = y + height;
    float flatBottomY = y;  // Düz kısmın alt seviyesi yarım dairenin üstüne denk gelir

    // Prizmanın üst kısmı (üst düz blok)
    std::vector<point4> boxVertices;

    point4 v[] = {
        point4(x - width, flatBottomY,     z + halfLength, 1.0),  // Front-bottom-left
        point4(x - width, topY,            z + halfLength, 1.0),  // Front-top-left
        point4(x + width, topY,            z + halfLength, 1.0),  // Front-top-right
        point4(x + width, flatBottomY,     z + halfLength, 1.0),  // Front-bottom-right
        point4(x - width, flatBottomY,     z - halfLength, 1.0),  // Back-bottom-left
        point4(x - width, topY,            z - halfLength, 1.0),  // Back-top-left
        point4(x + width, topY,            z - halfLength, 1.0),  // Back-top-right
        point4(x + width, flatBottomY,     z - halfLength, 1.0)   // Back-bottom-right
    };

    int faces[3][6] = {
        { 2, 3, 7, 2, 7, 6 }, // Right face
        { 6, 5, 1, 6, 1, 2 }, // Top face
        { 5, 4, 0, 5, 0, 1 }, // Left face
    };

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 6; j++)
            outVertices.push_back(v[faces[i][j]]);

    // Yarım daire boşluk
    std::vector<point4> frontArc;
    std::vector<point4> backArc;

    for (int i = 0; i <= slices; ++i) {
        float angle = i * angleStep;
        float arcX = radius * cos(angle);
        float arcY = radius * sin(angle);

        frontArc.push_back(point4(x + arcX, y + arcY, z + halfLength, 1.0f));
        backArc.push_back(point4(x + arcX, y + arcY, z - halfLength, 1.0f));
    }

    // Kavisli iç yüzey (köprünün altı)
    for (int i = 0; i < slices; ++i) {
        outVertices.push_back(frontArc[i]);
        outVertices.push_back(backArc[i]);
        outVertices.push_back(backArc[i + 1]);

        outVertices.push_back(frontArc[i]);
        outVertices.push_back(backArc[i + 1]);
        outVertices.push_back(frontArc[i + 1]);
    }
    
    // Bottom rectangle faces
    // right
    outVertices.push_back(frontArc[0]);
    outVertices.push_back(backArc[0]);
    outVertices.push_back(v[3]);
    
    outVertices.push_back(backArc[0]);
    outVertices.push_back(v[3]);
    outVertices.push_back(v[7]);
    // left
    outVertices.push_back(frontArc[slices]);
    outVertices.push_back(backArc[slices]);
    outVertices.push_back(v[0]);
    
    outVertices.push_back(backArc[slices]);
    outVertices.push_back(v[0]);
    outVertices.push_back(v[4]);
    
    
    // Front face
    for (int i = 0; i < slices; ++i) {
        if (i < slices/4){
            outVertices.push_back(frontArc[i]);
            outVertices.push_back(frontArc[i+1]);
            outVertices.push_back(v[3]);
            
            outVertices.push_back(frontArc[i]);
            outVertices.push_back(v[3]);
            outVertices.push_back(v[2]);
            
        } else if (i >= slices/4 and i < slices/2){
            outVertices.push_back(frontArc[i]);
            outVertices.push_back(frontArc[i-1]);
            outVertices.push_back(v[2]);
            
        } else if (i == slices/2){
            outVertices.push_back(frontArc[i]);
            outVertices.push_back(frontArc[i-1]);
            outVertices.push_back(v[2]);
            
            outVertices.push_back(frontArc[i]);
            outVertices.push_back(v[2]);
            outVertices.push_back(v[1]);
            
            outVertices.push_back(frontArc[i]);
            outVertices.push_back(frontArc[i+1]);
            outVertices.push_back(v[1]);
            
        } else if (i <= slices*3/4 and i > slices/2){
            outVertices.push_back(frontArc[i]);
            outVertices.push_back(frontArc[i+1]);
            outVertices.push_back(v[1]);
            
        } else if (i > slices*3/4){
            outVertices.push_back(frontArc[i]);
            outVertices.push_back(frontArc[i+1]);
            outVertices.push_back(v[0]);
            
            outVertices.push_back(frontArc[i]);
            outVertices.push_back(v[0]);
            outVertices.push_back(v[1]);
        }
        
    }
    
    // Back face
    for (int i = 0; i < slices; ++i) {
        if (i < slices/4){
            outVertices.push_back(backArc[i]);
            outVertices.push_back(backArc[i+1]);
            outVertices.push_back(v[7]);
            
            outVertices.push_back(backArc[i]);
            outVertices.push_back(v[7]);
            outVertices.push_back(v[6]);
            
        } else if (i >= slices/4 and i < slices/2){
            outVertices.push_back(backArc[i]);
            outVertices.push_back(backArc[i-1]);
            outVertices.push_back(v[6]);
            
        } else if (i == slices/2){
            outVertices.push_back(backArc[i]);
            outVertices.push_back(backArc[i-1]);
            outVertices.push_back(v[6]);
            
            outVertices.push_back(backArc[i]);
            outVertices.push_back(v[6]);
            outVertices.push_back(v[5]);
            
            outVertices.push_back(backArc[i]);
            outVertices.push_back(frontArc[i+1]);
            outVertices.push_back(v[5]);
            
        } else if (i <= slices*3/4 and i > slices/2){
            outVertices.push_back(backArc[i]);
            outVertices.push_back(backArc[i+1]);
            outVertices.push_back(v[5]);
        
        } else if (i > slices*3/4){
            outVertices.push_back(backArc[i]);
            outVertices.push_back(backArc[i+1]);
            outVertices.push_back(v[4]);
            
            outVertices.push_back(backArc[i]);
            outVertices.push_back(v[4]);
            outVertices.push_back(v[5]);
        }
        
    }
    
}
*/


void createBridgeShape(vec3 center, float width, float height, float length, float radius,
                       std::vector<point4>& outVertices, std::vector<vec3>& normals)
{
    int slices = 36;
    float angleStep = M_PI / slices;
    float halfLength = length / 2.0f;
    float halfWidth = width / 2.0f;

    float x = center.x;
    float y = center.y;
    float z = center.z;

    float topY = y + height;
    float flatBottomY = y;

    // Kutunun 8 köşesi
    point4 v[] = {
        point4(x - width, flatBottomY,     z + halfLength, 1.0f),  // Front-bottom-left (0)
        point4(x - width, topY,            z + halfLength, 1.0f),  // Front-top-left    (1)
        point4(x + width, topY,            z + halfLength, 1.0f),  // Front-top-right   (2)
        point4(x + width, flatBottomY,     z + halfLength, 1.0f),  // Front-bottom-right(3)
        point4(x - width, flatBottomY,     z - halfLength, 1.0f),  // Back-bottom-left  (4)
        point4(x - width, topY,            z - halfLength, 1.0f),  // Back-top-left     (5)
        point4(x + width, topY,            z - halfLength, 1.0f),  // Back-top-right    (6)
        point4(x + width, flatBottomY,     z - halfLength, 1.0f)   // Back-bottom-right (7)
    };

    // Kutunun yüzleri için üçgenler (sağ, üst, sol)
    int faces[3][6] = {
        { 2, 3, 7, 2, 7, 6 }, // Right face
        { 6, 5, 1, 6, 1, 2 }, // Top face
        { 5, 4, 0, 5, 0, 1 }, // Left face
    };

    // Normaller yüzeylere göre sabit
    vec3 rightNormal(-1.0f, 0.0f, 0.0f);
    vec3 topNormal(0.0f, 1.0f, 0.0f);
    vec3 leftNormal(1.0f, 0.0f, 0.0f);

    for (int i = 0; i < 3; i++)
    {
        vec3 currentNormal;
        if (i == 0) currentNormal = rightNormal;
        else if (i == 1) currentNormal = topNormal;
        else currentNormal = leftNormal;

        for (int j = 0; j < 6; j++)
        {
            outVertices.push_back(v[faces[i][j]]);
            normals.push_back(currentNormal);
        }
    }

    // Yarım daire kavisli boşluk
    std::vector<point4> frontArc;
    std::vector<point4> backArc;

    for (int i = 0; i <= slices; ++i) {
        float angle = i * angleStep;
        float arcX = radius * cos(angle);
        float arcY = radius * sin(angle);

        frontArc.push_back(point4(x + arcX, y + arcY, z + halfLength, 1.0f));
        backArc.push_back(point4(x + arcX, y + arcY, z - halfLength, 1.0f));
    }

    // Kavisli yüzey (köprünün altı) - normaller yarıçap yönünde
    for (int i = 0; i < slices; ++i) {
        vec3 normal0 = -normalize(vec3(frontArc[i].x - x, frontArc[i].y - y, 0.0f));
        vec3 normal1 = -normalize(vec3(backArc[i].x - x, backArc[i].y - y, 0.0f));
        vec3 normal2 = -normalize(vec3(backArc[i + 1].x - x, backArc[i + 1].y - y, 0.0f));
        vec3 normal3 = -normalize(vec3(frontArc[i + 1].x - x, frontArc[i + 1].y - y, 0.0f));

        outVertices.push_back(frontArc[i]);
        outVertices.push_back(backArc[i]);
        outVertices.push_back(backArc[i + 1]);
        normals.push_back(normal0);
        normals.push_back(normal1);
        normals.push_back(normal2);

        outVertices.push_back(frontArc[i]);
        outVertices.push_back(backArc[i + 1]);
        outVertices.push_back(frontArc[i + 1]);
        normals.push_back(normal0);
        normals.push_back(normal2);
        normals.push_back(normal3);
    }

    // Alt dikdörtgen yüzeyler (frontArc ve kutu köşeleri arasında)
    vec3 bottomNormal(0.0f, -1.0f, 0.0f); // Aşağı bakan yüzey normali

    // Sağ alt yüzey
    outVertices.push_back(frontArc[0]);
    outVertices.push_back(backArc[0]);
    outVertices.push_back(v[3]);
    outVertices.push_back(backArc[0]);
    outVertices.push_back(v[3]);
    outVertices.push_back(v[7]);

    for (int i = 0; i < 6; ++i) normals.push_back(bottomNormal);

    // Sol alt yüzey
    outVertices.push_back(frontArc[slices]);
    outVertices.push_back(backArc[slices]);
    outVertices.push_back(v[0]);
    outVertices.push_back(backArc[slices]);
    outVertices.push_back(v[0]);
    outVertices.push_back(v[4]);

    for (int i = 0; i < 6; ++i) normals.push_back(bottomNormal);

    // Ön yüz (kutunun ön yüzeyi + kavisli kısım arası bağlantı)
    for (int i = 0; i < slices; ++i) {
        if (i < slices/4) {
            // 2 üçgen ile kutunun ön üst kenarına bağla
            outVertices.push_back(frontArc[i]);
            outVertices.push_back(frontArc[i+1]);
            outVertices.push_back(v[3]);

            outVertices.push_back(frontArc[i]);
            outVertices.push_back(v[3]);
            outVertices.push_back(v[2]);

            // Normaller tahmini: ön yüz için +Z yönü
            for (int k = 0; k < 6; ++k) normals.push_back(vec3(0.0f, 0.0f, -1.0f));
        }
        else if (i >= slices/4 && i < slices/2) {
            outVertices.push_back(frontArc[i]);
            outVertices.push_back(frontArc[i-1]);
            outVertices.push_back(v[2]);
            for (int k = 0; k < 3; ++k) normals.push_back(vec3(0.0f, 0.0f, -1.0f));
        }
        else if (i == slices/2) {
            outVertices.push_back(frontArc[i]);
            outVertices.push_back(frontArc[i-1]);
            outVertices.push_back(v[2]);
            normals.push_back(vec3(0.0f, 0.0f, -1.0f));
            normals.push_back(vec3(0.0f, 0.0f, -1.0f));
            normals.push_back(vec3(0.0f, 0.0f, -1.0f));

            outVertices.push_back(frontArc[i]);
            outVertices.push_back(v[2]);
            outVertices.push_back(v[1]);
            normals.push_back(vec3(0.0f, 0.0f, -1.0f));
            normals.push_back(vec3(0.0f, 0.0f, -1.0f));
            normals.push_back(vec3(0.0f, 0.0f, -1.0f));

            outVertices.push_back(frontArc[i]);
            outVertices.push_back(frontArc[i+1]);
            outVertices.push_back(v[1]);
            normals.push_back(vec3(0.0f, 0.0f, -1.0f));
            normals.push_back(vec3(0.0f, 0.0f, -1.0f));
            normals.push_back(vec3(0.0f, 0.0f, -1.0f));
        }
        else if (i <= slices*3/4 && i > slices/2) {
            outVertices.push_back(frontArc[i]);
            outVertices.push_back(frontArc[i+1]);
            outVertices.push_back(v[1]);
            for (int k = 0; k < 3; ++k) normals.push_back(vec3(0.0f, 0.0f, -1.0f));
        }
        else if (i > slices*3/4) {
            outVertices.push_back(frontArc[i]);
            outVertices.push_back(frontArc[i+1]);
            outVertices.push_back(v[0]);

            outVertices.push_back(frontArc[i]);
            outVertices.push_back(v[0]);
            outVertices.push_back(v[1]);
            for (int k = 0; k < 6; ++k) normals.push_back(vec3(0.0f, 0.0f, -1.0f));
        }
    }

    // Arka yüz (benzer şekilde)
    for (int i = 0; i < slices; ++i) {
        if (i < slices/4){
            outVertices.push_back(backArc[i]);
            outVertices.push_back(backArc[i+1]);
            outVertices.push_back(v[7]);

            outVertices.push_back(backArc[i]);
            outVertices.push_back(v[7]);
            outVertices.push_back(v[6]);
            for (int k = 0; k < 6; ++k) normals.push_back(vec3(0.0f, 0.0f, 1.0f));
        }
        else if (i >= slices/4 && i < slices/2){
            outVertices.push_back(backArc[i]);
            outVertices.push_back(backArc[i-1]);
            outVertices.push_back(v[6]);
            for (int k = 0; k < 3; ++k) normals.push_back(vec3(0.0f, 0.0f, 1.0f));
        }
        else if (i == slices/2){
            outVertices.push_back(backArc[i]);
            outVertices.push_back(backArc[i-1]);
            outVertices.push_back(v[6]);
            normals.push_back(vec3(0.0f, 0.0f, 1.0f));
            normals.push_back(vec3(0.0f, 0.0f, 1.0f));
            normals.push_back(vec3(0.0f, 0.0f, 1.0f));

            outVertices.push_back(backArc[i]);
            outVertices.push_back(v[6]);
            outVertices.push_back(v[5]);
            normals.push_back(vec3(0.0f, 0.0f, 1.0f));
            normals.push_back(vec3(0.0f, 0.0f, 1.0f));
            normals.push_back(vec3(0.0f, 0.0f, 1.0f));

            outVertices.push_back(backArc[i]);
            outVertices.push_back(frontArc[i+1]);
            outVertices.push_back(v[5]);
            normals.push_back(vec3(0.0f, 0.0f, 1.0f));
            normals.push_back(vec3(0.0f, 0.0f, 1.0f));
            normals.push_back(vec3(0.0f, 0.0f, 1.0f));
        }
        else if (i <= slices*3/4 && i > slices/2){
            outVertices.push_back(backArc[i]);
            outVertices.push_back(backArc[i+1]);
            outVertices.push_back(v[5]);
            for (int k = 0; k < 3; ++k) normals.push_back(vec3(0.0f, 0.0f, 1.0f));
        }
        else if (i > slices*3/4){
            outVertices.push_back(backArc[i]);
            outVertices.push_back(backArc[i+1]);
            outVertices.push_back(v[4]);

            outVertices.push_back(backArc[i]);
            outVertices.push_back(v[4]);
            outVertices.push_back(v[5]);
            for (int k = 0; k < 6; ++k) normals.push_back(vec3(0.0f, 0.0f, 1.0f));
        }
    }
}

