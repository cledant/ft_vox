#ifndef FT_VOX_CHUNKSHADERDATASTRUCTURE_HPP
#define FT_VOX_CHUNKSHADERDATASTRUCTURE_HPP

#include <array>

#include "glm/glm.hpp"

typedef struct BlockTextureType
{
    glm::vec2 id_xy_plus;
    glm::vec2 id_xy_minus;
    glm::vec2 id_xz_plus;
    glm::vec2 id_xz_minus;
    glm::vec2 id_yz_plus;
    glm::vec2 id_yz_minus;
    glm::vec2 level_1_texture_coord;
    int use_color_modifier;
    int unused_1;
} BlockTextureType;

constexpr uint32_t NB_SHADER_TEXTURE = 16;

// Texture should have 16x16 tiles
constexpr float const TEX_STEP = 0.0625f;

// Up left texture coordinate
constexpr glm::vec2 TEX_STONE = glm::vec2(TEX_STEP, 0);
constexpr glm::vec2 TEX_GRASS_TOP = glm::vec2(0, 0);
constexpr glm::vec2 TEX_GRASS_SIDE = glm::vec2(6 * TEX_STEP, 2 * TEX_STEP);
constexpr glm::vec2 TEX_DIRT = glm::vec2(2 * TEX_STEP, 0);
constexpr glm::vec2 TEX_COBBLESTONE = glm::vec2(0, TEX_STEP);
constexpr glm::vec2 TEX_PLANK = glm::vec2(4 * TEX_STEP, 0);
constexpr glm::vec2 TEX_BEDROCK = glm::vec2(TEX_STEP, TEX_STEP);
constexpr glm::vec2 TEX_SAND = glm::vec2(2 * TEX_STEP, TEX_STEP);
constexpr glm::vec2 TEX_WATER = glm::vec2(0, 9 * TEX_STEP);
constexpr glm::vec2 TEX_SNOW = glm::vec2(2 * TEX_STEP, 4 * TEX_STEP);
constexpr glm::vec2 TEX_SNOW_GRASS = glm::vec2(4 * TEX_STEP, 4 * TEX_STEP);
constexpr glm::vec2 TEX_WOOD_TOP_BOT = glm::vec2(5 * TEX_STEP, TEX_STEP);
constexpr glm::vec2 TEX_WOOD_SIDE = glm::vec2(4 * TEX_STEP, TEX_STEP);
constexpr glm::vec2 TEX_FOLIAGE = glm::vec2(4 * TEX_STEP, 3 * TEX_STEP);
constexpr glm::vec2 TEX_TNT_SIDE = glm::vec2(8 * TEX_STEP, 0);
constexpr glm::vec2 TEX_TNT_TOP = glm::vec2(9 * TEX_STEP, 0);
constexpr glm::vec2 TEX_TNT_BOT = glm::vec2(10 * TEX_STEP, 0);
constexpr glm::vec2 TEX_BRICKS = glm::vec2(7 * TEX_STEP, 0);
constexpr glm::vec2 TEX_GLASS = glm::vec2(1 * TEX_STEP, 3 * TEX_STEP);
constexpr glm::vec2 TEX_ICE = glm::vec2(3 * TEX_STEP, 4 * TEX_STEP);
constexpr glm::vec2 TEX_NO_TEXTURE = glm::vec2(2.0);

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
                        TEX_STONE,
                        TEX_NO_TEXTURE,
                        0,
                        0 },
      BlockTextureType{ TEX_DIRT,
                        TEX_DIRT,
                        TEX_GRASS_TOP,
                        TEX_DIRT,
                        TEX_DIRT,
                        TEX_DIRT,
                        TEX_NO_TEXTURE,
                        1,
                        0 },
      BlockTextureType{ TEX_DIRT,
                        TEX_DIRT,
                        TEX_DIRT,
                        TEX_DIRT,
                        TEX_DIRT,
                        TEX_DIRT,
                        TEX_NO_TEXTURE,
                        0,
                        0 },
      BlockTextureType{ TEX_COBBLESTONE,
                        TEX_COBBLESTONE,
                        TEX_COBBLESTONE,
                        TEX_COBBLESTONE,
                        TEX_COBBLESTONE,
                        TEX_COBBLESTONE,
                        TEX_NO_TEXTURE,
                        0,
                        0 },
      BlockTextureType{ TEX_PLANK,
                        TEX_PLANK,
                        TEX_PLANK,
                        TEX_PLANK,
                        TEX_PLANK,
                        TEX_PLANK,
                        TEX_NO_TEXTURE,
                        0,
                        0 },
      BlockTextureType{ TEX_BEDROCK,
                        TEX_BEDROCK,
                        TEX_BEDROCK,
                        TEX_BEDROCK,
                        TEX_BEDROCK,
                        TEX_BEDROCK,
                        TEX_NO_TEXTURE,
                        0,
                        0 },
      BlockTextureType{ TEX_SAND,
                        TEX_SAND,
                        TEX_SAND,
                        TEX_SAND,
                        TEX_SAND,
                        TEX_SAND,
                        TEX_NO_TEXTURE,
                        0,
                        0 },
      BlockTextureType{ TEX_WATER,
                        TEX_WATER,
                        TEX_WATER,
                        TEX_WATER,
                        TEX_WATER,
                        TEX_WATER,
                        TEX_NO_TEXTURE,
                        0,
                        0 },
      BlockTextureType{ TEX_SNOW,
                        TEX_SNOW,
                        TEX_SNOW,
                        TEX_SNOW,
                        TEX_SNOW,
                        TEX_SNOW,
                        TEX_NO_TEXTURE,
                        0,
                        0 },
      BlockTextureType{ TEX_SNOW_GRASS,
                        TEX_SNOW_GRASS,
                        TEX_SNOW,
                        TEX_DIRT,
                        TEX_SNOW_GRASS,
                        TEX_SNOW_GRASS,
                        TEX_NO_TEXTURE,
                        0,
                        0 },
      BlockTextureType{ TEX_WOOD_SIDE,
                        TEX_WOOD_SIDE,
                        TEX_WOOD_TOP_BOT,
                        TEX_WOOD_TOP_BOT,
                        TEX_WOOD_SIDE,
                        TEX_WOOD_SIDE,
                        TEX_NO_TEXTURE,
                        0,
                        0 },
      BlockTextureType{ TEX_FOLIAGE,
                        TEX_FOLIAGE,
                        TEX_FOLIAGE,
                        TEX_FOLIAGE,
                        TEX_FOLIAGE,
                        TEX_FOLIAGE,
                        TEX_NO_TEXTURE,
                        1,
                        0 },
      BlockTextureType{ TEX_TNT_SIDE,
                        TEX_TNT_SIDE,
                        TEX_TNT_TOP,
                        TEX_TNT_BOT,
                        TEX_TNT_SIDE,
                        TEX_TNT_SIDE,
                        TEX_NO_TEXTURE,
                        0,
                        0 },
      BlockTextureType{ TEX_BRICKS,
                        TEX_BRICKS,
                        TEX_BRICKS,
                        TEX_BRICKS,
                        TEX_BRICKS,
                        TEX_BRICKS,
                        TEX_NO_TEXTURE,
                        0,
                        0 },
      BlockTextureType{ TEX_GLASS,
                        TEX_GLASS,
                        TEX_GLASS,
                        TEX_GLASS,
                        TEX_GLASS,
                        TEX_GLASS,
                        TEX_NO_TEXTURE,
                        0,
                        0 },
      BlockTextureType{ TEX_ICE,
                        TEX_ICE,
                        TEX_ICE,
                        TEX_ICE,
                        TEX_ICE,
                        TEX_ICE,
                        TEX_NO_TEXTURE,
                        0,
                        0 },
  };

#endif // FT_VOX_CHUNKSHADERDATASTRUCTURE_HPP