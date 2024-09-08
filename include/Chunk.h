#ifndef CHUNK_H
#define CHUNK_H

#include "Tile.h"
#include "Noise.h"
#include <vector>
#include <glm/glm.hpp>

class Chunk 
{
public:
    Chunk(glm::vec2 chunkCoords, Noise& noiseGenerator);
    ~Chunk();

    const std::vector<Tile>& GetTiles() const;     // Get all the tiles
    const glm::vec2 GetCoords() const;             // Get chunk coords

private:
    void GenerateHeights(Noise& noiseGenerator);           // Method to generate tile heights using noise

    std::vector<Tile> tiles;         // Grid of tiles (64x64)
    glm::vec2 chunkCoords;           // Position of the chunk in world coordinates
};

#endif
