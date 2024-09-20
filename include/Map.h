#ifndef MAP_H
#define MAP_H

#include <unordered_map>
#include <memory>
#include <glm/glm.hpp>
#include "Chunk.h"
#include "Camera.h"
#include "Noise.h"

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
    const std::vector<Tile>& CollectTileRenderData(const Camera& camera);

    std::shared_ptr<Chunk>& GetChunk(const glm::vec2& chunkCoords);                       // Get a single chunk

private: 
    void UpdateTileRenderBuffer(const Camera& camera);

    std::unordered_map<glm::vec2, std::shared_ptr<Chunk>, vec2_hash> fullChunkMap;  // All generated chunks
    std::vector<std::shared_ptr<Chunk>> chunkRenderBuffer;                  // 3x3 render buffer
    std::vector<Tile> tileRenderBuffer;
    glm::vec2 prevCenterChunkCoords;
    Noise& mapNoise;
    const std::vector<glm::mat2> rotationMatrices = {glm::mat2(1.0f, 0.0f, 0.0f, 1.0f),
                                                    glm::mat2(0.0f, -1.0f, 1.0f, 0.0f),
                                                    glm::mat2(-1.0f, 0.0f, 0.0f, -1.0f),
                                                    glm::mat2(0.0f, 1.0f, -1.0f, 0.0f)};
    const glm::mat2 isoMatrix = glm::mat2(1.0f, -1.0f, 1.0f, 1.0f);
};

#endif //MAP_H