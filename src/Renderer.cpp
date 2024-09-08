#include "Renderer.h"
#include <GL/glew.h>
#include "Config.h"
#include <iostream>

Renderer::Renderer() {}

Renderer::~Renderer() {}

void Renderer::InitOpenGL() 
{
    // Set background clear color and enable depth test
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);  // Dark green background
    glEnable(GL_DEPTH_TEST);
}

void Renderer::RenderMap(const Map& map, const Camera& camera) 
{
    auto& renderBuffer = map.GetRenderBuffer();
    //look at render buffer
    glm::vec2 cameraCoords = camera.GetCoords();
    float zoomLevel = camera.GetZoomLevel();
    int rotation = camera.GetRotation();

    // Loop through the 3x3 buffer and render each chunk
    for (int y = 0; y < MapConfig::CHUNKS_TO_RENDER_GRID_SIZE; ++y) 
    {
        for (int x = 0; x < MapConfig::CHUNKS_TO_RENDER_GRID_SIZE; ++x) 
        {
            if (renderBuffer[y][x]) 
            {
                RenderChunk(*renderBuffer[y][x], cameraCoords, zoomLevel, rotation);
            }
        }
    }
}

void Renderer::RenderChunk(const Chunk& chunk, const glm::vec2& cameraCoords, const float& zoomLevel, const int rotation)
{
    float tileSize = zoomLevel;   // Width of the tile

    // Get the chunk and its tiles
    const std::vector<Tile>& tiles = chunk.GetTiles();

    // Render all tiles in the chunk
    for (const Tile& tile : tiles) 
    {

        // Adjust the world coordinates based on the camera rotation
        float rotatedX, rotatedY;

        switch (rotation) {
            case 0:  // 0° (default)
                rotatedX = tile.tileCoords.x;
                rotatedY = tile.tileCoords.y;
                break;
            case 1:  // 90° clockwise
                rotatedX = tile.tileCoords.y;
                rotatedY = -tile.tileCoords.x;
                break;
            case 2:  // 180° (flipped)
                rotatedX = -tile.tileCoords.x;
                rotatedY = -tile.tileCoords.y;
                break;
            case 3:  // 270° clockwise
                rotatedX = -tile.tileCoords.y;
                rotatedY = tile.tileCoords.x;
                break;
        }

        // Adjust for the camera's position
        float screenX = rotatedX - cameraCoords.x;
        float screenY = rotatedY - cameraCoords.y;
        
        // Convert grid coordinates to isometric screen coordinates
        float isoX = (screenX - screenY) * (tileSize / 2);
        float isoY = (screenX + screenY) * (tileSize / 2);

        // Render the tile at the calculated isometric screen position
        RenderTile(tile, isoX, isoY, tileSize);
    }
}

void Renderer::RenderTile(const Tile& tile, float isoX, float isoY, float tileSize)
{
    glBegin(GL_QUADS);  // Use OpenGL's immediate mode for simplicity

    // Set the color (for now, all tiles are the same color)
    float color = tile.height / 10.0f;  // Normalize height to a color value
    glColor3f(0.0f, color, 0.0f);  // Green color for the tile

    // Define the 4 vertices of the square (in screen coordinates)
    glVertex2f(isoX, isoY - (tileSize / 2));                // Top
    glVertex2f(isoX + (tileSize / 2), isoY);                // Right
    glVertex2f(isoX, isoY + (tileSize / 2));                // Bottom
    glVertex2f(isoX - (tileSize / 2), isoY);                // Left

    glEnd();
}
