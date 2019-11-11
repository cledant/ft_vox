#include "Chunk.hpp"

#include <cstring>

BlockChunk const &
Chunk::getBlocks() const
{
    return (_block_chunk);
}

BlockChunkType const &
Chunk::getBlockTypes() const
{
    return (_block_types);
}

ChunkPosition const &
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
Chunk::setPosition(ChunkPosition const &pos)
{
    _position = pos;
}

void
Chunk::debugSetPlane(ChunkPosition const &chunk_position)
{
    std::memset(&_block_chunk, 0, sizeof(BlockChunk));
    std::memset(&_block_types, 0, sizeof(BlockChunkType));
    std::memset(
      &_block_chunk.plane[0], USHRT_MAX, sizeof(BlockLine) * LINE_PER_PLANE);
    std::memset(
      &_block_types.types[0], UCHAR_MAX, sizeof(uint8_t) * BLOCK_PER_LINE / 2);
    _state = ACTIVE;
    _position = chunk_position;
}