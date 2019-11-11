#ifndef FT_VOX_CHUNKDATASTRUCTURES_HPP
#define FT_VOX_CHUNKDATASTRUCTURES_HPP

#include <cstdint>
#include <cassert>

#include "glm/glm.hpp"

// Constants
static constexpr uint32_t BLOCK_PER_LINE = 16;
static constexpr uint32_t LINE_PER_PLANE = 16;
static constexpr uint32_t PLANE_PER_CHUNK = 256;
static constexpr uint32_t TOTAL_BLOCK =
  BLOCK_PER_LINE * LINE_PER_PLANE * PLANE_PER_CHUNK;
static constexpr glm::vec3 CHUNK_SIZE =
  glm::vec3(BLOCK_PER_LINE, LINE_PER_PLANE, PLANE_PER_CHUNK);

// Data structure Definition
typedef uint16_t BlockLine;

typedef struct BlockPlane
{
    BlockLine line[LINE_PER_PLANE];
} BlockPlane;

typedef struct BlockChunk
{
    BlockPlane plane[PLANE_PER_CHUNK];
} BlockChunk;

typedef struct BlockChunkType
{
    uint8_t types[TOTAL_BLOCK / 2];
} BlockChunkType;

typedef enum BlockType
{
    EMPTY = 0,
    SAND,
    FREE_2,
    FREE_3,
    FREE_4,
    FREE_5,
    FREE_6,
    FREE_7,
    FREE_8,
    FREE_9,
    FREE_10,
    FREE_11,
    FREE_12,
    FREE_13,
    FREE_14,
    FREE_15,
    FREE_16,
    FREE_17,
    FREE_18,
    FREE_19,
    FREE_20,
    FREE_21,
    FREE_22,
    FREE_23,
    FREE_24,
    FREE_25,
    FREE_26,
    FREE_27,
    FREE_28,
    FREE_29,
    FREE_30,
    FREE_31,
} BlockType;

typedef enum ChunkState
{
    DELETED = 0,
    ACTIVE,
    UPDATED,
} ChunkState;

typedef struct ChunkPosition
{
    ChunkPosition()
      : x(0)
      , y(0){};
    ChunkPosition(uint64_t init_x, uint64_t init_y)
      : x(init_x)
      , y(init_y){};
    virtual ~ChunkPosition() = default;

    int64_t x;
    int64_t y;
} ChunkPosition;

// Size Check
static_assert(sizeof(BlockLine) == 2);
static_assert(sizeof(BlockPlane) == 32);
static_assert(sizeof(BlockChunk) == 8192);
static_assert(sizeof(BlockChunkType) == 32768);

#endif // FT_VOX_CHUNKDATASTRUCTURES_HPP
