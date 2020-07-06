#include "glad/glad.h"

#include <iostream>

#include "ChunkManager.hpp"

ChunkManager::ChunkManager()
  : _current_render_distance(MIN_RENDER_DISTANCE)
  , _current_player_block(STONE)
  , _player_pos(0)
  , _chunk()
  , _compute_chunk()
  , _chunk_map()
  , _shader()
  , _texture()
  , _nb_displayed_chunk(0)
{}

void
ChunkManager::init()
{
    _shader.init("./ressources/shaders/textured_chunk/textured_chunk_vs.glsl",
                 "./ressources/shaders/textured_chunk/textured_chunk_gs.glsl",
                 "./ressources/shaders/textured_chunk/textured_chunk_fs.glsl",
                 "Textured Chunk");
    _chunk.reserve((2 * MIN_RENDER_DISTANCE) * (2 * MIN_RENDER_DISTANCE));
    _texture.init("./ressources/textures/terrain.png", 1);
}

void
ChunkManager::update(glm::vec3 const &player_pos)
{
    _player_pos = _get_chunk_coordinate(player_pos);
    _remove_out_of_range_chunk();
    _add_available_chunk_to_viewable();
    _chunk_computation();
}

void
ChunkManager::draw(glm::mat4 const &projection,
                   std::array<glm::vec4, 6> const &frustum_planes,
                   std::array<glm::vec4, 6> const &abs_frustum_planes)
{
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    _nb_displayed_chunk = 0;
    _shader.use();
    _shader.setMat4("uniform_mat_perspec_view", projection);
    for (auto &it : _chunk) {
        if (!it.isChunkInFrustum(frustum_planes, abs_frustum_planes)) {
            continue;
        }
        _shader.setVec3("uniform_vec_chunk_position", it.getSpaceCoordinate());
        _shader.setVec4("uniform_vec_color_modifier", it.getColorModifier());
        glBindVertexArray(it.getVao());
        glBindTexture(GL_TEXTURE_2D, _texture.getTextureID());
        glDrawArraysInstanced(GL_POINTS, 0, 1, it.getNbVisibleBlocks());
        ++_nb_displayed_chunk;
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glCullFace(GL_BACK);
    glDisable(GL_CULL_FACE);
}

void
ChunkManager::addBlock(glm::vec3 const &player_pos,
                       glm::vec3 const &direction,
                       BlockType type)
{
    auto targeted_pos = player_pos + direction;
    auto targeted_chunk_coord = _get_chunk_coordinate(targeted_pos);

    for (auto &it : _chunk) {
        if (it.getPosition() == targeted_chunk_coord) {
            if (!it.addBlock(targeted_pos, type) &&
                !it.allocateGPUResources()) {
                it.updateGPUResources();
            }
            break;
        }
    }
}

void
ChunkManager::addBlock(glm::vec3 const &player_pos, glm::vec3 const &direction)
{
    auto targeted_pos = player_pos + direction;
    auto targeted_chunk_coord = _get_chunk_coordinate(targeted_pos);

    for (auto &it : _chunk) {
        if (it.getPosition() == targeted_chunk_coord) {
            if (!it.addBlock(targeted_pos,
                             static_cast<BlockType>(_current_player_block)) &&
                !it.allocateGPUResources()) {
                it.updateGPUResources();
            }
            break;
        }
    }
}

void
ChunkManager::removeBlock(glm::vec3 const &player_pos,
                          glm::vec3 const &direction)
{
    auto targeted_pos = player_pos + direction;
    auto targeted_chunk_coord = _get_chunk_coordinate(targeted_pos);

    for (auto &it : _chunk) {
        if (it.getPosition() == targeted_chunk_coord) {
            if (!it.removeBlock(targeted_pos) && !it.allocateGPUResources()) {
                it.updateGPUResources();
            }
            break;
        }
    }
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

void
ChunkManager::increaseCurrentPlayerBlock()
{
    _current_player_block += 1;
    if (_current_player_block == TOTAL_BLOCKS) {
        _current_player_block = 1;
    }
}

void
ChunkManager::decreaseCurrentPlayerBlock()
{
    _current_player_block += 1;
    if (!_current_player_block) {
        _current_player_block = TOTAL_BLOCKS - 1;
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

int32_t
ChunkManager::getCurrentPlayerBlock() const
{
    return (_current_player_block);
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
        if (it->wait_for(std::chrono::nanoseconds(20)) ==
            std::future_status::ready) {

            // Adding new chunk to vector if it's in range and GPU resources
            // allocation is successful
            auto new_chunk = it->get();
            if (!_is_chunk_out_of_range(new_chunk.getPosition()) &&
                !new_chunk.allocateGPUResources()) {
                new_chunk.updateGPUResources();
                _chunk_map[new_chunk.getPosition()] = VISIBLE;
                _chunk.emplace_back(std::move(new_chunk));
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

glm::ivec2
ChunkManager::_get_chunk_coordinate(const glm::vec3 &space_coord)
{
    glm::ivec2 chunk_coord;

    chunk_coord.x = static_cast<int32_t>(space_coord.x) /
                    static_cast<int32_t>(BLOCK_PER_LINE);
    chunk_coord.y = static_cast<int32_t>(space_coord.z) /
                    static_cast<int32_t>(LINE_PER_PLANE);
    if (space_coord.x < 0.0f) {
        chunk_coord.x -= 1;
    }
    if (space_coord.z < 0.0f) {
        chunk_coord.y -= 1;
    }
    return (chunk_coord);
}