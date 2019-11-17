#include "ChunkManager.hpp"

ChunkManager::ChunkManager()
  : _current_render_distance(MIN_RENDER_DISTANCE)
  , _player_pos(0)
  , _chunk()
  , _shader()
{}

void
ChunkManager::init()
{
    _shader.init("./ressources/shaders/chunk/chunk_vs.glsl",
                 "./ressources/shaders/chunk/chunk_gs.glsl",
                 "./ressources/shaders/chunk/chunk_fs.glsl",
                 "Chunk");
    _chunk.reserve((2 * MIN_RENDER_DISTANCE) * (2 * MIN_RENDER_DISTANCE));
}

void
ChunkManager::update(glm::vec3 const &player_pos)
{
    _player_pos.x =
      static_cast<uint64_t>(std::fmod(player_pos.x, CHUNK_SIZE.x));
    _player_pos.y =
      static_cast<uint64_t>(std::fmod(player_pos.z, CHUNK_SIZE.y));
}

void
ChunkManager::draw(glm::mat4 const &projection)
{
    _shader.use();
    _shader.setMat4("uniform_mat_perspec_view", projection);
    for (auto &it : _chunk) {
        _shader.setVec3("uniform_vec_chunk_position", it.getPosition());
        it.updateVbo();
        glBindVertexArray(it.getVao());
        glDrawArraysInstanced(GL_POINTS, 0, 1, TOTAL_BLOCK);
    }
    glBindVertexArray(0);
}

void
ChunkManager::addBlock(glm::vec3 const &player_pos, glm::vec3 const &direction)
{
    // TODO
    (void)player_pos;
    (void)direction;
}

void
ChunkManager::removeBlock(glm::vec3 const &player_pos,
                          glm::vec3 const &direction)
{
    // TODO
    (void)player_pos;
    (void)direction;
}

void
ChunkManager::increaseRenderDistance()
{
    if (_current_render_distance < MAX_RENDER_DISTANCE) {
        _current_render_distance += 2;
    }
}

void
ChunkManager::decreaseRenderDistance()
{
    if (_current_render_distance > MIN_RENDER_DISTANCE) {
        _current_render_distance -= 2;
    }
}

uint64_t
ChunkManager::getRenderDistance() const
{
    return (_current_render_distance);
}

void
ChunkManager::debugGeneratePlane()
{
    for (uint32_t i = 0; i < 3; ++i) {
        for (uint32_t j = 0; j < 3; ++j) {
            Chunk tmp(glm::vec3(i * BLOCK_PER_LINE, 0, j * LINE_PER_PLANE));

            tmp.debugInitAsPlane();
            _chunk.emplace_back(std::move(tmp));
        }
    }
}