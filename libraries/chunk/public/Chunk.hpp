#ifndef FT_VOX_CHUNK_HPP
#define FT_VOX_CHUNK_HPP

#include <array>
#include <memory>

#include "ChunkDataStructures.hpp"
#include "PerlinNoise.hpp"

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

    uint8_t addBlock(uint16_t index, BlockType type);
    uint8_t addBlock(glm::vec3 const &pos, BlockType type);
    uint8_t removeBlock(uint16_t index);
    uint8_t removeBlock(glm::vec3 const &pos);
    [[nodiscard]] uint8_t getBlock(uint16_t index) const;
    [[nodiscard]] uint8_t getBlock(glm::vec3 const &pos) const;
    [[nodiscard]] uint16_t getBlockIndex(glm::vec3 const &pos) const;

    void setPosition(glm::ivec2 const &pos);
    [[nodiscard]] glm::ivec2 const &getPosition() const;
    [[nodiscard]] glm::vec3 const &getSpaceCoordinate() const;

    [[nodiscard]] uint32_t getVao() const;
    [[nodiscard]] uint32_t getNbVisibleBlocks() const;
    [[nodiscard]] uint32_t getIndirectCommandVbo() const;

    void generateChunk(PerlinNoise const &pn);
    uint8_t allocateGPUResources();
    void updateGPUResources();

    [[nodiscard]] uint8_t isChunkInFrustum(
      std::array<glm::vec4, 6> const &frustum_planes,
      std::array<glm::vec4, 6> const &abs_frustum_planes) const;

  private:
    // CPU generation related
    inline void _generate_with_seed(PerlinNoise const &pn);
    inline void _fill_block_chunk(int32_t x,
                                  int32_t y,
                                  glm::vec2 const &elevation_temperature,
                                  glm::vec2 const &cave_limits);
    inline void _debug_generate_plane();
    inline void _debug_generate_blocks();
    inline void _generate_visible_blocks_buffer();
    inline uint8_t _compute_block_visible_faces(int32_t index);

    // GPU allocation related
    inline uint8_t _allocate_vbo();
    inline uint8_t _allocate_vao();
    inline void _deallocate_gpu();

    static inline int32_t _get_index_from_pos(glm::vec3 const &pos);

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
    std::unique_ptr<uint32_t[]> _visible_blocks;
    uint32_t _nb_visible_blocks;

    glm::ivec2 _chunk_position;
    glm::vec3 _space_coord;
    glm::vec3 _center;

    uint32_t _vao;
    uint32_t _vbo_blocks;
    uint32_t _vbo_space_coord;
    uint32_t _vbo_indirect;
};

#endif // FT_VOX_CHUNK_HPP
