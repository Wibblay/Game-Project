#include "Map.h"
#include "Config.h"
#include <iostream>
#include <algorithm>

Map::Map(Noise& noise) : mapNoise(noise), 
                        prevCenterChunkCoords(glm::vec2(0.0f, 0.0f)) 
{
    InitializeTileTypeColors();
}

void Map::InitializeTileTypeColors() {
    // For now, only one type is used
    tileTypeColorMap[1] = glm::vec3(0.11f, 0.54f, 0.23f);  // Green color for flat grass
    tileTypeColorMap[2] = glm::vec3(0.19f, 0.43f, 0.76f); // Blue color for water
    // Add more mappings as needed
}

// Update the render buffer with chunks around the camera
void Map::UpdateChunkRenderBuffer(const Camera& camera, const bool& forceBufferUpdate) 
{
    glm::vec2 centerChunkCoords = glm::floor(camera.GetCoords() / static_cast<float>(GlobalConfig::CHUNK_SIZE));
    if (forceBufferUpdate || centerChunkCoords != prevCenterChunkCoords) 
    {   

        chunkRenderBuffer.clear();

        // Update the 3x3 render buffer with chunks around the camera
        for (int dy = -1; dy <= 1; ++dy) 
        {
            for (int dx = -1; dx <= 1; ++dx) 
            {
                glm::vec2 chunkCoords = centerChunkCoords + glm::vec2(dx, dy);

                std::shared_ptr<Chunk> chunkPtr;
                auto it = fullChunkMap.find(chunkCoords);
                if (it != fullChunkMap.end()) 
                {
                    chunkPtr = it->second;
                } 
                else 
                {
                    // Generate and store the chunk if it's not in the map
                    chunkPtr = std::make_shared<Chunk>(chunkCoords, mapNoise);
                    fullChunkMap[chunkCoords] = chunkPtr;
                }

                // Move the chunk from the preload cache into the render buffer
                chunkRenderBuffer.push_back(chunkPtr);
            }
        }
        prevCenterChunkCoords = centerChunkCoords;
    }
}

void Map::UpdateTileRenderBuffer(const Camera& camera)
{
    tileRenderBuffer.clear();
    glm::vec2 cameraPosition = camera.GetCoords();
    int cameraRotation = camera.GetRotation();
    float zoomLevel = camera.GetZoomLevel();

    for (const auto& chunkPtr : chunkRenderBuffer)
    {
        if (chunkPtr)
        {
            const auto& tiles = chunkPtr->GetTiles();
            for (const auto& tile: tiles)
            {
                if (L1Distance(tile.tileCoords, cameraPosition) <= 1500.0 / zoomLevel)
                {
                    TileRenderData tileRenderData;
                    tileRenderData.tileCoords = tile.tileCoords;
                    tileRenderData.height = tile.height; 
                    tileRenderData.color = tileTypeColorMap[tile.type];
                    tileRenderData.sideFacesVisibleFlag = tile.sideFacesVisibleFlags[cameraRotation];
                    tileRenderBuffer.push_back(tileRenderData);
                }
                
            }
        }
    }

}

void Map::UpdateMapRenderBuffers(const Camera& camera, const bool& forceChunkBufferUpdate) 
{
    UpdateChunkRenderBuffer(camera, forceChunkBufferUpdate);
    UpdateTileRenderBuffer(camera);
}

std::vector<TileRenderData>& Map::CollectTileRenderData(const Camera& camera)
{
    return tileRenderBuffer;
}

std::shared_ptr<Chunk>& Map::GetChunk(const glm::vec2& chunkCoords)
{   
    auto it = fullChunkMap.find(chunkCoords);
    if (it == fullChunkMap.end()) 
    {
        it = fullChunkMap.emplace(chunkCoords, std::make_shared<Chunk>(chunkCoords, mapNoise)).first;
    }
    return it->second;
}

float Map::L1Distance(const glm::vec2& vec1, const glm::vec2& vec2)
{
    return std::abs(vec1.x - vec2.x) + std::abs(vec1.y - vec2.y);
}
