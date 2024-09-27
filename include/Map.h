#pragma once

#include <unordered_map>
#include <memory>
#include <glm/glm.hpp>
#include "Chunk.h"
#include "Camera.h"
#include "Noise.h"
#include "TileRenderData.h"

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

    void UpdateChunkRenderBuffer(const Camera& camera, const bool& forceBufferUpdate);     // Gather chunks for rendering
    void UpdateMapRenderBuffers(const Camera& camera, const bool& forceChunkBufferUpdate); // Update chunk and tile render buffer
    
    std::vector<TileRenderData>& GetTileRenderData(const Camera& camera);                  // Return tile render data
    std::shared_ptr<Chunk>& GetChunk(const glm::vec2& chunkCoords);                        // Get a single chunk

    static float L1Distance(const glm::vec2& vec1, const glm::vec2& vec2);

private: 
    void UpdateTileRenderBuffer(const Camera& camera);
    void InitializeTileTypeColors();

    std::unordered_map<glm::vec2, std::shared_ptr<Chunk>, vec2_hash> fullChunkMap;  // All generated chunks
    std::vector<std::shared_ptr<Chunk>> chunkRenderBuffer;                          // 3x3 buffer of chunks around the camera
    std::vector<TileRenderData> tileRenderBuffer;                                   // All tiles to be rendered
    glm::vec2 prevCenterChunkCoords;
    Noise& mapNoise;
    std::unordered_map<uint8_t, glm::vec3> tileTypeColorMap;
};