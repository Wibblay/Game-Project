#ifndef RENDERER_H
#define RENDERER_H

#include "Map.h"
#include "Camera.h"
#include "Tile.h"

class Renderer 
{
public:
    Renderer();
    ~Renderer();

    void InitOpenGL();                                    // Initialize OpenGL settings
    void RenderMap(const Map& map, const Camera& camera); // Render the map based on the camera's position

private:
    void RenderChunk(const Chunk& chunk, const glm::vec2& cameraCoords, const float& zoomLevel, const int rotation); // Render a single chunk
    void RenderTile(const Tile& tile, float screenX, float screenY, float tileSize);  // Render a single tile
};

#endif
