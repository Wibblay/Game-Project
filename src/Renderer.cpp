#include "Renderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Config.h"
#include <iostream>

Renderer::Renderer() : tileVAO(0), tileVBO(0), tileInstanceVBO(0), tileEBO(0),
                        tileShaderProgram("build/shaders/tile_vertex_shader.glsl", "build/shaders/fragment_shader.glsl") 
{
    Renderer::isoMatrix = glm::mat2(0.707f, 0.354f, -0.707f, 0.354f);
    Renderer::rotationMatrices = {glm::mat2(1.0f, 0.0f, 0.0f, 1.0f),
                                glm::mat2(0.0f, -1.0f, 1.0f, 0.0f),
                                glm::mat2(-1.0f, 0.0f, 0.0f, -1.0f),
                                glm::mat2(0.0f, 1.0f, -1.0f, 0.0f)};
}

Renderer::~Renderer() 
{
    // Clean up OpenGL resources
    glDeleteBuffers(1, &tileVBO);
    glDeleteBuffers(1, &tileInstanceVBO);
    glDeleteBuffers(1, &tileEBO);
    glDeleteVertexArrays(1, &tileVAO);
}

void Renderer::InitOpenGL() 
{
    // Set background clear color and enable depth test
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);  

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Setup shaders and buffers
    SetupBuffers();
}

void Renderer::SetupBuffers() 
{
    // Define the geometry of a single tile (centered at origin)

    // x, y, verticalShift, colour
    float tileVertices[] = 
    {
        // Left
        0.0f, -0.354f, 0.0f, 0.0f,  // Bottom
        -0.707f, 0.0f, 0.0f, 0.0f,  // Bottom Left
        0.0f, -0.354f, 1.0f, 0.0f,  // Top
        -0.707f, 0.0f, 1.0f, 0.0f,  // Top Left

        // Right
        0.707f, 0.0f, 0.0f, 0.0f,  // Bottom right
        0.707f, 0.0f, 1.0f, 0.0f,  // Top right

        //Top
        0.0f,  0.354f, 1.0f, 1.0f,  // Top
        0.707f,  0.0f, 1.0f, 1.0f,  // Right
        0.0f,  -0.354f, 1.0f, 1.0f,  // Bottom
        -0.707f,  0.0f, 1.0f, 1.0f   // Left
    };

    unsigned int tileIndices[] = 
    {
        // Left Side Face
        0, 1, 2,
        2, 1, 3,
        // Right Side Face
        4, 5, 0,
        0, 2, 5,
        // Top face
        6, 7, 8,
        8, 9, 6
    };

    //Side face buffers

    // Generate and bind VAO and VBO
    glGenVertexArrays(1, &tileVAO);
    glGenBuffers(1, &tileVBO);
    glGenBuffers(1, &tileEBO);

    // Bind VAO
    glBindVertexArray(tileVAO);

    // Bind VBO and upload vertex data
    glBindBuffer(GL_ARRAY_BUFFER, tileVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tileVertices), tileVertices, GL_STATIC_DRAW);

    // Bind EBO and upload index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tileEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tileIndices), tileIndices, GL_STATIC_DRAW);

    // Define vertex attribute for position (location = 0)
    GLsizei stride = 4 * sizeof(float);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);

    // Define vertical shift flag (location = 4)
    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(4);

    // Define side face colour flag (location = 5)
    glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(5);

    // Generate instance VBO for per-instance data
    glGenBuffers(1, &tileInstanceVBO);

    glBindVertexArray(0);
}

void Renderer::RenderMap(Map& map, const Camera& camera) 
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    std::vector<TileRenderData>& tileData = map.CollectTileRenderData(camera);
    if (tileData.empty()) 
    {
        std::cout << "No data to render" << std::endl;
        return;  // Nothing to render
    }

    UpdateInstanceBuffer(tileData, tileVAO, tileInstanceVBO);

    glm::mat4 projection = ComputeProjectionMatrix(GlobalConfig::DEFAULT_WINDOW_WIDTH, GlobalConfig::DEFAULT_WINDOW_HEIGHT);

    float tileSize = camera.GetZoomLevel();
    int rotation = camera.GetRotation();
    glm::mat2 rotationMatrix = rotationMatrices[rotation];
    glm::mat2 transformMatrix = isoMatrix * rotationMatrix;
    glm::vec2 cameraPosition = camera.GetCoords();

    glBindVertexArray(tileVAO);
    tileShaderProgram.use();

    // Set uniforms
    tileShaderProgram.setMat4("projection", projection);
    tileShaderProgram.setMat2("transformMatrix", transformMatrix);
    tileShaderProgram.setFloat("tileSize", tileSize);
    tileShaderProgram.setVec2("cameraPosition", cameraPosition);

    tileShaderProgram.setBool("isWireframe", false);

    // Draw side faces using glDrawElementsInstanced
    glDrawElementsInstanced(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0, static_cast<GLsizei>(tileData.size()));

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Optionally set the line width
    glLineWidth(1.5f);

    // Disable depth writes to prevent z-fighting
    glDepthMask(GL_FALSE);

    // Set isWireframe to true and set outline color
    tileShaderProgram.setBool("isWireframe", true);
    tileShaderProgram.setVec3("outlineColor", glm::vec3(0.6f, 0.6f, 0.6f)); // Black outline

    // Draw only the top faces in wireframe
    // The indices for the top faces start at offset = 12 * sizeof(GLuint)
    //glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(12 * sizeof(GLuint)), static_cast<GLsizei>(tileData.size()));
    glDrawElementsInstanced(GL_TRIANGLES, 18, GL_UNSIGNED_INT, (void*)(0), static_cast<GLsizei>(tileData.size()));

    // Re-enable depth writes
    glDepthMask(GL_TRUE);

    // Reset polygon mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glBindVertexArray(0);
    glUseProgram(0);
    
}

void Renderer::UpdateInstanceBuffer(const std::vector<TileRenderData>& tileData, const int& VAO, const int& instanceVBO) 
{
    // Bind instance VBO
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);

    // Upload instance data
    glBufferData(GL_ARRAY_BUFFER, tileData.size() * sizeof(TileRenderData), tileData.data(), GL_DYNAMIC_DRAW);

    // Bind VAO to set vertex attributes
    glBindVertexArray(VAO);

    // Define instance attribute for tile position (location = 1)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(TileRenderData), (void*)offsetof(TileRenderData, tileCoords));
    glVertexAttribDivisor(1, 1);  // Advance per instance

    // Define instance attribute for height (location = 2)
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(TileRenderData), (void*)offsetof(TileRenderData, height));
    glVertexAttribDivisor(2, 1);

    // Define instance attribute for color (location = 3)
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(TileRenderData), (void*)offsetof(TileRenderData, color));
    glVertexAttribDivisor(3, 1);

    // Unbind VAO and VBO
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

glm::mat4 Renderer::ComputeProjectionMatrix(int windowWidth, int windowHeight) {
    // Orthographic projection adjusted for isometric view
    float left = -windowWidth * 0.5f;
    float right = windowWidth * 0.5f;
    float bottom = -windowHeight * 0.5f;
    float top = windowHeight * 0.5f;
    float near = -1000.0f;
    float far = 1000.0f;

    return glm::ortho(left, right, bottom, top, near, far);
}
