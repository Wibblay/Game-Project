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
    void RenderMap(Map& map, const Camera& camera); // Render the map based on the camera's position

private:
    void PrepTilesForRender(Chunk& chunk, std::vector<std::reference_wrapper<Tile>>& tilesToRender, const glm::vec2& cameraCoords, const float& zoomLevel, const int& rotation);
    void RenderTile(const Tile& tile, const float& tileSize, const int& rotation);  // Render a single tile
    void RenderTopFace(const Tile& tile, const float& tileSize);
    void RenderSideFaces(const Tile& tile, const float& tileSize);
    void RenderLeftFace(const Tile& tile, const float& tileSize);
    void RenderRightFace(const Tile& tile, const float& tileSize);
};

#endif
