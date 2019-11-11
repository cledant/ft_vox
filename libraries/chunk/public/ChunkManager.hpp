#ifndef FT_VOX_CHUNKMANAGER_HPP
#define FT_VOX_CHUNKMANAGER_HPP

#include <cstdint>
#include <vector>
#include <map>

#include "glm/glm.hpp"

#include "glad/glad.h"

#include "Chunk.hpp"
#include "GLShader.hpp"

class ChunkManager
{
  public:
    ChunkManager();
    virtual ~ChunkManager();
    ChunkManager(const ChunkManager &src) = delete;
    ChunkManager &operator=(ChunkManager const &rhs) = delete;
    ChunkManager(ChunkManager &&src) = delete;
    ChunkManager &operator=(ChunkManager &&rhs) = delete;

    void init();
    void update(glm::vec3 const &player_pos);
    void draw(glm::mat4 const &projection);
    void addBlock(glm::vec3 const &player_pos, glm::vec3 const &direction);
    void removeBlock(glm::vec3 const &player_pos, glm::vec3 const &direction);

    void debugGeneratePlane();

  private:
    static constexpr uint64_t MIN_RENDER_DISTANCE = 10;
    static constexpr uint64_t MAX_RENDER_DISTANCE = 64;

    void _allocate_vbo(uint32_t &vbo, uint64_t buff_size);
    void _allocate_vao();

    uint64_t _current_render_distance;
    uint64_t _previous_render_distance;

    ChunkPosition _player_pos;

    // Chunk lists
    std::map<ChunkPosition, ChunkState> _chunk_state;
    std::vector<Chunk> _chunk;
    GLShader _shader;
    uint32_t _vao;
    uint32_t _vbo_pos;
    uint32_t _vbo_type;
};

#endif // FT_VOX_CHUNKMANAGER_HPP
