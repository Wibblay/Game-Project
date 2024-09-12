#include "Map.h"
#include "Config.h"
#include <iostream>

Map::Map(Noise& noise) : mapNoise(noise), prevCenterChunkCoords(glm::vec2(0.0f, 0.0f)) 
{
    // Initialize the 3x3 renderBuffer with nullptr (empty unique_ptr<Chunk>)
    renderBuffer.resize(MapConfig::CHUNKS_TO_RENDER_GRID_SIZE);
    for (auto& row : renderBuffer) 
    {
        row.resize(MapConfig::CHUNKS_TO_RENDER_GRID_SIZE);  
    }
}

// Update the render buffer with chunks around the camera
void Map::UpdateChunkRenderBuffer(const Camera& camera, const bool& forceBufferUpdate) 
{
    glm::vec2 centerChunkCoords = glm::floor(camera.GetCoords() / static_cast<float>(MapConfig::CHUNK_SIZE));
    if (forceBufferUpdate || centerChunkCoords != prevCenterChunkCoords) 
    {   
       
        int renderChunkRadius = (MapConfig::CHUNKS_TO_RENDER_GRID_SIZE - 1) / 2;

        // Update the 3x3 render buffer with chunks around the camera
        for (int dy = -renderChunkRadius; dy <= renderChunkRadius; ++dy) 
        {
            for (int dx = -renderChunkRadius; dx <= renderChunkRadius; ++dx) 
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
                renderBuffer[dy + 1][dx + 1] = chunkPtr;
            }
        }
        prevCenterChunkCoords = centerChunkCoords;
        //DebugRenderBuffer();
    }
}

// Return the 3x3 render buffer for the renderer to use
const std::vector<std::vector<std::shared_ptr<Chunk>>>& Map::GetRenderBuffer() const
{
    return renderBuffer;
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

void Map::DebugRenderBuffer() const 
{
    std::cout << "Render Buffer Contents:" << std::endl;

    for (int y = 0; y < 3; ++y) 
    {
        for (int x = 0; x < 3; ++x) 
        {
            if (renderBuffer[y][x]) 
            {
                glm::vec2 chunkPos = renderBuffer[y][x]->GetCoords();
                std::cout << "[" << chunkPos.x << ", " << chunkPos.y << "] ";
            } else 
            {
                std::cout << "[null] ";
            }
        }
        std::cout << std::endl;
    }
}