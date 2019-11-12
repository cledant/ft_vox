#ifndef FT_VOX_CHUNK_HPP
#define FT_VOX_CHUNK_HPP

#include "ChunkDataStructures.hpp"

class Chunk
{
  public:
    Chunk();
    virtual ~Chunk() = default;
    Chunk(const Chunk &src) = default;
    Chunk &operator=(Chunk const &rhs) = default;
    Chunk(Chunk &&src) = default;
    Chunk &operator=(Chunk &&rhs) = default;

    void addBlock(uint16_t index, BlockType type);
    void removeBlock(uint16_t index);
    [[nodiscard]] uint8_t getBlock(uint16_t index) const;

    void setPosition(glm::ivec2 const &pos);
    [[nodiscard]] glm::ivec2 const &getPosition() const;

    [[nodiscard]] ChunkState getState() const;

    void debugSetPlane(glm::ivec2 const &chunk_position);

  private:
    uint8_t _block_chunk[TOTAL_BLOCK];
    glm::ivec2 _position;
    ChunkState _state;
};

#endif // FT_VOX_CHUNK_HPP
