#pragma once

#include <vector>
#include <unordered_map>
#include <memory>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Tile.h"
#include "Map.h"
#include "Camera.h"
#include "Shader.h"

class Renderer 
{
public:
    Renderer();
    ~Renderer();

    void InitOpenGL();                                    // Initialize OpenGL settings
    void RenderMap(Map& map, const Camera& camera); // Render the map based on the camera's position

private:
    void SetupBuffers();
    void UpdateInstanceBuffer(const std::vector<TileRenderData>& tileData, const int& VAO, const int& instanceVBO);

    glm::mat4 ComputeProjectionMatrix(int windowWidth, int windowHeight);

    GLuint tileVAO;
    GLuint tileVBO;
    GLuint tileInstanceVBO;
    GLuint tileEBO;

    Shader tileShaderProgram;

    glm::mat2 isoMatrix;
    std::vector<glm::mat2> rotationMatrices;
};
