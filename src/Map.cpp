#include "Map.h"
#include "Config.h"
#include <iostream>

Map::Map(Noise& noise) : mapNoise(noise), prevCenterChunkCoords(glm::vec2(0.0f, 0.0f)) //, mapThreadPool(std::thread::hardware_concurrency())
{
    // Initialize the 3x3 renderBuffer with nullptr (empty unique_ptr<Chunk>)
    renderBuffer.resize(MapConfig::CHUNKS_TO_RENDER_GRID_SIZE);
    for (auto& row : renderBuffer) 
    {
        row.resize(MapConfig::CHUNKS_TO_RENDER_GRID_SIZE);  
    }
}

// Update the render buffer with chunks around the camera
void Map::UpdateChunkRenderBuffer(Camera& camera, const bool& forcePreload) 
{
    glm::vec2 centerChunkCoords = glm::floor(camera.GetCoords() / static_cast<float>(MapConfig::CHUNK_SIZE));
    if (forcePreload || centerChunkCoords != prevCenterChunkCoords) 
    {   
        prevCenterChunkCoords = centerChunkCoords;
        int renderChunkRadius = (MapConfig::CHUNKS_TO_RENDER_GRID_SIZE - 1) / 2;
        std::cout << "Centre chunk: " << centerChunkCoords.x << centerChunkCoords.y << std::endl;

        // Preload chunks in the 5x5 grid around the camera
        PreloadChunksAsync(centerChunkCoords, renderChunkRadius);
        //WaitForPreload();

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
}

// Preload chunks in the 5x5 grid around the camera
void Map::PreloadChunksAsync(const glm::vec2& centerChunkCoords, const int& renderChunkRadius) 
{
    std::vector<std::future<void>> preLoadFutures;

    for (int dy = -renderChunkRadius - 2; dy <= renderChunkRadius + 2; ++dy) 
    {
        for (int dx = -renderChunkRadius - 2; dx <= renderChunkRadius + 2; ++dx) 
        {
            glm::vec2 chunkCoords = centerChunkCoords + glm::vec2(dx, dy);

            // Create an async task for loading/generating the chunk
            preLoadFutures.push_back(std::async(std::launch::async, [this, chunkCoords]() 
            {
                Chunk* chunkPtr = LoadOrGenerateChunk(chunkCoords);
                {
                    std::lock_guard<std::mutex> lock(chunkCacheMutex);
                    chunkCache[chunkCoords] = std::make_unique<Chunk>(*chunkPtr);
                }
            }));
        }
    }

    // Wait for all tasks to finish
    for (auto& future : preLoadFutures) 
    {
        future.get();
    }
}

// void Map::PreloadChunks(const glm::vec2& centerChunkCoords, const int& renderChunkRadius) 
// {
//     for (int dy = -renderChunkRadius - 2; dy <= renderChunkRadius + 2; ++dy) 
//     {
//         for (int dx = -renderChunkRadius - 2; dx <= renderChunkRadius + 2; ++dx) 
//         {
//             glm::vec2 chunkCoords = centerChunkCoords + glm::vec2(dx, dy);
//             mapThreadPool.EnqueueTask([this, chunkCoords]() 
//             {
//                 Chunk* chunkPtr = LoadOrGenerateChunk(chunkCoords);
//                 {
//                 std::lock_guard<std::mutex> lock(chunkCacheMutex);
//                 chunkCache[chunkCoords] = std::make_unique<Chunk>(std::move(*chunkPtr));
//                 }
//             });
//         }
//     }
// }

// void Map::WaitForPreload()
// {
//     // Wait for the thread pool to finish all active tasks
//     while (mapThreadPool.GetActiveTaskCount() > 0) 
//     {
//         std::this_thread::yield();
//     }
// }

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
    {
        std::shared_lock<std::shared_mutex> lock(fullMapMutex);

        if (fullChunkMap.find(chunkCoords) != fullChunkMap.end()) 
        {
            return fullChunkMap[chunkCoords].get();
        }
    }

    // Otherwise, generate a new chunk and store it in the fullChunkMap
    std::cout << "Making new chunk at: " << chunkCoords.x << chunkCoords.y << std::endl;
    std::unique_ptr<Chunk> newChunk = std::make_unique<Chunk>(chunkCoords, mapNoise);

    // Lock fullChunkMap again to safely store the new chunk
    {
        std::shared_lock<std::shared_mutex> lock(fullMapMutex);
        fullChunkMap[chunkCoords] = std::move(newChunk);
        return fullChunkMap[chunkCoords].get();
    }
}

// Return the 3x3 render buffer for the renderer to use
const std::vector<std::vector<std::unique_ptr<Chunk>>>& Map::GetRenderBuffer() const
{
    return renderBuffer;
}

std::unique_ptr<Chunk>& Map::GetChunk(glm::vec2 chunkCoords)
{
    std::shared_lock<std::shared_mutex> lock(fullMapMutex);
    
    // If the chunk doesn't exist, generate it
    if (fullChunkMap.find(chunkCoords) == fullChunkMap.end())
    {
        fullChunkMap[chunkCoords] = std::make_unique<Chunk>(chunkCoords, mapNoise);
    }

    return fullChunkMap[chunkCoords];
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