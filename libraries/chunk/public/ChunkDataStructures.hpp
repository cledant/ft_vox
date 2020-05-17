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

typedef enum BlockType
{
    EMPTY = 0,
    FREE_1,
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
    DEBUG,
} BlockType;

typedef enum BlockFaces
{
    XY_PLUS = 0,
    XY_MINUS,
    XZ_PLUS,
    XZ_MINUS,
    YZ_PLUS,
    YZ_MINUS,
} BlockFaces;

typedef enum ChunkState
{
    DELETED = 0,
    PENDING,
    VISIBLE
} ChunkState;

#endif // FT_VOX_CHUNKDATASTRUCTURES_HPP
