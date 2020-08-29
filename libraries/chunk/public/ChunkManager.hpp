#ifndef FT_VOX_CHUNKMANAGER_HPP
#define FT_VOX_CHUNKMANAGER_HPP

#include <cstdint>
#include <vector>
#include <unordered_map>
#include <future>

#include "glm/glm.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"

#include "Chunk.hpp"
#include "ChunkDataStructures.hpp"
#include "ChunkShaderDataStructure.hpp"
#include "GLShader.hpp"
#include "GLTexture2D.hpp"
#include "PerlinNoise.hpp"
#include "NoiseUtils.hpp"

class ChunkManager final
{
  public:
    ChunkManager();
    ~ChunkManager();
    ChunkManager(const ChunkManager &src) = delete;
    ChunkManager &operator=(ChunkManager const &rhs) = delete;
    ChunkManager(ChunkManager &&src) = delete;
    ChunkManager &operator=(ChunkManager &&rhs) = delete;

    void init(uint64_t seed);
    void update(glm::vec3 const &player_pos);
    void draw(glm::mat4 const &projection,
              std::array<glm::vec4, 6> const &frustum_planes,
              std::array<glm::vec4, 6> const &abs_frustum_planes);
    void addBlock(glm::vec3 const &player_pos, glm::vec3 const &direction);
    void removeBlock(glm::vec3 const &player_pos, glm::vec3 const &direction);

    void increaseRenderDistance();
    void decreaseRenderDistance();

    void increaseCurrentPlayerBlock();
    void decreaseCurrentPlayerBlock();

    [[nodiscard]] uint64_t getRenderDistance() const;
    glm::ivec2 const &getPlayerPosition() const;
    [[nodiscard]] uint64_t getNbInRangeChunks() const;
    [[nodiscard]] uint64_t getNbDisplayedChunk() const;
    [[nodiscard]] int32_t getCurrentPlayerBlock() const;
    [[nodiscard]] uint32_t getSeed() const;

  private:
    static constexpr int32_t const MIN_RENDER_DISTANCE = 10;
    static constexpr int32_t const MAX_RENDER_DISTANCE = 128;
    static constexpr uint32_t const NB_ASYNC_THREAD = 64;
    static constexpr float const MAX_BLOCK_DISTANCE_INTERACTION = 5.0f;
    static constexpr float const MIN_BLOCK_DISTANCE_ADD = 1.5f;
    static constexpr float const MIN_BLOCK_DISTANCE_REMOVE = 0.25f;

    int32_t _current_render_distance;
    int32_t _current_player_block;
    glm::ivec2 _player_pos;
    glm::vec3 _player_space_pos;

    std::vector<Chunk> _chunk;
    std::vector<std::future<Chunk>> _compute_chunk;
    std::unordered_map<glm::ivec2, ChunkState> _chunk_map;
    GLShader _shader;
    GLTexture2D _texture;

    uint64_t _nb_displayed_chunk;

    PerlinNoise _pn;

    uint32_t _ubo_texture_type;
    float _log_z_fighting;

    inline uint8_t _is_chunk_out_of_range(glm::ivec2 const &pos) const;
    inline void _remove_out_of_range_chunk();
    inline void _add_available_chunk_to_viewable();
    inline void _chunk_computation();
    inline void _init_texture_type_ubo();
    inline uint8_t _add_new_chunk(glm::ivec2 const &pos);
    static inline Chunk _generate_chunk(glm::ivec2 pos, PerlinNoise pn);
    static inline glm::ivec2 _get_chunk_coordinate(
      glm::vec3 const &space_coord);
};

#endif // FT_VOX_CHUNKMANAGER_HPP
