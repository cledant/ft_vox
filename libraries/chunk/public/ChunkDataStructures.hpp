#ifndef FT_VOX_CHUNKDATASTRUCTURES_HPP
#define FT_VOX_CHUNKDATASTRUCTURES_HPP

#include <cstdint>

#define GLM_FORCE_SWIZZLE
#include "glm/glm.hpp"

// Constants
static constexpr int32_t BLOCK_PER_LINE = 16;
static constexpr int32_t LINE_PER_PLANE = 16;
static constexpr int32_t PLANE_PER_CHUNK = 256;
static constexpr int32_t TOTAL_BLOCK =
  BLOCK_PER_LINE * LINE_PER_PLANE * PLANE_PER_CHUNK;
static constexpr int32_t BLOCK_PER_PLANE = BLOCK_PER_LINE * LINE_PER_PLANE;

static constexpr glm::vec3 CHUNK_SIZE =
  glm::vec3(BLOCK_PER_LINE, PLANE_PER_CHUNK, LINE_PER_PLANE);
static constexpr glm::vec3 EXTENT = CHUNK_SIZE * 0.5f;

static constexpr uint8_t LEFT_3_BITS = 224;

static constexpr int32_t XY_PLUS_UPPER_LIMIT =
  PLANE_PER_CHUNK - BLOCK_PER_LINE - 1;
static constexpr int32_t YZ_PLUS_UPPER_LIMIT = BLOCK_PER_LINE - 1;

typedef enum BlockType
{
    EMPTY = 0,
    STONE,
    GRASS,
    DIRT,
    COBBLESTONE,
    PLANKS,
    BEDROCK,
    SAND,
    WATER,
    SNOW,
    SNOW_GRASS,
    WOOD,
    FOLIAGE,
    TNT,
    BRICKS,
    GLASS,
    ICE,
    TOTAL_BLOCKS,
} BlockType;

static constexpr uint8_t BlockTypeTransparency[TOTAL_BLOCKS] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0
};

typedef enum BlockFaces
{
    XZ_PLUS = 0,
    XZ_MINUS,
    YZ_PLUS,
    YZ_MINUS,
    XY_PLUS,
    XY_MINUS,
} BlockFaces;

typedef enum ChunkState
{
    DELETED = 0,
    PENDING,
    VISIBLE
} ChunkState;

#endif // FT_VOX_CHUNKDATASTRUCTURES_HPP
