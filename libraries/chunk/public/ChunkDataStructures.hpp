#ifndef FT_VOX_CHUNKDATASTRUCTURES_HPP
#define FT_VOX_CHUNKDATASTRUCTURES_HPP

#include <cstdint>

#define GLM_FORCE_SWIZZLE
#include "glm/glm.hpp"

// Constants
static constexpr int32_t const BLOCK_PER_LINE = 16;
static constexpr int32_t const LINE_PER_PLANE = 16;
static constexpr int32_t const PLANE_PER_CHUNK = 256;
static constexpr int32_t const TOTAL_BLOCK =
  BLOCK_PER_LINE * LINE_PER_PLANE * PLANE_PER_CHUNK;
static constexpr int32_t const BLOCK_PER_PLANE =
  BLOCK_PER_LINE * LINE_PER_PLANE;

static constexpr glm::vec3 const CHUNK_SIZE =
  glm::vec3(BLOCK_PER_LINE, PLANE_PER_CHUNK, LINE_PER_PLANE);
static constexpr glm::vec3 const EXTENT = CHUNK_SIZE * 0.5f;

static constexpr uint8_t const LEFT_3_BITS = 224;

static constexpr int32_t const XY_PLUS_UPPER_LIMIT =
  PLANE_PER_CHUNK - BLOCK_PER_LINE - 1;
static constexpr int32_t const YZ_PLUS_UPPER_LIMIT = BLOCK_PER_LINE - 1;

static constexpr float const NORMALIZED_PLANE_PER_CHUNK =
  1.0f / static_cast<float>(PLANE_PER_CHUNK);

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

static constexpr uint8_t const BlockTypeTransparency[TOTAL_BLOCKS] = {
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
