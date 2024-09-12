#ifndef TILE_H
#define TILE_H

#include <glm/glm.hpp>

struct Tile 
{
    glm::vec2 tileCoords;    // Tile position in the chunk grid
    int type;                // Tile type (for now, this can be a simple integer)
    float height;

    Tile(glm::vec2 _tileCoords, int _type, float _height = 0.0f)
        : tileCoords(_tileCoords), type(_type), height(_height) {}
};

#endif //TILE_H
