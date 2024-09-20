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
        // Calculate the world position of the tile relative to chunk start
        float worldX = chunkCoords.x + tile.tileCoords.x;
        float worldY = chunkCoords.y + tile.tileCoords.y;

        // Get the noise value for this tile 
        float height = noiseGenerator.GetNoise(worldX, worldY); // Noise in [-1, 1]
        tile.height = (height + 1.0f) * 0.25f;  
        
        if (tile.height <= MapConfig::WATER_NOISE_THRESHOLD)
        {
            tile.type = 2; // Water
            //tile.height = MapConfig::WATER_NOISE_THRESHOLD - 0.05;
        }
        else if (tile.height <= MapConfig::LOWER_HILL_NOISE_THRESHOLD)
        {
            tile.type = 1; // Grass
            //tile.height = MapConfig::WATER_NOISE_THRESHOLD;
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
