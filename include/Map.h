#ifndef MAP_H
#define MAP_H

#include <unordered_map>
#include <memory>
#include <future>
#include <mutex>
#include <thread>
#include <shared_mutex>
#include <glm/glm.hpp>
#include "Chunk.h"
#include "Camera.h"
#include "Noise.h"
//#include "ThreadPool.h"

class Renderer;

// Hash function for glm::vec2 to use in chunkk map
struct vec2_hash
{
    size_t operator()(const glm::vec2& v) const
    {
        return std::hash<float>()(v.x) ^ std::hash<float>()(v.y);
    }
};

class Map 
{
public:
    Map(Noise& noise);
    ~Map() = default;

    void UpdateChunkRenderBuffer(Camera& camera, const bool& forcePreload);
    void DebugRenderBuffer() const;

    //Getters
    const std::vector<std::vector<std::unique_ptr<Chunk>>>& GetRenderBuffer() const; //Get the buffer of chunks to be rendered
    std::unique_ptr<Chunk>& GetChunk(glm::vec2 chunkCoords);             // Get a single chunk

private:
    void PreloadChunks(const glm::vec2& centerChunkCoords, const int& renderChunkRadius);
    void PreloadChunksAsync(const glm::vec2& centerChunkCoords, const int& renderChunkRadius);
    void WaitForPreload();
    void UnloadDistantChunks(const glm::vec2& centerChunkCoords, const int& renderChunkRadius);
    Chunk* LoadOrGenerateChunk(const glm::vec2& chunkCoords);
    
    std::unordered_map<glm::vec2, std::unique_ptr<Chunk>, vec2_hash> fullChunkMap;  // All generated chunks
    std::unordered_map<glm::vec2, std::unique_ptr<Chunk>, vec2_hash> chunkCache;    // 5x5 chunk cache
    std::vector<std::vector<std::unique_ptr<Chunk>>> renderBuffer;                  // 3x3 render buffer
    glm::vec2 prevCenterChunkCoords;
    Noise& mapNoise;
    std::mutex chunkCacheMutex;
    std::shared_mutex fullMapMutex;
    //ThreadPool mapThreadPool;
};

#endif //MAP_H