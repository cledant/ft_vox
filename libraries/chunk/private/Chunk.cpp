#include "Chunk.hpp"

#include <cstring>
#include <stdexcept>

#include "glad/glad.h"

Chunk::Chunk()
  : _block_chunk()
  , _position(0.0f)
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
  , _position(0.0f)
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
    _position = rhs._position;
    _updated = rhs._updated;
    _vao = rhs._vao;
    _vbo = rhs._vbo;
    rhs._vao = 0;
    rhs._vbo = 0;
    return (*this);
}

Chunk::Chunk(glm::vec3 const &chunk_position)
  : _block_chunk()
  , _position(chunk_position)
  , _updated(0)
  , _vao(0)
  , _vbo(0)
{}

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
Chunk::setPosition(glm::vec3 const &pos)
{
    _position = pos;
}

glm::vec3 const &
Chunk::getPosition() const
{
    return (_position);
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
Chunk::debugInitAsPlane()
{
    std::memset(&_block_chunk, 0, sizeof(uint8_t) * TOTAL_BLOCK);
    std::memset(&_block_chunk,
                DEBUG_BLOCK,
                sizeof(uint8_t) * BLOCK_PER_LINE * LINE_PER_PLANE);
    _debug_allocate_vbo();
    _debug_allocate_vao();
    _updated = 1;
}

void
Chunk::_debug_allocate_vbo()
{
    glGenBuffers(1, &_vbo);
    if (!_vbo) {
        throw std::runtime_error("Chunk Debug: Failed to create vbo");
    }
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(
      GL_ARRAY_BUFFER, sizeof(uint8_t) * TOTAL_BLOCK, nullptr, GL_DYNAMIC_DRAW);
    if (glGetError() == GL_OUT_OF_MEMORY) {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        throw std::runtime_error("Chunk: Failed to allocate buffer");
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void
Chunk::_debug_allocate_vao()
{
    glGenVertexArrays(1, &_vao);
    if (!_vao) {
        throw std::runtime_error("Chunk Debug: Failed to create vao");
    }
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glVertexAttribIPointer(
      0, 1, GL_UNSIGNED_BYTE, sizeof(uint8_t), reinterpret_cast<void *>(0));
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(0, 1);
    glBindVertexArray(0);
}