#include "ChunkManager.hpp"

ChunkManager::ChunkManager()
  : _current_render_distance(MIN_RENDER_DISTANCE)
  , _player_pos(0)
  , _chunk()
  , _chunk_map()
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
        _shader.setVec3("uniform_vec_chunk_position", it.getSpaceCoordinate());
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

uint32_t
ChunkManager::_allocate_vbo()
{
    uint32_t vbo;

    glGenBuffers(1, &vbo);
    if (!vbo) {
        return (0);
    }
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(
      GL_ARRAY_BUFFER, sizeof(uint8_t) * TOTAL_BLOCK, nullptr, GL_DYNAMIC_DRAW);
    if (glGetError() == GL_OUT_OF_MEMORY) {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &vbo);
        return (0);
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return (vbo);
}

uint32_t
ChunkManager::_allocate_vao(uint32_t vbo)
{
    uint32_t vao;

    glGenVertexArrays(1, &vao);
    if (!vao) {
        return (0);
    }
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribIPointer(
      0, 1, GL_UNSIGNED_BYTE, sizeof(uint8_t), reinterpret_cast<void *>(0));
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(0, 1);
    glBindVertexArray(0);
    return (vao);
}