#include "Chunk.hpp"

#include <cstring>
#include <stdexcept>

#include "glad/glad.h"

#include "NoiseUtils.hpp"

Chunk::Chunk()
  : _block_chunk()
  , _visible_blocks()
  , _nb_visible_blocks(0)
  , _chunk_position(0.0f)
  , _space_coord(0)
  , _center(0)
  , _vao(0)
  , _vbo_blocks(0)
  , _vbo_space_coord(0)
  , _vbo_indirect(0)
{}

Chunk::~Chunk()
{
    _deallocate_gpu();
}

Chunk::Chunk(Chunk &&src) noexcept
  : _block_chunk()
  , _visible_blocks()
  , _nb_visible_blocks(0)
  , _chunk_position(0.0f)
  , _space_coord(0)
  , _center(0)
  , _vao(0)
  , _vbo_blocks(0)
  , _vbo_space_coord(0)
  , _vbo_indirect(0)
{
    *this = std::move(src);
}

Chunk &
Chunk::operator=(Chunk &&rhs) noexcept
{
    _nb_visible_blocks = rhs._nb_visible_blocks;
    std::memcpy(
      &_block_chunk, &rhs._block_chunk, sizeof(uint8_t) * TOTAL_BLOCK);
    _visible_blocks = std::move(rhs._visible_blocks);
    _chunk_position = rhs._chunk_position;
    _space_coord = rhs._space_coord;
    _center = rhs._center;
    _vao = rhs._vao;
    _vbo_blocks = rhs._vbo_blocks;
    _vbo_space_coord = rhs._vbo_space_coord;
    _vbo_indirect = rhs._vbo_indirect;
    rhs._vao = 0;
    rhs._vbo_blocks = 0;
    rhs._vbo_space_coord = 0;
    rhs._vbo_indirect = 0;
    return (*this);
}

Chunk::Chunk(glm::ivec2 const &chunk_position)
  : _block_chunk()
  , _visible_blocks()
  , _nb_visible_blocks(0)
  , _chunk_position(chunk_position)
  , _space_coord(0.0f)
  , _center(0)
  , _vao(0)
  , _vbo_blocks(0)
  , _vbo_space_coord(0)
  , _vbo_indirect(0)
{
    _space_coord =
      glm::vec3(static_cast<float>(chunk_position.x) * BLOCK_PER_LINE,
                0,
                static_cast<float>(chunk_position.y) * LINE_PER_PLANE);
    _center = _space_coord + EXTENT;
}

uint8_t
Chunk::addBlock(uint16_t index, BlockType type)
{
    _block_chunk[index] = (_block_chunk[index] & LEFT_3_BITS) | type;
    _visible_blocks = std::make_unique<uint32_t[]>(TOTAL_BLOCK);
    _generate_visible_blocks_buffer();
    return (0);
}

uint8_t
Chunk::addBlock(glm::vec3 const &pos, BlockType type)
{
    auto index = _get_index_from_pos(pos - _space_coord);
    if (index < 0 || index > TOTAL_BLOCK) {
        return (1);
    }

    if (!_block_chunk[index]) {
        _block_chunk[index] = (_block_chunk[index] & LEFT_3_BITS) | type;
        _visible_blocks = std::make_unique<uint32_t[]>(TOTAL_BLOCK);
        _generate_visible_blocks_buffer();
        return (0);
    }
    return (1);
}

uint8_t
Chunk::removeBlock(uint16_t index)
{
    _block_chunk[index] = 0;
    _visible_blocks = std::make_unique<uint32_t[]>(TOTAL_BLOCK);
    _generate_visible_blocks_buffer();
    return (0);
}

uint8_t
Chunk::removeBlock(glm::vec3 const &pos)
{
    auto index = _get_index_from_pos(pos - _space_coord);
    if (index < 0 || index > TOTAL_BLOCK) {
        return (1);
    }

    if (_block_chunk[index]) {
        _block_chunk[index] = 0;
        _visible_blocks = std::make_unique<uint32_t[]>(TOTAL_BLOCK);
        _generate_visible_blocks_buffer();
        return (0);
    }
    return (1);
}

uint8_t
Chunk::getBlock(uint16_t index) const
{
    return (_block_chunk[index]);
}

uint8_t
Chunk::getBlock(glm::vec3 const &pos) const
{
    auto index = _get_index_from_pos(pos - _space_coord);
    if (index < 0 || index > TOTAL_BLOCK) {
        return (0);
    }
    return (_block_chunk[index]);
}

uint16_t
Chunk::getBlockIndex(glm::vec3 const &pos) const
{
    return (_get_index_from_pos(pos - _space_coord));
}

void
Chunk::setPosition(glm::ivec2 const &pos)
{
    _chunk_position = pos;
    _space_coord =
      glm::vec3(static_cast<float>(_chunk_position.x) * BLOCK_PER_LINE,
                0.0f,
                static_cast<float>(_chunk_position.y) * LINE_PER_PLANE);
    _center = _space_coord + EXTENT;
}

glm::ivec2 const &
Chunk::getPosition() const
{
    return (_chunk_position);
}

glm::vec3 const &
Chunk::getSpaceCoordinate() const
{
    return (_space_coord);
}

uint32_t
Chunk::getVao() const
{
    return (_vao);
}

uint32_t
Chunk::getNbVisibleBlocks() const
{
    return (_nb_visible_blocks);
}

uint32_t
Chunk::getIndirectCommandVbo() const
{
    return (_vbo_indirect);
}

void
Chunk::generateChunk(PerlinNoise const &pn)
{
    _generate_with_seed(pn);
    _visible_blocks = std::make_unique<uint32_t[]>(TOTAL_BLOCK);
    _generate_visible_blocks_buffer();
}

uint8_t
Chunk::allocateGPUResources()
{
    _deallocate_gpu();
    if (_allocate_vbo()) {
        return (1);
    }
    if (_allocate_vao()) {
        return (1);
    }
    return (0);
}

void
Chunk::updateGPUResources()
{
    glBindBuffer(GL_ARRAY_BUFFER, _vbo_blocks);
    glBufferSubData(GL_ARRAY_BUFFER,
                    0,
                    sizeof(uint32_t) * _nb_visible_blocks,
                    &_visible_blocks[0]);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    auto gpu_space_coord = glm::vec4(_space_coord, 1.0f);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo_space_coord);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 4, &gpu_space_coord);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    _visible_blocks.reset();
}

uint8_t
Chunk::isChunkInFrustum(
  std::array<glm::vec4, 6> const &frustum_planes,
  std::array<glm::vec4, 6> const &abs_frustum_planes) const
{
    uint32_t i = 0;
    for (auto const &plane : frustum_planes) {
        float d = glm::dot(_center.xyz(), plane.xyz());
        float r = glm::dot(EXTENT.xyz(), abs_frustum_planes[i].xyz());
        if (d + r < -plane.w) {
            return (0);
        }
        ++i;
    }
    return (1);
}

void
Chunk::_generate_with_seed(PerlinNoise const &pn)
{
    for (int32_t i = 0; i < BLOCK_PER_LINE; ++i) {
        for (int32_t j = 0; j < LINE_PER_PLANE; ++j) {
            // uv_coord is be between [0;1]
            auto uv_coord =
              glm::mod(glm::abs(glm::vec2(
                         (_space_coord.x + i + (MAX_BLOCK_PER_LINE / 2)),
                         (_space_coord.z + j + (MAX_LINE_PER_PLANE / 2)))),
                       glm::vec2(MAX_BLOCK_PER_LINE, MAX_LINE_PER_PLANE));
            uv_coord.x /= static_cast<float>(MAX_BLOCK_PER_LINE);
            uv_coord.y /= static_cast<float>(MAX_LINE_PER_PLANE);

            auto elevation_temperature = getElevationTemperature(uv_coord, pn);
            auto cave_limits = getCaveLimits(uv_coord, pn);
            _fill_block_chunk(i, j, elevation_temperature, cave_limits);
        }
    }
}

void
Chunk::_fill_block_chunk(int32_t x,
                         int32_t y,
                         glm::vec2 const &elevation_temperature,
                         glm::vec2 const &cave_limits)
{
    int32_t z_max = elevation_temperature.x * (PLANE_PER_CHUNK - 1);
    int32_t cave_skip_min = cave_limits.x * (PLANE_PER_CHUNK - 1);
    int32_t cave_skip_max = cave_limits.y * (PLANE_PER_CHUNK - 1);
    z_max = (z_max < 0) ? 0 : z_max;

    // Adding blocks
    for (int32_t i = 0; i <= z_max; ++i) {
        // Cave generation
        if (i > cave_skip_min && i < cave_skip_max) {
            continue;
        }

        // Land generation
        if (elevation_temperature.y <= COLD_TEMP) {
            if (i <= BEDROCK_LEVEL) {
                _block_chunk[x + y * LINE_PER_PLANE + i * PLANE_PER_CHUNK] =
                  BEDROCK;
            } else if (i < WATER_LEVEL) {
                _block_chunk[x + y * LINE_PER_PLANE + i * PLANE_PER_CHUNK] =
                  STONE;
            } else if (i == z_max && i <= SNOW_LEVEL) {
                _block_chunk[x + y * LINE_PER_PLANE + i * PLANE_PER_CHUNK] =
                  SNOW_GRASS;
            } else if (i > SNOW_LEVEL) {
                _block_chunk[x + y * LINE_PER_PLANE + i * PLANE_PER_CHUNK] =
                  SNOW;
            } else if (i == cave_skip_min && i > WATER_LEVEL) {
                _block_chunk[x + y * LINE_PER_PLANE + i * PLANE_PER_CHUNK] =
                  SNOW_GRASS;
            } else {
                _block_chunk[x + y * LINE_PER_PLANE + i * PLANE_PER_CHUNK] =
                  DIRT;
            }
        } else if (elevation_temperature.y <= DESERT_TEMP) {
            if (i <= BEDROCK_LEVEL) {
                _block_chunk[x + y * LINE_PER_PLANE + i * PLANE_PER_CHUNK] =
                  BEDROCK;
            } else if (i < WATER_LEVEL) {
                _block_chunk[x + y * LINE_PER_PLANE + i * PLANE_PER_CHUNK] =
                  STONE;
            } else if (i == WATER_LEVEL) {
                _block_chunk[x + y * LINE_PER_PLANE + i * PLANE_PER_CHUNK] =
                  SAND;
            } else if (i == z_max && i < 140) {
                _block_chunk[x + y * LINE_PER_PLANE + i * PLANE_PER_CHUNK] =
                  GRASS;
            } else if ((i == z_max && i >= 140 && i < SNOW_LEVEL) ||
                       i == SNOW_LEVEL) {
                _block_chunk[x + y * LINE_PER_PLANE + i * PLANE_PER_CHUNK] =
                  SNOW_GRASS;
            } else if (i > SNOW_LEVEL) {
                _block_chunk[x + y * LINE_PER_PLANE + i * PLANE_PER_CHUNK] =
                  SNOW;
            } else if (i == cave_skip_min && i > WATER_LEVEL) {
                _block_chunk[x + y * LINE_PER_PLANE + i * PLANE_PER_CHUNK] =
                  GRASS;
            } else {
                _block_chunk[x + y * LINE_PER_PLANE + i * PLANE_PER_CHUNK] =
                  DIRT;
            }
        } else {
            if (i <= BEDROCK_LEVEL) {
                _block_chunk[x + y * LINE_PER_PLANE + i * PLANE_PER_CHUNK] =
                  BEDROCK;
            } else if (i < WATER_LEVEL) {
                _block_chunk[x + y * LINE_PER_PLANE + i * PLANE_PER_CHUNK] =
                  STONE;
            } else if (i == WATER_LEVEL || (i == z_max && i < 100)) {
                _block_chunk[x + y * LINE_PER_PLANE + i * PLANE_PER_CHUNK] =
                  SAND;
            } else if (i == z_max && i >= 100 && i <= SNOW_LEVEL) {
                _block_chunk[x + y * LINE_PER_PLANE + i * PLANE_PER_CHUNK] =
                  EMPTY;
            } else if (i > SNOW_LEVEL) {
                _block_chunk[x + y * LINE_PER_PLANE + i * PLANE_PER_CHUNK] =
                  SNOW;
            } else {
                _block_chunk[x + y * LINE_PER_PLANE + i * PLANE_PER_CHUNK] =
                  COBBLESTONE;
            }
        }
    }

    // Filling with water
    for (int32_t i = 0; i <= WATER_LEVEL; ++i) {
        if (!_block_chunk[x + y * LINE_PER_PLANE + i * PLANE_PER_CHUNK]) {
            if (elevation_temperature.y <= COLD_TEMP) {
                _block_chunk[x + y * LINE_PER_PLANE + i * PLANE_PER_CHUNK] =
                  ICE;
            } else if (elevation_temperature.y <= DESERT_TEMP) {
                _block_chunk[x + y * LINE_PER_PLANE + i * PLANE_PER_CHUNK] =
                  WATER;
            } else {
                _block_chunk[x + y * LINE_PER_PLANE + i * PLANE_PER_CHUNK] =
                  SAND;
            }
        }
    }
}

void
Chunk::_debug_generate_plane()
{
    std::memset(&_block_chunk, 0, sizeof(uint8_t) * TOTAL_BLOCK);
    uint32_t offset = 33;
    for (uint32_t i = offset; i < (BLOCK_PER_PLANE * 3 + offset); ++i) {
        _block_chunk[i] = BRICKS;
    }
}

void
Chunk::_debug_generate_blocks()
{
    std::memset(&_block_chunk, 0, sizeof(uint8_t) * TOTAL_BLOCK);
    uint32_t offset = 0;
    for (uint32_t i = 1; i < TOTAL_BLOCKS; ++i) {
        if (i && !((2 * i) % BLOCK_PER_LINE)) {
            offset += BLOCK_PER_LINE;
        }
        _block_chunk[2 * i + offset] = i;
    }
}

void
Chunk::_generate_visible_blocks_buffer()
{
    _nb_visible_blocks = 0;
    for (int32_t i = 0; i < TOTAL_BLOCK; ++i) {
        uint16_t visible_faces;
        if (_block_chunk[i] &&
            (visible_faces = _compute_block_visible_faces(i))) {
            _visible_blocks[_nb_visible_blocks] =
              _block_chunk[i] | (visible_faces << 8) | (i << 16);
            ++_nb_visible_blocks;
        }
    }
}

uint8_t
Chunk::_compute_block_visible_faces(int32_t index)
{
    uint8_t visible_faces = 0;

    // XZ+
    if (((index + BLOCK_PER_PLANE) >= TOTAL_BLOCK) ||
        !_block_chunk[index + BLOCK_PER_PLANE] ||
        (BlockTypeTransparency[_block_chunk[index + BLOCK_PER_PLANE]] &&
         _block_chunk[index + BLOCK_PER_PLANE] != _block_chunk[index])) {
        visible_faces |= (1 << XZ_PLUS);
    }
    // XZ-
    if (((index - BLOCK_PER_PLANE) < 0) ||
        !_block_chunk[index - BLOCK_PER_PLANE] ||
        (BlockTypeTransparency[_block_chunk[index - BLOCK_PER_PLANE]] &&
         _block_chunk[index - BLOCK_PER_PLANE] != _block_chunk[index])) {
        visible_faces |= (1 << XZ_MINUS);
    }
    // YZ+
    if (((index % BLOCK_PER_LINE) == YZ_PLUS_UPPER_LIMIT) ||
        !_block_chunk[index + 1] ||
        (BlockTypeTransparency[_block_chunk[index + 1]] &&
         _block_chunk[index + 1] != _block_chunk[index])) {
        visible_faces |= (1 << YZ_PLUS);
    }
    // YZ-
    if (!(index % BLOCK_PER_LINE) || !_block_chunk[index - 1] ||
        (BlockTypeTransparency[_block_chunk[index - 1]] &&
         _block_chunk[index - 1] != _block_chunk[index])) {
        visible_faces |= (1 << YZ_MINUS);
    }
    // XY+
    if (((index % BLOCK_PER_PLANE) > XY_PLUS_UPPER_LIMIT) ||
        !_block_chunk[index + BLOCK_PER_LINE] ||
        (BlockTypeTransparency[_block_chunk[index + BLOCK_PER_LINE]] &&
         _block_chunk[index + BLOCK_PER_LINE] != _block_chunk[index])) {
        visible_faces |= (1 << XY_PLUS);
    }
    // XY-
    if (((index % BLOCK_PER_PLANE) < BLOCK_PER_LINE) ||
        !_block_chunk[index - BLOCK_PER_LINE] ||
        (BlockTypeTransparency[_block_chunk[index - BLOCK_PER_LINE]] &&
         _block_chunk[index - BLOCK_PER_LINE] != _block_chunk[index])) {
        visible_faces |= (1 << XY_MINUS);
    }
    return (visible_faces);
}

uint8_t
Chunk::_allocate_vbo()
{
    // Vbo for blocks
    glGenBuffers(1, &_vbo_blocks);
    if (!_vbo_blocks) {
        return (1);
    }
    glBindBuffer(GL_ARRAY_BUFFER, _vbo_blocks);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(uint32_t) * _nb_visible_blocks,
                 nullptr,
                 GL_STATIC_DRAW);
    if (glGetError() == GL_OUT_OF_MEMORY) {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        _deallocate_gpu();
        return (1);
    }

    // Vbo for space coord
    glGenBuffers(1, &_vbo_space_coord);
    if (!_vbo_space_coord) {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        _deallocate_gpu();
        return (1);
    }
    glBindBuffer(GL_ARRAY_BUFFER, _vbo_space_coord);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4, nullptr, GL_STATIC_DRAW);
    if (glGetError() == GL_OUT_OF_MEMORY) {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        _deallocate_gpu();
        return (1);
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Vbo for indirect draw command
    DrawArrayIndirectCommand command = { _nb_visible_blocks, 1, 0, 0 };
    glGenBuffers(1, &_vbo_indirect);
    if (!_vbo_indirect) {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_DRAW_INDIRECT_BUFFER, 0);
        _deallocate_gpu();
        return (1);
    }
    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, _vbo_indirect);
    glBufferData(GL_DRAW_INDIRECT_BUFFER,
                 sizeof(DrawArrayIndirectCommand),
                 &command,
                 GL_STATIC_DRAW);
    if (glGetError() == GL_OUT_OF_MEMORY) {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_DRAW_INDIRECT_BUFFER, 0);
        _deallocate_gpu();
        return (1);
    }
    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, 0);
    return (0);
}

uint8_t
Chunk::_allocate_vao()
{
    glGenVertexArrays(1, &_vao);
    if (!_vao) {
        return (1);
    }
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo_blocks);
    glVertexAttribIPointer(
      0, 1, GL_UNSIGNED_INT, sizeof(uint32_t), reinterpret_cast<void *>(0));
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo_space_coord);
    glVertexAttribPointer(
      1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, reinterpret_cast<void *>(0));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(1, 1);
    glBindVertexArray(0);
    return (0);
}

void
Chunk::_deallocate_gpu()
{
    if (_vao) {
        glDeleteVertexArrays(1, &_vao);
        _vao = 0;
    }
    if (_vbo_blocks) {
        glDeleteBuffers(1, &_vbo_blocks);
        _vbo_blocks = 0;
    }
    if (_vbo_space_coord) {
        glDeleteBuffers(1, &_vbo_space_coord);
        _vbo_space_coord = 0;
    }
    if (_vbo_indirect) {
        glDeleteBuffers(1, &_vbo_indirect);
        _vbo_indirect = 0;
    }
}

inline int32_t
Chunk::_get_index_from_pos(glm::vec3 const &pos)
{
    glm::ivec3 chunk_coord;

    chunk_coord.x = glm::abs(static_cast<int32_t>(glm::round(pos.x)) %
                             static_cast<int32_t>(BLOCK_PER_LINE));
    chunk_coord.y = glm::abs(static_cast<int32_t>(glm::round(pos.z)) %
                             static_cast<int32_t>(LINE_PER_PLANE));
    chunk_coord.z = glm::abs(static_cast<int32_t>(glm::round(pos.y)) %
                             static_cast<int32_t>(PLANE_PER_CHUNK));

    // Should not be drawn
    if (static_cast<int32_t>(glm::round(pos.y)) /
        static_cast<int32_t>(PLANE_PER_CHUNK)) {
        chunk_coord.z = -1;
    }

    int32_t index = chunk_coord.z * static_cast<int32_t>(PLANE_PER_CHUNK) +
                    chunk_coord.y * static_cast<int32_t>(LINE_PER_PLANE) +
                    chunk_coord.x;
    return (index);
}