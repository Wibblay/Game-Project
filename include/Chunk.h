#ifndef CHUNK_H
#define CHUNK_H

#include "Tile.h"
#include "Noise.h"
#include <vector>
#include <glm/glm.hpp>

namespace TerrainConfig
{
    inline constexpr float MAX_TERRAIN_HEIGHT = 10.0f;
    inline constexpr float WATER_THRESHOLD_PCNT = 0.3f;
    inline constexpr float LOWER_HILL_THRESHOLD_PCNT = 0.57f;
    inline constexpr float UPPER_HILL_THRESHOLD_PCNT = 0.61f;
}

class Chunk 
{
public:
    Chunk(glm::vec2 chunkCoords, Noise& noiseGenerator);
    ~Chunk();

    std::vector<Tile>& GetTiles();              // Get all the tiles
    const glm::vec2 GetCoords() const;                      // Get chunk coords

private:
    void GenerateHeights(Noise& noiseGenerator);            // Method to generate tile heights using noise

    std::vector<Tile> tiles;                                // Grid of tiles (64x64)
    glm::vec2 chunkCoords;                                  // Position of the chunk in world coordinates
};

#endif //CHUNK_H
