#pragma once

#include <glm/glm.hpp>
#include <cstdint>

struct TileRenderData 
{
    glm::vec2 tileCoords;    // Tile position in the chunk grid
    float height;
    glm::vec3 color;       
    bool sideFacesVisibleFlag;
};