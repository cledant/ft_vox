#ifndef FT_VOX_CHUNKMANAGER_HPP
#define FT_VOX_CHUNKMANAGER_HPP

#include <cstdint>
#include <vector>
#include <unordered_map>

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

  private:
    static constexpr uint32_t MIN_RENDER_DISTANCE = 16;
    static constexpr uint32_t MAX_RENDER_DISTANCE = 128;
    static constexpr uint32_t NB_ASYNC_THREAD = 32;

    uint32_t _current_render_distance;

    glm::ivec2 _player_pos;

    // Chunk lists
    std::vector<Chunk> _chunk;
    std::unordered_map<glm::ivec2, ChunkState> _chunk_map;
    GLShader _shader;

    static uint32_t _allocate_vbo();
    static uint32_t _allocate_vao(uint32_t vbo);
};

#endif // FT_VOX_CHUNKMANAGER_HPP
