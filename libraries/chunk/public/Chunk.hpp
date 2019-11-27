#ifndef FT_VOX_CHUNK_HPP
#define FT_VOX_CHUNK_HPP

#include <array>

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
    explicit Chunk(glm::ivec2 const &chunk_position);

    void addBlock(uint16_t index, BlockType type);
    void removeBlock(uint16_t index);
    [[nodiscard]] uint8_t getBlock(uint16_t index) const;

    void setPosition(glm::ivec2 const &pos);
    [[nodiscard]] glm::ivec2 const &getPosition() const;
    [[nodiscard]] glm::vec3 const &getSpaceCoordinate() const;

    void updateVbo();
    [[nodiscard]] uint32_t getVao() const;
    void attachVaoVbo(glm::uvec2 const &pair);
    glm::uvec2 detachVaoVbo();

    void generateChunk();
    [[nodiscard]] uint8_t isChunkInFrustum(
      glm::mat4 const &proj_view,
      std::array<glm::vec4, 6> const &frustum_planes) const;

  private:
    void _debugGeneratePlane();

    uint8_t _block_chunk[TOTAL_BLOCK];
    glm::ivec2 _chunk_position;
    glm::vec3 _space_coord;
    glm::vec3 _center;
    uint8_t _updated;
    uint32_t _vao;
    uint32_t _vbo;
};

#endif // FT_VOX_CHUNK_HPP
