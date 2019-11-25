#include "ChunkManager.hpp"

ChunkManager::ChunkManager()
  : _current_render_distance(MIN_RENDER_DISTANCE)
  , _player_pos(0)
  , _chunk()
  , _gl_memory()
  , _compute_chunk()
  , _chunk_map()
  , _shader()
  , _nb_displayed_chunk(0)
{}

void
ChunkManager::init()
{
    _shader.init("./ressources/shaders/chunk/chunk_vs.glsl",
                 "./ressources/shaders/chunk/chunk_gs.glsl",
                 "./ressources/shaders/chunk/chunk_fs.glsl",
                 "Chunk");
    _chunk.reserve((2 * MIN_RENDER_DISTANCE) * (2 * MIN_RENDER_DISTANCE));
    _gl_memory.reserve(128);
}

void
ChunkManager::update(glm::vec3 const &player_pos)
{
    // Update Player position
    _player_pos.x =
      static_cast<int32_t>(player_pos.x) / static_cast<int32_t>(BLOCK_PER_LINE);
    _player_pos.y =
      static_cast<int32_t>(player_pos.z) / static_cast<int32_t>(LINE_PER_PLANE);
    if (player_pos.x < 0.0f) {
        _player_pos.x -= 1;
    }
    if (player_pos.z < 0.0f) {
        _player_pos.y -= 1;
    }
    _remove_out_of_range_chunk();
    _add_available_chunk_to_viewable();
    _chunk_computation();
}

void
ChunkManager::draw(glm::mat4 const &projection,
                   std::array<glm::vec4, 6> const &frustum_planes)
{
    glCullFace(GL_FRONT);
    _nb_displayed_chunk = 0;
    _shader.use();
    _shader.setMat4("uniform_mat_perspec_view", projection);
    for (auto &it : _chunk) {
        if (!it.isChunkInFrustum(frustum_planes)) {
            continue;
        }
        _shader.setVec3("uniform_vec_chunk_position", it.getSpaceCoordinate());
        it.updateVbo();
        glBindVertexArray(it.getVao());
        glDrawArraysInstanced(GL_POINTS, 0, 1, TOTAL_BLOCK);
        ++_nb_displayed_chunk;
    }
    glBindVertexArray(0);
    glCullFace(GL_BACK);
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

glm::ivec2 const &
ChunkManager::getPlayerPosition() const
{
    return (_player_pos);
}

uint64_t
ChunkManager::getNbInRangeChunks() const
{
    return (_chunk.size());
}

uint64_t
ChunkManager::getNbDisplayedChunk() const
{
    return (_nb_displayed_chunk);
}

uint8_t
ChunkManager::_is_chunk_out_of_range(glm::ivec2 const &pos) const
{
    if (abs(_player_pos.x - pos.x) > _current_render_distance) {
        return (1);
    }
    if (abs(_player_pos.y - pos.y) > _current_render_distance) {
        return (1);
    }
    return (0);
}

void
ChunkManager::_remove_out_of_range_chunk()
{
    for (auto it = _chunk.begin(); it != _chunk.end();) {
        auto pos = it->getPosition();

        if (_is_chunk_out_of_range(pos)) {
            _gl_memory.emplace_back(it->detachVaoVbo());
            _chunk_map[pos] = DELETED;
            std::swap(*it, _chunk.back());
            _chunk.pop_back();
        } else {
            ++it;
        }
    }
}

void
ChunkManager::_add_available_chunk_to_viewable()
{
    for (auto it = _compute_chunk.begin(); it != _compute_chunk.end();) {
        if (it->wait_for(std::chrono::nanoseconds(10)) ==
            std::future_status::ready) {

            // Adding new chunk to vector if it's in range and got opengl memory
            // available
            Chunk new_chunk = it->get();
            if (!_is_chunk_out_of_range(new_chunk.getPosition())) {

                // Using already allocated memory or allocating new one
                glm::uvec2 gl_mem(0);
                if (!_gl_memory.empty()) {
                    new_chunk.attachVaoVbo(_gl_memory.back());
                    _gl_memory.pop_back();
                    _chunk_map[new_chunk.getPosition()] = VISIBLE;
                    _chunk.emplace_back(std::move(new_chunk));
                } else if (!_allocate_gl_memory(gl_mem)) {
                    new_chunk.attachVaoVbo(gl_mem);
                    _chunk_map[new_chunk.getPosition()] = VISIBLE;
                    _chunk.emplace_back(std::move(new_chunk));
                } else {
                    _chunk_map[new_chunk.getPosition()] = DELETED;
                }
            } else {
                _chunk_map[new_chunk.getPosition()] = DELETED;
            }
            std::swap(*it, _compute_chunk.back());
            _compute_chunk.pop_back();
        } else {
            ++it;
        }
    }
}

void
ChunkManager::_chunk_computation()
{
    if (_compute_chunk.size() >= NB_ASYNC_THREAD) {
        return;
    }
    if (_add_new_chunk(_player_pos)) {
        return;
    }
    for (int32_t i = 1; i <= _current_render_distance; ++i) {
        // Upper Line + Lower Line
        for (int32_t j = _player_pos.x - i; j <= _player_pos.x + i; ++j) {
            if (_add_new_chunk(glm::ivec2(j, _player_pos.y + i))) {
                return;
            }
            if (_add_new_chunk(glm::ivec2(j, _player_pos.y - i))) {
                return;
            }
        }
        // Right Side + Left Side
        for (int32_t j = _player_pos.y - (i - 1); j <= _player_pos.y + (i - 1);
             ++j) {
            if (_add_new_chunk(glm::ivec2(_player_pos.x + i, j))) {
                return;
            }
            if (_add_new_chunk(glm::ivec2(_player_pos.x - i, j))) {
                return;
            }
        }
    }
}

uint8_t
ChunkManager::_add_new_chunk(glm::ivec2 const &pos)
{
    if (_chunk_map[pos] == DELETED) {
        _compute_chunk.emplace_back(
          std::async(std::launch::async, &ChunkManager::_generate_chunk, pos));
        _chunk_map[pos] = PENDING;
    }
    if (_compute_chunk.size() >= NB_ASYNC_THREAD) {
        return (1);
    }
    return (0);
}

Chunk
ChunkManager::_generate_chunk(glm::ivec2 pos)
{
    auto new_chunk = Chunk(pos);

    new_chunk.generateChunk();
    return (new_chunk);
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

uint8_t
ChunkManager::_allocate_gl_memory(glm::uvec2 &memory)
{
    if (!(memory.y = _allocate_vbo())) {
        return (1);
    }
    if (!(memory.x = _allocate_vao(memory.y))) {
        glDeleteBuffers(1, &memory.y);
        return (1);
    }
    return (0);
}