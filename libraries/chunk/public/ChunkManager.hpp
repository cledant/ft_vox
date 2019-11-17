#ifndef FT_VOX_CHUNKMANAGER_HPP
#define FT_VOX_CHUNKMANAGER_HPP

#include <cstdint>
#include <vector>
#include <map>

#include "glm/glm.hpp"

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

    void debugGeneratePlane();

  private:
    static constexpr uint64_t MIN_RENDER_DISTANCE = 16;
    static constexpr uint64_t MAX_RENDER_DISTANCE = 128;

    uint64_t _current_render_distance;

    glm::ivec2 _player_pos;

    // Chunk lists
    std::vector<Chunk> _chunk;
    GLShader _shader;
};

#endif // FT_VOX_CHUNKMANAGER_HPP
