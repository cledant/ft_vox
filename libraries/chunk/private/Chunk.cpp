#include "Chunk.hpp"

#include <cstring>

Chunk::Chunk()
  : _block_chunk()
  , _position(0.0f)
  , _state(DELETED)
{}

uint8_t
Chunk::getBlock(uint16_t index) const
{
    return (_block_chunk[index]);
}

void
Chunk::setPosition(glm::ivec2 const &pos)
{
    _position = pos;
}

glm::ivec2 const &
Chunk::getPosition() const
{
    return (_position);
}

ChunkState
Chunk::getState() const
{
    return (_state);
}

void
Chunk::debugSetPlane(glm::ivec2 const &chunk_position)
{
    std::memset(&_block_chunk, 0, sizeof(uint8_t) * TOTAL_BLOCK);
    std::memset(&_block_chunk,
                DEBUG_BLOCK,
                sizeof(uint8_t) * BLOCK_PER_LINE * BLOCK_PER_LINE);
    _state = ACTIVE;
    _position = chunk_position;
}