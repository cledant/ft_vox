#include "Chunk.hpp"

#include <cstring>
#include <stdexcept>
#include <iostream>

#include "glad/glad.h"

Chunk::Chunk()
  : _block_chunk()
  , _chunk_position(0.0f)
  , _space_coord(0)
  , _updated(1)
  , _vao(0)
  , _vbo(0)
{}

Chunk::~Chunk()
{
    if (_vao) {
        glDeleteVertexArrays(1, &_vao);
    }
    if (_vbo) {
        glDeleteBuffers(1, &_vbo);
    }
}

Chunk::Chunk(Chunk &&src) noexcept
  : _block_chunk()
  , _chunk_position(0.0f)
  , _space_coord(0)
  , _updated(0)
  , _vao(0)
  , _vbo(0)
{
    *this = std::move(src);
}

Chunk &
Chunk::operator=(Chunk &&rhs) noexcept
{
    std::memcpy(&_block_chunk, &rhs._block_chunk, TOTAL_BLOCK);
    _chunk_position = rhs._chunk_position;
    _space_coord = rhs._space_coord;
    _updated = rhs._updated;
    _vao = rhs._vao;
    _vbo = rhs._vbo;
    rhs._vao = 0;
    rhs._vbo = 0;
    return (*this);
}

Chunk::Chunk(glm::ivec2 const &chunk_position)
  : _block_chunk()
  , _chunk_position(chunk_position)
  , _space_coord(0.0f)
  , _updated(1)
  , _vao(0)
  , _vbo(0)
{
    _space_coord =
      glm::vec3(static_cast<float>(chunk_position.x) * BLOCK_PER_LINE,
                0,
                static_cast<float>(chunk_position.y) * LINE_PER_PLANE);
}

void
Chunk::addBlock(uint16_t index, BlockType type)
{
    _block_chunk[index] = (_block_chunk[index] & LEFT_3_BITS) | type;
    _updated = 1;
}

void
Chunk::removeBlock(uint16_t index)
{
    _block_chunk[index] = 0;
    _updated = 1;
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
                0,
                static_cast<float>(_chunk_position.y) * LINE_PER_PLANE);
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

void
Chunk::updateVbo()
{
    if (_updated) {
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferSubData(
          GL_ARRAY_BUFFER, 0, sizeof(uint8_t) * TOTAL_BLOCK, &_block_chunk[0]);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        _updated = 0;
    }
}

uint32_t
Chunk::getVao() const
{
    return (_vao);
}

void
Chunk::attachVaoVbo(glm::uvec2 const &pair)
{
    _vao = pair.x;
    _vbo = pair.y;
}

glm::uvec2
Chunk::detachVaoVbo()
{
    glm::uvec2 pair(_vao, _vbo);

    _vao = 0;
    _vbo = 0;
    return (pair);
}

void
Chunk::generateChunk()
{
    // TODO : Actual generation
    _debugGeneratePlane();
}

void
Chunk::_debugGeneratePlane()
{
    std::memset(&_block_chunk, 0, sizeof(uint8_t) * TOTAL_BLOCK);
    std::memset(&_block_chunk,
                DEBUG_BLOCK,
                sizeof(uint8_t) * BLOCK_PER_LINE * LINE_PER_PLANE);
    _updated = 1;
}