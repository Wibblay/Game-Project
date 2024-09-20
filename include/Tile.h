#ifndef TILE_H
#define TILE_H

#include <glm/glm.hpp>
#include <cstdint>

struct Tile 
{
    glm::vec2 tileCoords;    // Tile position in the chunk grid
    std::uint8_t type;       // Tile type (for now, this can be a simple integer)
    float height;
    bool sideFacesVisibleFlags[4];

    Tile(glm::vec2 _tileCoords, std::uint8_t _type = 1, float _height = 0.0f)
        : tileCoords(_tileCoords), type(_type), height(_height) 
        {
            sideFacesVisibleFlags[0] = true;
            sideFacesVisibleFlags[1] = true;
            sideFacesVisibleFlags[2] = true;
            sideFacesVisibleFlags[3] = true;
        }
};

#endif //TILE_H
