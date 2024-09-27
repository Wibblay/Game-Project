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

    void UpdateChunkRenderBuffer(const Camera& camera, const bool& forceBufferUpdate);    //Prepare chunks for rendering
    void UpdateMapRenderBuffers(const Camera& camera, const bool& forceChunkBufferUpdate);
    std::vector<TileRenderData>& CollectTileRenderData(const Camera& camera);

    std::shared_ptr<Chunk>& GetChunk(const glm::vec2& chunkCoords);                       // Get a single chunk
    static float L1Distance(const glm::vec2& vec1, const glm::vec2& vec2);

private: 
    void UpdateTileRenderBuffer(const Camera& camera);
    void InitializeTileTypeColors();

    std::unordered_map<glm::vec2, std::shared_ptr<Chunk>, vec2_hash> fullChunkMap;  // All generated chunks
    std::vector<std::shared_ptr<Chunk>> chunkRenderBuffer;                  // 3x3 render buffer
    std::vector<TileRenderData> tileRenderBuffer;
    glm::vec2 prevCenterChunkCoords;
    Noise& mapNoise;
    std::unordered_map<uint8_t, glm::vec3> tileTypeColorMap;
    const std::vector<glm::mat2> rotationMatrices = {glm::mat2(1.0f, 0.0f, 0.0f, 1.0f),
                                                    glm::mat2(0.0f, -1.0f, 1.0f, 0.0f),
                                                    glm::mat2(-1.0f, 0.0f, 0.0f, -1.0f),
                                                    glm::mat2(0.0f, 1.0f, -1.0f, 0.0f)};
    const glm::mat2 isoMatrix = glm::mat2(1.0f, -1.0f, 1.0f, 1.0f);
};