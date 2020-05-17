#ifndef FT_VOX_CHUNK_HPP
#define FT_VOX_CHUNK_HPP

#include <array>

#include "ChunkDataStructures.hpp"

class Chunk final
{
  public:
    Chunk();
    ~Chunk();
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

    [[nodiscard]] uint32_t getVao() const;
    [[nodiscard]] uint32_t getNbVisibleBlocks() const;

    void generateChunk();
    uint8_t allocateGPUResources();
    void updateGPUResources() const;

    [[nodiscard]] uint8_t isChunkInFrustum(
      std::array<glm::vec4, 6> const &frustum_planes,
      std::array<glm::vec4, 6> const &abs_frustum_planes) const;

  private:
    // CPU generation related
    inline void _debug_generate_plane();
    inline void _generate_visible_blocks_buffer();
    inline uint8_t _compute_block_visible_faces(int32_t index);

    // GPU allocation related
    inline uint8_t _allocate_vbo();
    inline uint8_t _allocate_vao();
    inline void _deallocate_gpu();

    /*
     * Buffers that will be used for block type generation
     */
    uint8_t _block_chunk[TOTAL_BLOCK];

    /*
     * Buffers that will be used for vbo (GPU)
     * Uint32 is map this way :
     * | 16 bits: A | 2 bits = B | 6 bits = C | 3 bits = D | 5 bits = E |
     * A = Block ID
     * B = Unused
     * C = Visible face
     * D = Unused
     * E = Block Type
     */
    uint32_t _visible_blocks[TOTAL_BLOCK];
    uint32_t _nb_visible_blocks;

    glm::ivec2 _chunk_position;
    glm::vec3 _space_coord;
    glm::vec3 _center;

    uint32_t _vao;
    uint32_t _vbo;
};

#endif // FT_VOX_CHUNK_HPP
