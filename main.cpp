#include "Angel.h"
#include <GLFW/glfw3.h>
#include <vector>
#include "Shapes.h"


typedef vec4 point4;
typedef vec4 color4;

// Array of rotation angles (in degrees) for each coordinate axis
enum { Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3 };
int      Axis = Xaxis;
GLfloat  Theta[NumAxes] = { 0.0, 0.0, 0.0 };

const int CubeVertexCount = 36; // 6 faces * 2 triangles * 3 vertices
int objectNumber = 0;
// const int NumVertices = CubeVertexCount * objectNumber; // 2 cubes

std::vector<point4> vertices;

std::vector<vec3> normals;

std::vector<std::string> objectNames;


// std::vector<std::vector<point4>> vertices;
std::vector<int> beginIndex; // first index of the object in "vertices" vector
std::vector<int> numVertices; // vertices number of the object in "vertices" vector
int selectedObject = -1;
std::vector<vec3> centers; // Vector holding the centers of the objects
std::vector<color4> objectColors;

// Physics variables for gravity and collision
std::vector<vec3> velocities; // Velocity for each object
std::vector<bool> isGrounded; // Whether object is on ground/another object
const float GRAVITY = -0.001f; // Gravity constant (negative Y direction)
const float DAMPING = 0.8f; // Velocity damping when hitting ground
const float OBJECT_SIZE = 0.25f; // Default object size for collision detection

struct AABB {
    vec3 min;
    vec3 max;
};

#define FLT_MAX 3.402823466e+38F

float myFmin(float a, float b) {
    return (a < b) ? a : b;
}

float myFmax(float a, float b) {
    return (a > b) ? a : b;
}

vec3 minVec3(vec3 a, vec3 b) {
    return vec3(
        (a.x < b.x) ? a.x : b.x,
        (a.y < b.y) ? a.y : b.y,
        (a.z < b.z) ? a.z : b.z
    );
}

vec3 maxVec3(vec3 a, vec3 b) {
    return vec3(
        (a.x > b.x) ? a.x : b.x,
        (a.y > b.y) ? a.y : b.y,
        (a.z > b.z) ? a.z : b.z
    );
}


AABB getAABB(int objIndex) {
    vec3 minPt = vec3(FLT_MAX);
    vec3 maxPt = vec3(-FLT_MAX);

    for (int i = beginIndex[objIndex]; i < beginIndex[objIndex] + numVertices[objIndex]; ++i) {
        vec3 v = vec3(vertices[i].x, vertices[i].y, vertices[i].z);
        minPt = minVec3(minPt, v);
        maxPt = maxVec3(maxPt, v);
    }

    return { minPt, maxPt };
}

bool checkAABBCollision(const AABB& a, const AABB& b) {
    return (a.min.x <= b.max.x && a.max.x >= b.min.x) &&
           (a.min.y <= b.max.y && a.max.y >= b.min.y) &&
           (a.min.z <= b.max.z && a.max.z >= b.min.z);
}


// Shader uniform locations
GLuint ModelView, Projection, Color;

GLuint buffer;
GLuint normalBuffer;

GLuint program;        // Flat shader (picking için)
GLuint phongProgram;   // Phong shader (görselleştirme için)

// Apply physics update (gravity and collision)
void updatePhysics() {
    for (int i = 0; i < objectNumber; ++i) {
        if (objectNames[i] == "ground") continue;

        // Eğer yere yapıştıysa, yer çekimi hızını sıfırla
        if (isGrounded[i]) {
            velocities[i].y = 0;
        } else {
            velocities[i].y += GRAVITY;  // Yerçekimi uygula
        }

        vec3 originalCenter = centers[i];
        vec3 newCenter = originalCenter + velocities[i];

        AABB objBbox = getAABB(i);
        float objectBottom = objBbox.min.y + velocities[i].y;

        bool hitGround = (objectBottom <= -1.0f); // Yer seviyesi -1.0
        bool hitObject = false;

        if (!hitGround) {
            vec3 displacement = velocities[i];
            // Test pozisyonuna taşı
            for (int j = beginIndex[i]; j < beginIndex[i] + numVertices[i]; ++j) {
                vertices[j] = vertices[j] + vec4(displacement, 0.0);
            }
            centers[i] = newCenter;

            AABB currentAABB = getAABB(i);

            for (int j = 0; j < objectNumber; ++j) {
                if (i == j) continue;
                AABB otherAABB = getAABB(j);
                if (checkAABBCollision(currentAABB, otherAABB)) {
                    hitObject = true;
                    break;
                }
            }

            // Pozisyonu eski haline getir
            for (int j = beginIndex[i]; j < beginIndex[i] + numVertices[i]; ++j) {
                vertices[j] = vertices[j] - vec4(displacement, 0.0);
            }
            centers[i] = originalCenter;
        }

        if (hitGround || hitObject) {
            if (velocities[i].y < 0) {
                velocities[i].y = 0;
            }
            isGrounded[i] = true;
        } else {
            // Hareket uygula
            vec3 displacement = velocities[i];
            for (int j = beginIndex[i]; j < beginIndex[i] + numVertices[i]; ++j) {
                vertices[j] = vertices[j] + vec4(displacement, 0.0);
            }
            centers[i] = newCenter;
            isGrounded[i] = false;

            // Vertex buffer güncelle
            glBindBuffer(GL_ARRAY_BUFFER, buffer);
            glBufferData(GL_ARRAY_BUFFER, (vertices.size() * sizeof(point4)) + (normals.size() * sizeof(vec3)), NULL, GL_STATIC_DRAW);
            glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(point4), vertices.data());
            glBufferSubData(GL_ARRAY_BUFFER, vertices.size() * sizeof(point4), normals.size() * sizeof(vec3), normals.data());
        }
    }
}


void addObject(vec3 center, float size, const std::string& objectType)
{
    objectNumber += 1;
    int start = vertices.size();
    // createCube(center, size, vertices);
    if (objectType == "cube") {
        createCube(center, size, vertices, normals);  // Add a cube
    } else if (objectType == "pyramid") {
        createPyramid(center, size, vertices, normals);  // Add a pyramid
    } else if (objectType == "rectangle") {
        createRectanglePrism(center, size, size, size*2, vertices, normals);  // Add a rectangle prism
    } else if (objectType == "half_cube") {
        createHalfCube(center, size, vertices, normals);   // Add a half cube
    } else if (objectType == "triangular_prism") {
        createTriangularPrism(center, size, vertices, normals);   // Add a triangular prism
    } else if (objectType == "cylinder") {
        createCylinder(center, size, size*2, vertices, normals);   // Add a cylinder
    } else if (objectType == "sphere") {
        createSphere(center, size, vertices, normals);   // Add a sphere
    } else if (objectType == "half_cylinder") {
        createHalfCylinderPrism(center, size, size*2, vertices, normals);   // Add a half cylinder
    } else if (objectType == "bridge") {
        createBridgeShape(center, size*2, size*1.5, size*2, size, vertices, normals);
    } else if (objectType == "ground") {
        // Y düzleminde (XZ üzerinde) büyük bir kare
        createGround(center, size*2, vertices, normals);
    }
    
    
    else {
        std::cout << "Unknown object type: " << objectType << std::endl;  // Error handling for unknown object types
        return;
    }
    
    centers.push_back(center);  // Add the center of the cube to the centers vector
    beginIndex.push_back(start);
    numVertices.push_back(vertices.size() - start);
    if (objectType == "ground") {
        objectColors.push_back(color4(0.5, 0.5, 0.5, 1.0));
    } else {
        objectColors.push_back(color4(1.0, 0.0, 0.0, 1.0));
    }
    objectNames.push_back(objectType);

    // Initialize physics properties for the new object
    velocities.push_back(vec3(0.0, 0.0, 0.0)); // Start with zero velocity
    isGrounded.push_back(objectType == "ground"); // Ground objects start grounded
    
    // Update the buffer with new vertex positions
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, (vertices.size() * sizeof(point4)) + (normals.size() * sizeof(vec3)),
          NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(point4), vertices.data());
    glBufferSubData( GL_ARRAY_BUFFER, vertices.size() * sizeof(point4),
                    normals.size() * sizeof(vec3), normals.data() );


}


mat4 M = mat4(1.0); // needed for shadow projection
point4 light_position( 0.0, 1.0, 0.25, 0.0 ); // light source


void setUpPhongShader(){
    phongProgram = InitShader("vshader_phong.glsl", "fshader_phong.glsl");
    glUseProgram(phongProgram);

    GLuint vPosition = glGetAttribLocation(phongProgram, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    
    GLuint vNormal = glGetAttribLocation( phongProgram, "vNormal" );
    glEnableVertexAttribArray( vNormal );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0,
               BUFFER_OFFSET(vertices.size() * sizeof(point4)) );
    
    /*
    GLuint vNormal = glGetAttribLocation(phongProgram, "vNormal");
    glEnableVertexAttribArray(vNormal);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
     */
    
    // Initialize shader lighting parameters
   
    color4 light_ambient( 0.5, 0.5, 0.5, 1.0 ); // L_a
    color4 light_diffuse( 1.0, 1.0, 1.0, 1.0 ); // L_d
    color4 light_specular( 1.0, 1.0, 1.0, 1.0 ); // L_s

    color4 material_ambient( 1.0, 0.0, 1.0, 1.0 ); // k_a
    color4 material_diffuse( 1.0, 0.0, 0.0, 1.0 ); // k_d
    color4 material_specular( 1.0, 0.8, 0.0, 1.0 ); // k_s
    float  material_shininess = 100.0;

    color4 ambient_product = light_ambient * material_ambient; // k_a * L_a
    color4 diffuse_product = light_diffuse * material_diffuse; // k_d * L_d
    color4 specular_product = light_specular * material_specular; // k_s * L_s
    glUniform4fv( glGetUniformLocation(phongProgram, "AmbientProduct"),1, ambient_product );
    glUniform4fv( glGetUniformLocation(phongProgram, "DiffuseProduct"),1, diffuse_product );
    glUniform4fv( glGetUniformLocation(phongProgram, "SpecularProduct"),1, specular_product );
    glUniform4fv( glGetUniformLocation(phongProgram, "LightPosition"),1, light_position );
    glUniform1f( glGetUniformLocation(phongProgram, "Shininess"),material_shininess );
    //

    ModelView = glGetUniformLocation(phongProgram, "ModelView");
    Projection = glGetUniformLocation(phongProgram, "Projection");
    Color = glGetUniformLocation(phongProgram, "color");

   // mat4 projection = Ortho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    mat4 projection = Perspective(45.00, 1.0, 0.1, 10.0);
    glUniformMatrix4fv(Projection, 1, GL_TRUE, projection);

    mat4 model_view = mat4(1.0);
    glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view);
}

void setUpShader(){
    // Use flat shader
    if (program == 0) { // Initialize once if not already
        program = InitShader("vshader.glsl", "fshader.glsl");
    }
    glUseProgram(program);

    // Re-bind vPosition
    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    // Set uniforms again for this shader
    ModelView = glGetUniformLocation(program, "ModelView");
    Projection = glGetUniformLocation(program, "Projection");
    Color = glGetUniformLocation(program, "color");

    mat4 projection = Perspective(45.0, 1.0, 1.0, 50.0); // ya da display ile aynı
    mat4 model_view = (Translate(0.0, 0.0, -3.0) *
                       Translate(0.0, 0.0, 0.0) * // displacement
                       Scale(1.0, 1.0, 1.0) *
                       RotateX(Theta[Xaxis]) *
                       RotateY(Theta[Yaxis]) *
                       RotateZ(Theta[Zaxis]) );
    glUniformMatrix4fv(Projection, 1, GL_TRUE, projection);
    glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view);
}

// Initialization
void init()
{
    /*
    float groundY = -1.0;
    M = mat4(
             1.0, 0.0, 0.0, 0.0,
             0.0, 1.0, 0.0, 0.0,
             0.0, 0.0, 1.0, 0.0,
             0.0, -1.0/groundY, 0.0, 0.0
    );
     */

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    
    // Create and initialize a buffer object
    glGenBuffers( 1, &buffer );
    
    // Create objects
    addObject(vec3(-0.85, 0.0, 0.0), 0.25, "cube"); // birinci küp
    addObject(vec3(0.85, 0.0, 0.0f), 0.25, "pyramid"); // Bir piramit ekler
    addObject(vec3(0.0, 0.60, 0.0f), 0.25, "rectangle"); // Bir piramit ekler
    addObject(vec3(0.0, -0.60, 0.0f), 0.25, "half_cube");
   // addObject(vec3(0.40, 0.0, 0.0f), 0.25, "triangular_prism");
   
    
    // addObject(vec3(-0.60, 0.0, 0.0f), 0.125, "cylinder");
   // addObject(vec3(0.70, 0.70, 0.0f), 0.125, "sphere");
    
    addObject(vec3(0.0, -1.0, 0.0f), 3.0, "ground"); // geniş bir zemin objesi

    setUpPhongShader();
    

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);  // <-- stencil buffer FOR SEND SHADOW ALWAYS BEHIND OBJECT
    
    glClearColor(1.0, 1.0, 1.0, 1.0);
    
 
}


// Display function
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
    // Update physics (gravity and collision)
    updatePhysics();
    
    const vec3 displacement(0.0, 0.0, 0.0);
    mat4 model_view = (Translate(0.0, 0.0, -3.0) * // Kamerayı geri çek (for perspective only)
                       Translate(displacement) * Scale(1.0, 1.0, 1.0) *
                        RotateX(Theta[Xaxis]) *
                        RotateY(Theta[Yaxis]) *
                        RotateZ(Theta[Zaxis]) );

    glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view);

    // FIRST ALL OBJECTS EXCEPT GROUND
    for (int i = 0; i < objectNumber; ++i) {
        if (objectNames[i] != "ground") {
            
            // Normal objeler stencil testi yapsın ama hep geçsin (stencil buffer etkilenmesin)
            glStencilFunc(GL_ALWAYS, 0, 0xFF);
            glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
            glStencilMask(0x00);  // stencil buffer yazılmaz

            setUpPhongShader();
            glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view);

            // Phong parametreleri...
            color4 diffuse = objectColors[i];
            color4 ambient = color4(0.4, 0.4, 0.4, 1.0) * diffuse;
            color4 specular = objectColors[i];
            float shininess = 100.0;
            glUniform4fv(glGetUniformLocation(phongProgram, "AmbientProduct"), 1, ambient);
            glUniform4fv(glGetUniformLocation(phongProgram, "DiffuseProduct"), 1, diffuse);
            glUniform4fv(glGetUniformLocation(phongProgram, "SpecularProduct"), 1, specular);
            glUniform1f(glGetUniformLocation(phongProgram, "Shininess"), shininess);

            glDrawArrays(GL_TRIANGLES, beginIndex[i], numVertices[i]);
        }
    }
    
    // THEN GROUND (Because if ground drawed first then objects added with mouse (later after init) fall behinds shadow !!!)
    for (int i = 0; i < objectNumber; ++i) {
        if (objectNames[i] == "ground") {
            // Zemin için stencil 1 yaz
            glStencilFunc(GL_ALWAYS, 1, 0xFF);
            glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
            glStencilMask(0xFF);

            setUpPhongShader();
            glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view);

            // Phong parametreleri...
            color4 diffuse = objectColors[i];
            color4 ambient = color4(0.4, 0.4, 0.4, 1.0) * diffuse;
            color4 specular = objectColors[i];
            float shininess = 100.0;
            glUniform4fv(glGetUniformLocation(phongProgram, "AmbientProduct"), 1, ambient);
            glUniform4fv(glGetUniformLocation(phongProgram, "DiffuseProduct"), 1, diffuse);
            glUniform4fv(glGetUniformLocation(phongProgram, "SpecularProduct"), 1, specular);
            glUniform1f(glGetUniformLocation(phongProgram, "Shininess"), shininess);

            glDrawArrays(GL_TRIANGLES, beginIndex[i], numVertices[i]);
            
        }
    }
    

    // Sadece stencil == 1 (zemin) olan yerlere gölge çiz
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_EQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    glStencilMask(0x00);

    // Derinlik testi burada ya kapatılır ya da farklı ayarlanır


    for (int i = 0; i < objectNumber; ++i) {
        if (objectNames[i] == "ground") continue;

        setUpShader();
        vec4 plane = vec4(0.0, 1.0, 0.0, 1.0);
        float dot = plane.x * light_position.x +
                    plane.y * light_position.y +
                    plane.z * light_position.z +
                    plane.w * light_position.w;

        mat4 shadowMat = mat4(1.0);
        for (int r = 0; r < 4; ++r) {
            for (int c = 0; c < 4; ++c) {
                shadowMat[r][c] = dot * (r == c) - light_position[r] * plane[c];
            }
        }

        mat4 shadow_model_view = model_view * shadowMat;
        glUniformMatrix4fv(ModelView, 1, GL_TRUE, shadow_model_view);
        glUniform4f(Color, 0.0, 0.0, 0.0, 1.0);  // siyah gölge

        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(0.1, 0.1);
        glDrawArrays(GL_TRIANGLES, beginIndex[i], numVertices[i]);
        glDisable(GL_POLYGON_OFFSET_FILL);
    }

    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 0, 0xFF);



    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Reset
}



// Update the position of the selected object based on axis ('x', 'y', or 'z') and direction (+ or -)
void moveSelectedObject(char axis, int direction)
{
    // Check if a valid object is selected
    if (selectedObject >= 0 && selectedObject < objectColors.size()) {
        float increment = 0.05f * direction;

        // Move along the specified axis
        for (int i = beginIndex[selectedObject]; i < beginIndex[selectedObject] + numVertices[selectedObject]; ++i) {
            if (axis == 'x') { // X axis
                vertices[i].x += increment ;
            }
            else if (axis == 'y') { // Y axis
                vertices[i].y += increment;
            }
            else if (axis == 'z') { // Z axis
                vertices[i].z += increment;
            }
        }

        // Update center position
        if (axis == 'x') {
            centers[selectedObject].x += increment;
        } else if (axis == 'y') {
            centers[selectedObject].y += increment;
        } else if (axis == 'z') {
            centers[selectedObject].z += increment;
        }

        // Reset grounded state when object is moved manually
        if (selectedObject < isGrounded.size()) {
            isGrounded[selectedObject] = false;
        }

        // Update the buffer with new vertex positions
        glBindBuffer( GL_ARRAY_BUFFER, buffer );
        glBufferData( GL_ARRAY_BUFFER, (vertices.size() * sizeof(point4)) + (normals.size() * sizeof(vec3)),
              NULL, GL_STATIC_DRAW );
        glBufferSubData( GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(point4), vertices.data());
        glBufferSubData( GL_ARRAY_BUFFER, vertices.size() * sizeof(point4),
                        normals.size() * sizeof(vec3), normals.data() );
    }
}

char axis = 'a';

// Scale a selected object while keeping its center fixed
void scaleObject(int selectedObject, float scaleFactor, std::vector<point4>& vertices, const std::vector<vec3>& centers, char axis)
{
    // Check if the selectedObject index is valid
    if (selectedObject >= 0 && selectedObject < centers.size()) {
        // Get the center of the selected object
        vec3 center = centers[selectedObject];

        // Scale all the vertices of the selected object
        for (int i = beginIndex[selectedObject]; i < beginIndex[selectedObject] + numVertices[selectedObject]; ++i) {
            // Get the current vertex position
            point4& vertex = vertices[i];
            
            if (axis == 'x') { // X axis
                vertex.x = center.x + (vertex.x - center.x) * scaleFactor;
            }
            else if (axis == 'y') { // Y axis
                vertex.y = center.y + (vertex.y - center.y) * scaleFactor;
            }
            else if (axis == 'z') { // Z axis
                vertex.z = center.z + (vertex.z - center.z) * scaleFactor;
            } else {
                // Apply the scaling while keeping the center fixed
                vertex.x = center.x + (vertex.x - center.x) * scaleFactor;
                vertex.y = center.y + (vertex.y - center.y) * scaleFactor;
                vertex.z = center.z + (vertex.z - center.z) * scaleFactor;
            }

           
        }

        // Update the buffer with new vertex positions
        glBindBuffer( GL_ARRAY_BUFFER, buffer );
        glBufferData( GL_ARRAY_BUFFER, (vertices.size() * sizeof(point4)) + (normals.size() * sizeof(vec3)),
              NULL, GL_STATIC_DRAW );
        glBufferSubData( GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(point4), vertices.data());
        glBufferSubData( GL_ARRAY_BUFFER, vertices.size() * sizeof(point4),
                        normals.size() * sizeof(vec3), normals.data() );
    }
}

void rotateObject(int index, float rotationX, float rotationY, std::vector<point4>& vertices, std::vector<vec3>& centers)
{
    // Objenin merkezini al
    vec3 center = centers[index];

    // Çevirme işlemi: objeyi merkeze getirmek için, merkezden uzaklaştırıyoruz
    mat4 translationToOrigin = Translate(-center.x, -center.y, -center.z);  // Merkeze taşı
    mat4 translationBack = Translate(center.x, center.y, center.z);  // Geri merkeze taşı
    
    /*
    // Dönüş matrislerini oluşturuyoruz
    mat4 rotationMatrixX = RotateX(rotationX);  // X ekseninde dönüş
    mat4 rotationMatrixY = RotateY(rotationY);  // Y ekseninde dönüş

    // Dönüşüm işlemi: İlk olarak merkeze taşı, sonra dönüş işlemi yap, son olarak tekrar merkeze geri koy
    for (int i = beginIndex[index]; i < beginIndex[index] + numVertices[index]; ++i) {
        // Çevirme ve dönüş işlemlerini sırayla uygula
        vertices[i] = translationBack * rotationMatrixY * rotationMatrixX * translationToOrigin * vertices[i];
    }
    */
    
    mat4 rotationMatrix = mat4(1.0f);
    const float angle = 1.0f; // Her seferinde 30 derece döndür

    // Seçilen eksene göre dönüş matrisini belirle
    switch (axis) {
        case 'x':
            rotationMatrix = RotateX(angle);
            break;
        case 'y':
            rotationMatrix = RotateY(angle);
            break;
        case 'z':
            rotationMatrix = RotateZ(angle);
            break;
        default:
            return; // Geçersiz eksen, hiçbir şey yapma
    }
    
    mat4 modelMatrix = translationBack * rotationMatrix * translationToOrigin;  // tanımla

    // vertices için dönüşüm
    for (int i = beginIndex[index]; i < beginIndex[index] + numVertices[index]; ++i) {
        vertices[i] = modelMatrix * vertices[i];
    }

    // Senin elle transpose ettiğin 3x3 alt matris
    mat3 normalMatrix(
        vec3(modelMatrix[0].x, modelMatrix[1].x, modelMatrix[2].x),
        vec3(modelMatrix[0].y, modelMatrix[1].y, modelMatrix[2].y),
        vec3(modelMatrix[0].z, modelMatrix[1].z, modelMatrix[2].z)
    );

    // Normalleri dönüştür
    for (int i = beginIndex[index]; i < beginIndex[index] + numVertices[index]; ++i) {
        normals[i] = normalize(normalMatrix * normals[i]);
    }


    
    // Update the buffer with new vertex positions
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, (vertices.size() * sizeof(point4)) + (normals.size() * sizeof(vec3)),
          NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(point4), vertices.data());
    glBufferSubData( GL_ARRAY_BUFFER, vertices.size() * sizeof(point4),
                    normals.size() * sizeof(vec3), normals.data() );
}

void deleteSelectedObject() {
    if (selectedObject >= 0 && selectedObject < objectNumber) {
        // Objeye ait verileri sil
        vertices.erase(vertices.begin() + beginIndex[selectedObject],
                       vertices.begin() + beginIndex[selectedObject] + numVertices[selectedObject]);
        
        normals.erase(normals.begin() + beginIndex[selectedObject],
                              normals.begin() + beginIndex[selectedObject] + numVertices[selectedObject]);

        
        centers.erase(centers.begin() + selectedObject);
        numVertices.erase(numVertices.begin() + selectedObject);
        objectColors.erase(objectColors.begin() + selectedObject);
        beginIndex.erase(beginIndex.begin() + selectedObject);
        objectNames.erase(objectNames.begin() + selectedObject);

        // Remove physics properties
        if (selectedObject < velocities.size()) {
            velocities.erase(velocities.begin() + selectedObject);
        }
        if (selectedObject < isGrounded.size()) {
            isGrounded.erase(isGrounded.begin() + selectedObject);
        }

        objectNumber--;

        // BeginIndex değerlerini güncelle
        for (int i = selectedObject; i < beginIndex.size(); ++i) {
            if (i == 0)
                beginIndex[i] = 0;
            else
                beginIndex[i] = beginIndex[i-1] + numVertices[i-1];
        }

        // Update the buffer with new vertex positions
        glBindBuffer( GL_ARRAY_BUFFER, buffer );
        glBufferData( GL_ARRAY_BUFFER, (vertices.size() * sizeof(point4)) + (normals.size() * sizeof(vec3)),
              NULL, GL_STATIC_DRAW );
        glBufferSubData( GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(point4), vertices.data());
        glBufferSubData( GL_ARRAY_BUFFER, vertices.size() * sizeof(point4),
                        normals.size() * sizeof(vec3), normals.data() );

        selectedObject = -1; // Seçimi iptal et
        printf("Object deleted!\n"); // Bilgi mesajı
        
        setUpPhongShader();
    }
}



// Variables to store the previous mouse position for tracking the drag direction
double prevX = 0.0, prevY = 0.0;
bool isDragging = false;
bool isStable = true;

double posX = 0.0, posY = 0.0;
// Mouse callback for picking and selecting area
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        setUpShader();

        // Render cubes with unique colors for picking
        // Her bir küpü render etmek için döngü
        for (int i = 0; i < objectNumber; ++i) {
            // Küpün renklerini ayarlayın (örneğin, her küp farklı renklerde)
            float r = (i % 256) / 255.0f;
            float g = ((i / 256) % 256) / 255.0f;
            float b = (i / (256 * 256)) / 255.0f;

        //    printf("Assigned colors: %f %f %f \n", r, g, b);

            glUniform4f(Color, r, g, b, 1.0f); // Küpün rengini ayarla

            // Bu küp için doğru vertex aralığını kullanarak çizim yap
            glDrawArrays(GL_TRIANGLES, beginIndex[i], numVertices[i]);
        }

        glFlush();

        double x, y;
        glfwGetCursorPos(window, &x, &y);

        int fb_width, fb_height;
        glfwGetFramebufferSize(window, &fb_width, &fb_height);
        int win_width, win_height;
        glfwGetWindowSize(window, &win_width, &win_height);

        x *= (fb_width / (float)win_width);
        y *= (fb_height / (float)win_height);
        y = fb_height - y;
        
        posX = x;
        posY = y;

        unsigned char pixel[4];
        glReadPixels((int)x, (int)y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixel);
        
    //    printf("Selected colors: %f %f %f \n", pixel[0], pixel[1], pixel[2]);
        
        int selectedColor = pixel[0] + (pixel[1] << 8) + (pixel[2] << 16);

        if(selectedColor < 0 || selectedColor >= objectNumber){
            selectedObject = -1;
        } else {
            selectedObject = selectedColor;
        }
        
        // NOT INCLUDE GROUND OBJECT
        if (selectedObject >= 0 && objectNames[selectedObject] == "ground") {
            selectedObject = -1;
        }
        
     //   printf("Object selected: %d\n", selectedObject);
        
        isDragging = true;
        isStable = true;
        
        glUseProgram(phongProgram); // Picking sonrası tekrar Phong'a geç
        setUpPhongShader();
        
    } else if (action == GLFW_RELEASE) {
        isDragging = false;

        // Clicked without move
        if (isStable) {
            printf("Mouse clicked, but not dragged!\n");

        } else {
            printf("DRAG!\n");
        }
    }
}

bool isScaling = false;
bool isRotating = false;


// Mouse movement callback to handle rotation while dragging
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
   // printf("INSIDE cursor_position_callback \n");
    
    if (isDragging) {
        double dx = xpos - prevX; // Horizontal movement (left-right)
        double dy = ypos - prevY; // Vertical movement (up-down)
        
        if(abs(dx) > 1 or abs(dy) > 1){
            isStable = false;
        }
        
        // Update the rotation angles based on mouse movement
        Theta[Yaxis] += dx * 0.2; // Scale factor for rotation speed (adjust if needed)
        Theta[Xaxis] += dy * 0.2; // Scale Y-axis for natural mouse behavior

        // Ensure Theta stays within 0-360 degrees
        if (Theta[Xaxis] > 360.0) Theta[Xaxis] -= 360.0;
        if (Theta[Xaxis] < 0.0) Theta[Xaxis] += 360.0;

        if (Theta[Yaxis] > 360.0) Theta[Yaxis] -= 360.0;
        if (Theta[Yaxis] < 0.0) Theta[Yaxis] += 360.0;
    }
    
    if (isScaling) {
        // Hareket farkı
        double deltaX = xpos - prevX;
        double deltaY = ypos - prevY;

        // Ölçekleme oranını ayarlıyoruz, burada deltaY'yi kullanarak ölçeklendiriyoruz
        float scaleFactor = 1.0f + deltaY * 0.01f; // Dikey hareketle ölçeklendirme (daha hassas)

        if (selectedObject >= 0 && selectedObject < objectColors.size()) {
            scaleObject(selectedObject, scaleFactor, vertices, centers, axis);
        }

    }
    
    if (isRotating) {
        // Hareket farkları
        double deltaX = xpos - prevX;
        double deltaY = ypos - prevY;

        // Dönüş açısını hesaplıyoruz
        float rotationFactorX = deltaY * 0.1f; // Dikey hareketle X ekseninde döndür
        float rotationFactorY = deltaX * 0.1f; // Yatay hareketle Y ekseninde döndür

        if (selectedObject >= 0 && selectedObject < objectColors.size()) {
            rotateObject(selectedObject, rotationFactorX, rotationFactorY, vertices, centers);
        }

    }

    // Update the previous position for next move
    prevX = xpos;
    prevY = ypos;
}

void addObjectAtMouse(GLFWwindow* window, float size, const std::string& shapeType)
{
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    int width, height;
    glfwGetWindowSize(window, &width, &height);

    // Fare konumunu dünya koordinatlarına dönüştür
    vec3 newCenter = vec3(
        (xpos - width / 2) / (width / 2),
        (height / 2 - ypos) / (height / 2),
        0.0
    );

    addObject(newCenter, size, shapeType);
  //  setUpPhongShader();
}


const char* helpText =
"\n=== Rubik's Cube Help ===\n"
"Mouse Controls:\n"
" - Hold left mouse button and move to rotate the cube.\n"
"\nKeyboard Controls:\n"
" - H: Show this help message\n"
" - Q or ESC: Quit the program\n"
" - 1: Create Cube\n"
" - 2: Create Pyramid\n"
" - 3: Create Rectangle Prism\n"
" - 4: Create Half Cube\n"
" - 5: Create Triangular Prism\n"
" - 6: Create Cylinder\n"
" - 7: Create Sphere\n"
" - 8: Create Half Cylinder\n"
" - 9: Create Bridge\n"
" - R: Set selected object color to Red\n"
" - G: Set selected object color to Green\n"
" - B: Set selected object color to Blue\n"
" - M: Set selected object color to Magenta\n"
" - Y: Set selected object color to Yellow\n"
" - C: Set selected object color to Cyan\n"
" - W: Set selected object color to White\n"
" - Arrow Up: Move selected object up (positive Y direction)\n"
" - Arrow Down: Move selected object down (negative Y direction)\n"
" - Arrow Right: Move selected object right (positive X direction)\n"
" - Arrow Left: Move selected object left (negative X direction)\n"
" - Enter: Move selected object forward (positive Z direction)\n"
" - Tab: Move selected object backward (negative Z direction)\n"
" - S: Activate scaling mode\n"
" - O: Activate rotation mode\n"
" - D: Delete selected object\n"
" - A: Choose axis (all)\n"
" - Z: Choose axis (z)\n"
" - X: Choose axis (x)\n"
" - E: Choose axis (y)\n"
"=========================\n\n";


// Specify what to do when a keyboard event happens, i.e., when the user presses or releases a key
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_ESCAPE:
                isScaling = false;
                isRotating = false;
                break;
                
            case GLFW_KEY_Q:
                exit(EXIT_SUCCESS);
                break;
                
            case GLFW_KEY_H:
                printf("%s", helpText);
                break;
                
            // COLORS
            case GLFW_KEY_R: // Red
               if (selectedObject >= 0 && selectedObject < objectColors.size())
                   objectColors[selectedObject] = color4(1.0, 0.0, 0.0, 1.0);
               break;

           case GLFW_KEY_G: // Green
               if (selectedObject >= 0 && selectedObject < objectColors.size())
                   objectColors[selectedObject] = color4(0.0, 1.0, 0.0, 1.0);
               break;

           case GLFW_KEY_B: // Blue
               if (selectedObject >= 0 && selectedObject < objectColors.size())
                   objectColors[selectedObject] = color4(0.0, 0.0, 1.0, 1.0);
               break;

           case GLFW_KEY_M: // Magenta
               if (selectedObject >= 0 && selectedObject < objectColors.size())
                   objectColors[selectedObject] = color4(1.0, 0.0, 1.0, 1.0);
               break;

           case GLFW_KEY_Y: // Yellow
               if (selectedObject >= 0 && selectedObject < objectColors.size())
                   objectColors[selectedObject] = color4(1.0, 1.0, 0.0, 1.0);
               break;
                
            case GLFW_KEY_C: // Cyan
                if (selectedObject >= 0 && selectedObject < objectColors.size())
                    objectColors[selectedObject] = color4(0.0, 1.0, 1.0, 1.0);
                break;
                
            case GLFW_KEY_W: // White
                if (selectedObject >= 0 && selectedObject < objectColors.size())
                    objectColors[selectedObject] = color4(1.0, 1.0, 1.0, 1.0);
                break;
                
            // TRANSFORMATIONS
            case GLFW_KEY_UP: // MOVE selected object up (positive Y direction)
                moveSelectedObject('y', 1);
                break;

            case GLFW_KEY_DOWN: // MOVE selected object down (negative Y direction)
                moveSelectedObject('y', -1);
                break;

            case GLFW_KEY_RIGHT: // MOVE selected object right (positive X direction)
                moveSelectedObject('x', 1);
                break;

            case GLFW_KEY_LEFT: // MOVE selected object left (negative X direction)
                moveSelectedObject('x', -1);
                break;
            
            case GLFW_KEY_ENTER: // MOVE selected object forward (positive Z direction)
                moveSelectedObject('z', 1);
                break;

            case GLFW_KEY_TAB: // MOVE selected object backward (negative Z direction)
                moveSelectedObject('z', -1);
                break;
                
            case GLFW_KEY_S: // SCALE
                isScaling = true; // put scaling mode in cursor_position_callback
                break;
                
            case GLFW_KEY_O: // ROTATE
                isRotating = true; // put scaling mode in cursor_position_callback
                break;
                
            case GLFW_KEY_D: // DELETE
                deleteSelectedObject();
                break;
                
            case GLFW_KEY_A: // Choose axis
                axis = 'a';
                break;
            
            case GLFW_KEY_Z: // Choose axis
                axis = 'z';
                break;
                
            case GLFW_KEY_X: // Choose axis
                axis = 'x';
                break;
                
            case GLFW_KEY_E: // Choose axis
                axis = 'y';
                break;
            
            // CREATE OBJECTS
            case GLFW_KEY_1: // CREATE cube
                addObjectAtMouse(window, 0.25f, "cube");
                break;
                
            case GLFW_KEY_2: // CREATE pyramid
                addObjectAtMouse(window, 0.25f, "pyramid");
                break;
            
            case GLFW_KEY_3: // CREATE rectangle
                addObjectAtMouse(window, 0.25f, "rectangle");
                break;
            
            case GLFW_KEY_4: // CREATE half_cube
                addObjectAtMouse(window, 0.25f, "half_cube");
                break;
            
            case GLFW_KEY_5: // CREATE triangular_prism
                addObjectAtMouse(window, 0.25f, "triangular_prism");
                break;
            
            case GLFW_KEY_6: // CREATE cylinder
                addObjectAtMouse(window, 0.125f, "cylinder");
                break;
                
            case GLFW_KEY_7: // CREATE sphere
                addObjectAtMouse(window, 0.125f, "sphere");
                break;
                
            case GLFW_KEY_8: // CREATE half_cylinder
                addObjectAtMouse(window, 0.125f, "half_cylinder");
                break;
                
            case GLFW_KEY_9: // CREATE bridge
                addObjectAtMouse(window, 0.125f, "bridge");
                break;
                
        }

        // Açılar 0-360 arasında kalsın
        for (int i = 0; i < 3; ++i) {
            if (Theta[i] < 0.0) Theta[i] += 360.0;
            if (Theta[i] > 360.0) Theta[i] -= 360.0;
        }
    }
}

// Main
int main(int argc, char** argv)
{
    if (!glfwInit()) exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
#ifdef __APPLE__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#else
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
#endif
    
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(800, 800, "Building Blocks", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetKeyCallback(window, key_callback);

#ifndef __APPLE__
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW\n";
        return -1;
    }
#endif

    // Print OpenGL version for debugging
    printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
    printf("GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    init();

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        display();
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
