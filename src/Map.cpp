#include "Map.h"
#include "Config.h"
#include <iostream>

Map::Map(Noise& noise) : map_noise(noise) 
{
    // Initialize the 3x3 renderBuffer with nullptr (empty unique_ptr<Chunk>)
    renderBuffer.resize(MapConfig::CHUNKS_TO_RENDER_GRID_SIZE);
    for (auto& row : renderBuffer) 
    {
        row.resize(MapConfig::CHUNKS_TO_RENDER_GRID_SIZE);  
    }
}

// Update the render buffer with chunks around the camera
void Map::UpdateChunkRenderBuffer(Camera& camera) 
{
    glm::vec2 centerChunkCoords = glm::floor(camera.GetCoords() / static_cast<float>(MapConfig::CHUNK_SIZE));
    int renderChunkRadius = (MapConfig::CHUNKS_TO_RENDER_GRID_SIZE - 1) / 2;

    // Preload chunks in the 5x5 grid around the camera
    PreloadChunks(centerChunkCoords, renderChunkRadius);

    // Update the 3x3 render buffer with chunks around the camera
    for (int dy = -renderChunkRadius; dy <= renderChunkRadius; ++dy) 
    {
        for (int dx = -renderChunkRadius; dx <= renderChunkRadius; ++dx) 
        {
            glm::vec2 chunkCoords = centerChunkCoords + glm::vec2(dx, dy);

            // Move the chunk from the preload cache into the render buffer
            renderBuffer[dy + 1][dx + 1] = std::move(chunkCache[chunkCoords]);
        }
    }
    DebugRenderBuffer();
    // Unload chunks outside the 5x5 grid
    UnloadDistantChunks(centerChunkCoords, renderChunkRadius);
    
}

// Preload chunks in the 5x5 grid around the camera
void Map::PreloadChunks(const glm::vec2& centerChunkCoords, const int& renderChunkRadius) 
{
    for (int dy = -renderChunkRadius - 2; dy <= renderChunkRadius + 2; ++dy) 
    {
        for (int dx = -renderChunkRadius - 2; dx <= renderChunkRadius + 2; ++dx) 
        {
            glm::vec2 chunkCoords = centerChunkCoords + glm::vec2(dx, dy);

            // Load the chunk from the fullChunkMap or generate it if necessary
            Chunk* chunkPtr = LoadOrGenerateChunk(chunkCoords);
            chunkCache[chunkCoords] = std::make_unique<Chunk>(*chunkPtr);
        }
    }
}

// Unload chunks outside the 5x5 grid
void Map::UnloadDistantChunks(const glm::vec2& centerChunkCoords, const int& renderChunkRadius) 
{
    std::vector<glm::vec2> chunksToUnload;

    // Mark chunks for unloading if they are outside the 5x5 grid
    for (const auto& chunkPair : chunkCache) 
    {
        glm::vec2 chunkCoords = chunkPair.first;
        if (glm::abs(chunkCoords.x - centerChunkCoords.x) > renderChunkRadius + 2 || glm::abs(chunkCoords.y - centerChunkCoords.y) > renderChunkRadius + 2) 
        {
            chunksToUnload.push_back(chunkCoords);
        }
    }

    // Unload the marked chunks from the chunkCache
    for (const glm::vec2& chunkCoords : chunksToUnload) 
    {
        chunkCache.erase(chunkCoords);
    }
}

// Load a chunk from the fullChunkMap or generate it if necessary
Chunk* Map::LoadOrGenerateChunk(const glm::vec2& chunkCoords) 
{
    // Check if the chunk already exists in the fullChunkMap
    if (fullChunkMap.find(chunkCoords) != fullChunkMap.end()) 
    {
        return fullChunkMap[chunkCoords].get(); 
    }

    // Otherwise, generate a new chunk and store it in the fullChunkMap
    std::cout << "Making new chunk at: [" << chunkCoords.x << chunkCoords.y << "]" << std::endl;
    fullChunkMap[chunkCoords] = std::make_unique<Chunk>(chunkCoords, map_noise);
    return fullChunkMap[chunkCoords].get();
}

// Return the 3x3 render buffer for the renderer to use
const std::vector<std::vector<std::unique_ptr<Chunk>>>& Map::GetRenderBuffer() const
{
    return renderBuffer;
}

std::unique_ptr<Chunk>& Map::GetChunk(glm::vec2 chunkCoords)
{
    // If the chunk doesn't exist, generate it
    if (fullChunkMap.find(chunkCoords) == fullChunkMap.end())
    {
        fullChunkMap[chunkCoords] = std::make_unique<Chunk>(chunkCoords, map_noise);
    }

    return fullChunkMap[chunkCoords];
}

void Map::DebugRenderBuffer() const {
    std::cout << "Render Buffer Contents:" << std::endl;

    for (int y = 0; y < 3; ++y) {
        for (int x = 0; x < 3; ++x) {
            if (renderBuffer[y][x]) {
                glm::vec2 chunkPos = renderBuffer[y][x]->GetCoords();
                std::cout << "[" << chunkPos.x << ", " << chunkPos.y << "] ";
            } else {
                std::cout << "[null] ";
            }
        }
        std::cout << std::endl;
    }
}