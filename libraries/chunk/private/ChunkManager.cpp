#include "ChunkManager.hpp"

ChunkManager::ChunkManager()
  : _current_render_distance(MIN_RENDER_DISTANCE)
  , _previous_render_distance(MIN_RENDER_DISTANCE)
  , _player_pos()
  , _chunk_state()
  , _chunk()
  , _shader()
  , _vao(0)
  , _vbo_pos(0)
  , _vbo_type(0)
{
    _chunk.reserve((2 * MAX_RENDER_DISTANCE) * (2 * MAX_RENDER_DISTANCE));
}

ChunkManager::~ChunkManager()
{
    if (_vao) {
        glDeleteVertexArrays(1, &_vao);
    }
    if (_vbo_pos) {
        glDeleteBuffers(1, &_vbo_pos);
    }
    if (_vbo_type) {
        glDeleteBuffers(1, &_vbo_type);
    }
}

void
ChunkManager::init()
{
    _shader.init("./ressources/shaders/chunk/chunk_vs.glsl",
                 "./ressources/shaders/chunk/chunk_gs.glsl",
                 "./ressources/shaders/chunk/chunk_fs.glsl",
                 "Chunk");
    _allocate_vbo(_vbo_pos,
                  sizeof(BlockChunk) * (2 * _current_render_distance) *
                    (2 * _current_render_distance));
    _allocate_vbo(_vbo_type,
                  sizeof(BlockChunkType) * (2 * _current_render_distance) *
                    (2 * _current_render_distance));
    _allocate_vao();
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
    _shader.setVec3("uniform_vec_chunk_size", CHUNK_SIZE);
    glBindVertexArray(_vao);
    glDrawArrays(GL_POINTS, 0, _chunk.size());
    glBindVertexArray(0);
}

void
ChunkManager::debugGeneratePlane()
{
    Chunk plane;

    plane.debugSetPlane(ChunkPosition{ 0, 0 });
    for (uint32_t i = 0; i < 10; ++i) {
        plane.setPosition(ChunkPosition{ i, 0 });
        _chunk.emplace_back(plane);
    }
}

void
ChunkManager::_allocate_vbo(uint32_t &vbo, uint64_t buff_size)
{
    glGenBuffers(1, &vbo);
    if (!vbo) {
        throw std::runtime_error("ChunkManager: Failed to create buffer");
    }
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, buff_size, nullptr, GL_DYNAMIC_DRAW);
    if (glGetError() == GL_OUT_OF_MEMORY) {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        throw std::runtime_error("ChunkManager: Failed to allocate buffer");
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void
ChunkManager::_allocate_vao()
{
    glGenVertexArrays(1, &_vao);
    if (!_vao) {
        throw std::runtime_error("ChunkManager: Failed to create vao");
    }
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo_pos);
    glVertexAttribPointer(0,
                          1,
                          GL_UNSIGNED_SHORT,
                          GL_FALSE,
                          sizeof(BlockChunk),
                          reinterpret_cast<void *>(0));
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo_type);
    glVertexAttribPointer(1,
                          GL_UNSIGNED_BYTE,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(float),
                          reinterpret_cast<void *>(sizeof(BlockChunk)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}