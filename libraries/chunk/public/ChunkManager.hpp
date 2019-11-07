#ifndef FT_VOX_CHUNKMANAGER_HPP
#define FT_VOX_CHUNKMANAGER_HPP

#include <cstdint>

#include "glm/glm.hpp"

#include "glad/glad.h"

class ChunkManager
{
  public:
    ChunkManager() = default;
    virtual ~ChunkManager() = default;
    ChunkManager(const ChunkManager &src) = delete;
    ChunkManager &operator=(ChunkManager const &rhs) = delete;
    ChunkManager(ChunkManager &&src) = delete;
    ChunkManager &operator=(ChunkManager &&rhs) = delete;

  private:
    static constexpr uint32_t BLOCK_PER_LINE = 16;
    static constexpr uint32_t LINE_PER_PLANE = 16;
    static constexpr uint32_t PLANE_PER_CHUNK = 256;

    // Chunk Definition
    typedef uint16_t ChunckLine;
    typedef struct ChunkPlane
    {
        ChunckLine line[LINE_PER_PLANE];
    } ChunkPlane;
    typedef struct Chunk
    {
        ChunkPlane line[PLANE_PER_CHUNK];
    } Chunk;
};

#endif // FT_VOX_CHUNKMANAGER_HPP
