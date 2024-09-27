#include "Chunk.h"

#include "Config.h"
#include <iostream>

Chunk::Chunk(glm::vec2 chunkCoords, Noise& noiseGenerator)
    : chunkCoords(chunkCoords) 
{
    // Create a grid of tiles
    tiles.reserve(GlobalConfig::CHUNK_SIZE * GlobalConfig::CHUNK_SIZE);
    for (int y = 0; y < GlobalConfig::CHUNK_SIZE; ++y) 
    {
        for (int x = 0; x < GlobalConfig::CHUNK_SIZE; ++x) 
        {
            tiles.push_back(Tile(glm::vec2(chunkCoords.x * GlobalConfig::CHUNK_SIZE + x, 
                                            chunkCoords.y * GlobalConfig::CHUNK_SIZE + y))); 
        }
    }

    GenerateHeights(noiseGenerator);  // Generate heights for each tile using the Perlin noise generator
}

Chunk::~Chunk() {}

void Chunk::GenerateHeights(Noise& noiseGenerator) 
{
    // Iterate over each tile and assign a height value based on Perlin noise
    for (Tile& tile : tiles) 
    {
        tile.height = noiseGenerator.GetNoise(tile.tileCoords.x, tile.tileCoords.y); // Noise in [0, 1]

        tile.type = 1; 
        
        if (tile.height <= TerrainConfig::WATER_THRESHOLD_PCNT)
        {
            tile.type = 2;
            tile.height = TerrainConfig::WATER_THRESHOLD_PCNT - 0.025f;
        }
        else if (tile.height <= TerrainConfig::LOWER_HILL_THRESHOLD_PCNT)
        {
            tile.height = TerrainConfig::WATER_THRESHOLD_PCNT;
            tile.type = 1;
        }
        else if (tile.height <= TerrainConfig::UPPER_HILL_THRESHOLD_PCNT)
        {
            tile.height -= (TerrainConfig::LOWER_HILL_THRESHOLD_PCNT - TerrainConfig::WATER_THRESHOLD_PCNT);
            tile.type = 1;
        }
        else
        {
            tile.height = TerrainConfig::WATER_THRESHOLD_PCNT + (TerrainConfig::UPPER_HILL_THRESHOLD_PCNT - TerrainConfig::LOWER_HILL_THRESHOLD_PCNT);
            tile.type = 1;
        }

        tile.height *= TerrainConfig::MAX_TERRAIN_HEIGHT;
    }
}

std::vector<Tile>& Chunk::GetTiles() 
{
    return tiles;
}

const glm::vec2& Chunk::GetCoords() const
{
    return chunkCoords;
}
