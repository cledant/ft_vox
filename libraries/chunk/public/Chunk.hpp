#ifndef FT_VOX_CHUNK_HPP
#define FT_VOX_CHUNK_HPP

#include "ChunkDataStructures.hpp"

class Chunk
{
  public:
    Chunk();
    virtual ~Chunk();
    Chunk(const Chunk &src) = delete;
    Chunk &operator=(Chunk const &rhs) = delete;
    Chunk(Chunk &&src) noexcept;
    Chunk &operator=(Chunk &&rhs) noexcept;
    explicit Chunk(glm::vec3 const &chunk_position);

    void addBlock(uint16_t index, BlockType type);
    void removeBlock(uint16_t index);
    [[nodiscard]] uint8_t getBlock(uint16_t index) const;

    void setPosition(glm::vec3 const &pos);
    [[nodiscard]] glm::vec3 const &getPosition() const;

    void updateVbo();
    [[nodiscard]] uint32_t getVao() const;

    void debugInitAsPlane();

  private:
    void _debug_allocate_vbo();
    void _debug_allocate_vao();

    uint8_t _block_chunk[TOTAL_BLOCK];
    glm::vec3 _position;
    uint8_t _updated;
    uint32_t _vao;
    uint32_t _vbo;
};

#endif // FT_VOX_CHUNK_HPP
