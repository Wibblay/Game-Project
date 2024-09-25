#include "Chunk.h"
#include "Config.h"
#include <iostream>

Chunk::Chunk(glm::vec2 chunkCoords, Noise& noiseGenerator)
    : chunkCoords(chunkCoords) 
{
    // Create a grid of tiles
    for (int y = 0; y < MapConfig::CHUNK_SIZE; ++y) 
    {
        for (int x = 0; x < MapConfig::CHUNK_SIZE; ++x) 
        {
            tiles.push_back(Tile(glm::vec2(chunkCoords.x * MapConfig::CHUNK_SIZE + x, 
                                            chunkCoords.y * MapConfig::CHUNK_SIZE + y)));  // Initialize all tiles with type 1
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
        // Get the noise value for this tile 
        float height = noiseGenerator.GetNoise(tile.tileCoords.x, tile.tileCoords.y); // Noise in [-1, 1]
        tile.height = (height + 1.0f) * 5.0f;  
        
        if (tile.height <= 2.0f)
        {
            tile.type = 2; // Water
            tile.height = 2.0f;
        }
        else if (tile.height >= 8.0f)
        {
            tile.type = 1; // Grass
            tile.height = 8.0f;
        }
        else if (tile.height <= MapConfig::UPPER_HILL_NOISE_THRESHOLD)
        {
            tile.type = 3; //Uneven
            //tile.height -= MapConfig::WATER_NOISE_THRESHOLD;
        }
        else
        {
            tile.type = 4; //Hill
            //tile.height = MapConfig::WATER_NOISE_THRESHOLD + MapConfig::UPPER_HILL_NOISE_THRESHOLD - MapConfig::LOWER_HILL_NOISE_THRESHOLD;
        }
    }
}

std::vector<Tile>& Chunk::GetTiles() 
{
    return tiles;
}

const glm::vec2 Chunk::GetCoords() const
{
    return chunkCoords;
}
