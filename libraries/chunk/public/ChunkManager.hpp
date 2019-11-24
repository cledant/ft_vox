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
#include "GLShader.hpp"

class ChunkManager
{
  public:
    ChunkManager();
    virtual ~ChunkManager() = default;
    ChunkManager(const ChunkManager &src) = delete;
    ChunkManager &operator=(ChunkManager const &rhs) = delete;
    ChunkManager(ChunkManager &&src) = delete;
    ChunkManager &operator=(ChunkManager &&rhs) = delete;

    void init();
    void update(glm::vec3 const &player_pos);
    void draw(glm::mat4 const &projection);
    void addBlock(glm::vec3 const &player_pos, glm::vec3 const &direction);
    void removeBlock(glm::vec3 const &player_pos, glm::vec3 const &direction);

    void increaseRenderDistance();
    void decreaseRenderDistance();
    [[nodiscard]] uint64_t getRenderDistance() const;

    glm::ivec2 const &getPlayerPosition() const;
    uint64_t getNbInRangeChunks() const;

    uint64_t getNbDisplayedChunk() const;

  private:
    static constexpr int32_t MIN_RENDER_DISTANCE = 1;
    static constexpr int32_t MAX_RENDER_DISTANCE = 128;
    static constexpr uint32_t NB_ASYNC_THREAD = 32;

    int32_t _current_render_distance;

    glm::ivec2 _player_pos;

    std::vector<Chunk> _chunk;
    std::vector<glm::uvec2> _gl_memory;
    std::vector<std::future<Chunk>> _compute_chunk;
    std::unordered_map<glm::ivec2, ChunkState> _chunk_map;
    GLShader _shader;

    uint64_t _nb_displayed_chunk;

    inline uint8_t _is_chunk_out_of_range(glm::ivec2 const &pos) const;
    inline void _remove_out_of_range_chunk();
    inline void _add_available_chunk_to_viewable();
    inline void _chunk_computation();
    inline uint8_t _add_new_chunk(glm::ivec2 const &pos);
    static inline Chunk _generate_chunk(glm::ivec2 pos);

    static uint32_t _allocate_vbo();
    static uint32_t _allocate_vao(uint32_t vbo);
    static uint8_t _allocate_gl_memory(glm::uvec2 &memory);
};

#endif // FT_VOX_CHUNKMANAGER_HPP
