#ifndef FT_VOX_CHUNK_HPP
#define FT_VOX_CHUNK_HPP

#include "ChunkDataStructures.hpp"

class Chunk
{
  public:
    Chunk() = default;
    virtual ~Chunk() = default;
    Chunk(const Chunk &src) = default;
    Chunk &operator=(Chunk const &rhs) = default;
    Chunk(Chunk &&src) = default;
    Chunk &operator=(Chunk &&rhs) = default;

    [[nodiscard]] BlockChunk const &getBlocks() const;
    [[nodiscard]] BlockChunkType const &getBlockTypes() const;
    [[nodiscard]] ChunkPosition const &getPosition() const;
    [[nodiscard]] ChunkState getState() const;

    void addBlock(uint16_t index);
    void removeBlock(uint16_t index);
    int32_t getBlockState(uint16_t index) const;

    void setType(uint32_t value, uint16_t chunk_index);
    uint32_t getType(uint16_t chunk_index) const;

    void setPosition(ChunkPosition const &pos);

    void debugSetPlane(ChunkPosition const &chunk_position);

  private:
    BlockChunk _block_chunk;
    BlockChunkType _block_types;
    ChunkPosition _position;
    ChunkState _state;
};

#endif // FT_VOX_CHUNK_HPP
