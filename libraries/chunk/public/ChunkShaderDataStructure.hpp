#ifndef FT_VOX_CHUNKSHADERDATASTRUCTURE_HPP
#define FT_VOX_CHUNKSHADERDATASTRUCTURE_HPP

#include <array>

#include "glm/glm.hpp"

typedef struct BlockTextureType
{
    glm::vec4 id_xz_plus;
    glm::vec4 id_xz_minus;
    glm::vec4 id_yz_plus;
    glm::vec4 id_yz_minus;
    glm::vec4 id_xy_plus;
    glm::vec4 id_xy_minus;
} BlockTextureType;

constexpr uint32_t NB_SHADER_TEXTURE = 16;

// Texture should have 16x16 tiles
constexpr float const TEX_STEP = 0.0625f;

// Up left texture coordinate
constexpr glm::vec4 TEX_STONE = glm::vec4(TEX_STEP, 0, 0, 0);
constexpr glm::vec4 TEX_GRASS_TOP = glm::vec4(0, 0, 0, 0);
constexpr glm::vec4 TEX_GRASS_SIDE =
  glm::vec4(6 * TEX_STEP, 2 * TEX_STEP, 0, 0);
constexpr glm::vec4 TEX_DIRT = glm::vec4(2 * TEX_STEP, 0, 0, 0);
constexpr glm::vec4 TEX_COBBLESTONE = glm::vec4(0, TEX_STEP, 0, 0);
constexpr glm::vec4 TEX_PLANK = glm::vec4(4 * TEX_STEP, 0, 0, 0);
constexpr glm::vec4 TEX_BEDROCK = glm::vec4(TEX_STEP, TEX_STEP, 0, 0);
constexpr glm::vec4 TEX_SAND = glm::vec4(2 * TEX_STEP, TEX_STEP, 0, 0);
constexpr glm::vec4 TEX_WATER = glm::vec4(0, 9 * TEX_STEP, 0, 0);
constexpr glm::vec4 TEX_SNOW = glm::vec4(2 * TEX_STEP, 4 * TEX_STEP, 0, 0);
constexpr glm::vec4 TEX_SNOW_GRASS =
  glm::vec4(4 * TEX_STEP, 4 * TEX_STEP, 0, 0);
constexpr glm::vec4 TEX_WOOD_TOP_BOT = glm::vec4(5 * TEX_STEP, TEX_STEP, 0, 0);
constexpr glm::vec4 TEX_WOOD_SIDE = glm::vec4(4 * TEX_STEP, TEX_STEP, 0, 0);
constexpr glm::vec4 TEX_FOLIAGE = glm::vec4(4 * TEX_STEP, 3 * TEX_STEP, 0, 0);
constexpr glm::vec4 TEX_TNT_SIDE = glm::vec4(8 * TEX_STEP, 0, 0, 0);
constexpr glm::vec4 TEX_TNT_TOP = glm::vec4(9 * TEX_STEP, 0, 0, 0);
constexpr glm::vec4 TEX_TNT_BOT = glm::vec4(10 * TEX_STEP, 0, 0, 0);
constexpr glm::vec4 TEX_BRICKS = glm::vec4(7 * TEX_STEP, 0, 0, 0);
constexpr glm::vec4 TEX_GLASS = glm::vec4(1 * TEX_STEP, 3 * TEX_STEP, 0, 0);
constexpr glm::vec4 TEX_ICE = glm::vec4(3 * TEX_STEP, 4 * TEX_STEP, 0, 0);
constexpr glm::vec4 TEX_NO_TEXTURE = glm::vec4(0.0);

// Constants for Z fighting
constexpr float const C = 1.0f;
constexpr float const FAR = 8000.0f;
constexpr float const OFFSET = 1.0f;

// Texture definition table
constexpr std::array<BlockTextureType, NB_SHADER_TEXTURE> const
  SHADER_TEXTURE_TABLE = {
      BlockTextureType{ TEX_STONE,
                        TEX_STONE,
                        TEX_STONE,
                        TEX_STONE,
                        TEX_STONE,
                        TEX_STONE },
      BlockTextureType{ TEX_GRASS_TOP,
                        TEX_DIRT,
                        TEX_DIRT,
                        TEX_DIRT,
                        TEX_DIRT,
                        TEX_DIRT },
      BlockTextureType{ TEX_DIRT,
                        TEX_DIRT,
                        TEX_DIRT,
                        TEX_DIRT,
                        TEX_DIRT,
                        TEX_DIRT },
      BlockTextureType{ TEX_COBBLESTONE,
                        TEX_COBBLESTONE,
                        TEX_COBBLESTONE,
                        TEX_COBBLESTONE,
                        TEX_COBBLESTONE,
                        TEX_COBBLESTONE },
      BlockTextureType{ TEX_PLANK,
                        TEX_PLANK,
                        TEX_PLANK,
                        TEX_PLANK,
                        TEX_PLANK,
                        TEX_PLANK },
      BlockTextureType{ TEX_BEDROCK,
                        TEX_BEDROCK,
                        TEX_BEDROCK,
                        TEX_BEDROCK,
                        TEX_BEDROCK,
                        TEX_BEDROCK },
      BlockTextureType{ TEX_SAND,
                        TEX_SAND,
                        TEX_SAND,
                        TEX_SAND,
                        TEX_SAND,
                        TEX_SAND },
      BlockTextureType{ TEX_WATER,
                        TEX_WATER,
                        TEX_WATER,
                        TEX_WATER,
                        TEX_WATER,
                        TEX_WATER },
      BlockTextureType{ TEX_SNOW,
                        TEX_SNOW,
                        TEX_SNOW,
                        TEX_SNOW,
                        TEX_SNOW,
                        TEX_SNOW },
      BlockTextureType{ TEX_SNOW,
                        TEX_DIRT,
                        TEX_SNOW_GRASS,
                        TEX_SNOW_GRASS,
                        TEX_SNOW_GRASS,
                        TEX_SNOW_GRASS },
      BlockTextureType{ TEX_WOOD_TOP_BOT,
                        TEX_WOOD_TOP_BOT,
                        TEX_WOOD_SIDE,
                        TEX_WOOD_SIDE,
                        TEX_WOOD_SIDE,
                        TEX_WOOD_SIDE },
      BlockTextureType{ TEX_FOLIAGE,
                        TEX_FOLIAGE,
                        TEX_FOLIAGE,
                        TEX_FOLIAGE,
                        TEX_FOLIAGE,
                        TEX_FOLIAGE },
      BlockTextureType{ TEX_TNT_TOP,
                        TEX_TNT_BOT,
                        TEX_TNT_SIDE,
                        TEX_TNT_SIDE,
                        TEX_TNT_SIDE,
                        TEX_TNT_SIDE },
      BlockTextureType{ TEX_BRICKS,
                        TEX_BRICKS,
                        TEX_BRICKS,
                        TEX_BRICKS,
                        TEX_BRICKS,
                        TEX_BRICKS },
      BlockTextureType{ TEX_GLASS,
                        TEX_GLASS,
                        TEX_GLASS,
                        TEX_GLASS,
                        TEX_GLASS,
                        TEX_GLASS },
      BlockTextureType{ TEX_ICE, TEX_ICE, TEX_ICE, TEX_ICE, TEX_ICE, TEX_ICE },
  };

constexpr std::array<BlockTextureType, NB_SHADER_TEXTURE> const
  SHADER_TEXTURE_LEVEL_1_TABLE = {
      BlockTextureType{ TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE },
      BlockTextureType{ TEX_GRASS_SIDE,
                        TEX_GRASS_SIDE,
                        TEX_GRASS_SIDE,
                        TEX_GRASS_SIDE,
                        TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE },
      BlockTextureType{ TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE },
      BlockTextureType{ TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE },
      BlockTextureType{ TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE },
      BlockTextureType{ TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE },
      BlockTextureType{ TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE },
      BlockTextureType{ TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE },
      BlockTextureType{ TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE },
      BlockTextureType{ TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE },
      BlockTextureType{ TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE },
      BlockTextureType{ TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE },
      BlockTextureType{ TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE },
      BlockTextureType{ TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE },
      BlockTextureType{ TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE },
      BlockTextureType{ TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE,
                        TEX_NO_TEXTURE },
  };

#endif // FT_VOX_CHUNKSHADERDATASTRUCTURE_HPP