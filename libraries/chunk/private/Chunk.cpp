#include "Chunk.hpp"

#include <cstring>
#include <stdexcept>
#include <iostream>

#include "glad/glad.h"

Chunk::Chunk()
  : _block_chunk()
  , _visible_blocks()
  , _nb_visible_blocks(0)
  , _chunk_position(0.0f)
  , _space_coord(0)
  , _center(0)
  , _vao(0)
  , _vbo(0)
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
  , _vbo(0)
{
    *this = std::move(src);
}

Chunk &
Chunk::operator=(Chunk &&rhs) noexcept
{
    _nb_visible_blocks = rhs._nb_visible_blocks;
    std::memcpy(
      &_block_chunk, &rhs._block_chunk, sizeof(uint8_t) * TOTAL_BLOCK);
    std::memcpy(
      &_visible_blocks, &rhs._visible_blocks, sizeof(uint32_t) * TOTAL_BLOCK);
    _chunk_position = rhs._chunk_position;
    _space_coord = rhs._space_coord;
    _center = rhs._center;
    _vao = rhs._vao;
    _vbo = rhs._vbo;
    rhs._vao = 0;
    rhs._vbo = 0;
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
  , _vbo(0)
{
    _space_coord =
      glm::vec3(static_cast<float>(chunk_position.x) * BLOCK_PER_LINE,
                0,
                static_cast<float>(chunk_position.y) * LINE_PER_PLANE);
    _center = _space_coord + EXTENT;
}

void
Chunk::addBlock(uint16_t index, BlockType type)
{
    _block_chunk[index] = (_block_chunk[index] & LEFT_3_BITS) | type;
    generateChunk();
}

void
Chunk::removeBlock(uint16_t index)
{
    _block_chunk[index] = 0;
    generateChunk();
}

uint8_t
Chunk::getBlock(uint16_t index) const
{
    return (_block_chunk[index]);
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

void
Chunk::generateChunk()
{
    // TODO : Actual generation
    _debug_generate_plane();
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
Chunk::updateGPUResources() const
{
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferSubData(GL_ARRAY_BUFFER,
                    0,
                    sizeof(uint32_t) * _nb_visible_blocks,
                    &_visible_blocks[0]);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
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
Chunk::_debug_generate_plane()
{
    std::memset(&_block_chunk, 0, sizeof(uint8_t) * TOTAL_BLOCK);
    uint32_t offset = 8;
    for (uint32_t i = offset; i < (BLOCK_PER_PLANE * 1 + offset); ++i) {
        _block_chunk[i] = DEBUG;
    }
}

void
Chunk::_generate_visible_blocks_buffer()
{
    for (uint32_t i = 0; i < TOTAL_BLOCK; ++i) {
        uint16_t visible_faces;
        if (_block_chunk[i] == EMPTY ||
            !(visible_faces = _compute_block_visible_faces(i))) {
            continue;
        }
        _visible_blocks[i] = _block_chunk[i] | (visible_faces << 8) | (i << 16);
        ++_nb_visible_blocks;
    }
    std::cout << "NB VISIBLE BLOCKS = " << _nb_visible_blocks << std::endl;
}

uint8_t
Chunk::_compute_block_visible_faces(int32_t index)
{
    uint8_t visible_faces = 0;

    // XZ+
    if ((((index + BLOCK_PER_PLANE) < 0) ||
         ((index + BLOCK_PER_PLANE) >= TOTAL_BLOCK)) ||
        !_block_chunk[index + BLOCK_PER_PLANE]) {
        visible_faces |= (1 << XZ_PLUS);
    }
    // XZ-
    if ((((index - BLOCK_PER_PLANE) < 0) ||
         ((index - BLOCK_PER_PLANE) >= TOTAL_BLOCK)) ||
        !_block_chunk[index - BLOCK_PER_PLANE]) {
        visible_faces |= (1 << XZ_MINUS);
    }
    // YZ+
    if (((((index % BLOCK_PER_LINE) + 1) < 0) ||
         (((index % BLOCK_PER_LINE) + 1) >= BLOCK_PER_LINE)) ||
        !_block_chunk[index + 1]) {
        visible_faces |= (1 << YZ_PLUS);
    }
    // YZ-
    if (((((index % BLOCK_PER_LINE) - 1) < 0) ||
         (((index % BLOCK_PER_LINE) - 1) >= BLOCK_PER_LINE)) ||
        !_block_chunk[index - 1]) {
        visible_faces |= (1 << YZ_MINUS);
    }
    // XY+
    if ((((index + BLOCK_PER_LINE) < 0) ||
         ((index + BLOCK_PER_LINE) >= TOTAL_BLOCK)) ||
        !_block_chunk[index + BLOCK_PER_LINE]) {
        visible_faces |= (1 << XY_PLUS);
    }
    // XY-
    if ((((index - BLOCK_PER_LINE) < 0) ||
         ((index - BLOCK_PER_LINE) >= TOTAL_BLOCK)) ||
        !_block_chunk[index - BLOCK_PER_LINE]) {
        visible_faces |= (1 << XY_MINUS);
    }
    return (visible_faces);
}

uint8_t
Chunk::_allocate_vbo()
{
    glGenBuffers(1, &_vbo);
    if (!_vbo) {
        return (1);
    }
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(uint32_t) * _nb_visible_blocks,
                 nullptr,
                 GL_STATIC_DRAW);
    if (glGetError() == GL_OUT_OF_MEMORY) {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &_vbo);
        return (1);
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
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
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glVertexAttribIPointer(
      0, 1, GL_UNSIGNED_INT, sizeof(uint32_t), reinterpret_cast<void *>(0));
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(0, 1);
    glBindVertexArray(0);
    return (0);
}

void
Chunk::_deallocate_gpu()
{
    if (_vao) {
        glDeleteVertexArrays(1, &_vao);
    }
    if (_vbo) {
        glDeleteBuffers(1, &_vbo);
    }
}
