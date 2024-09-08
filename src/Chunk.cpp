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
                                            chunkCoords.y * MapConfig::CHUNK_SIZE + y), 1));  // Initialize all tiles with type 1
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
        // Calculate the world position of the tile relative to chunk start
        float worldX = chunkCoords.x + tile.tileCoords.x;
        float worldY = chunkCoords.y + tile.tileCoords.y;

        // Get the noise value for this tile (adjust scaling as necessary)
        float height = noiseGenerator.GetNoise(worldX, worldY);

        tile.height = (height + 1.0f) * 5.0f;  // Convert noise from [-1, 1] to [0, 10]
    }
}

const std::vector<Tile>& Chunk::GetTiles() const 
{
    return tiles;
}

const glm::vec2 Chunk::GetCoords() const
{
    return chunkCoords;
}
