#include "glad/glad.h"

#include <iostream>

#include "ChunkManager.hpp"

ChunkManager::ChunkManager()
  : _current_render_distance(MIN_RENDER_DISTANCE)
  , _current_player_block(STONE)
  , _player_pos(0)
  , _player_space_pos(0.0f)
  , _chunk()
  , _compute_chunk()
  , _chunk_map()
  , _shader()
  , _texture()
  , _nb_displayed_chunk(0)
  , _pn()
  , _ubo_texture_type()
  , _log_z_fighting()
{}

ChunkManager::~ChunkManager()
{
    if (!_ubo_texture_type) {
        glDeleteBuffers(1, &_ubo_texture_type);
    }
}

void
ChunkManager::init(uint64_t seed)
{
    _shader.init("./ressources/shaders/textured_chunk/textured_chunk_vs.glsl",
                 "./ressources/shaders/textured_chunk/textured_chunk_gs.glsl",
                 "./ressources/shaders/textured_chunk/textured_chunk_fs.glsl",
                 "Textured Chunk");
    _chunk.reserve((2 * MIN_RENDER_DISTANCE) * (2 * MIN_RENDER_DISTANCE));
    _texture.init("./ressources/textures/terrain.png", 1);
    _pn.setSeed(seed);
    _current_render_distance = MIN_RENDER_DISTANCE;
    _log_z_fighting = 1 / glm::log(C * FAR + OFFSET);
    _init_texture_type_ubo();
    _shader.use();
    _shader.setFloat("uniform_log_z_fighting", _log_z_fighting);
    _shader.setUbo("uniform_block_type",
                   0,
                   _ubo_texture_type,
                   sizeof(BlockTextureType) * NB_SHADER_TEXTURE);
    std::cout << sizeof(BlockTextureType) << std::endl;
}

void
ChunkManager::update(glm::vec3 const &player_pos)
{
    _player_pos = _get_chunk_coordinate(player_pos);
    _player_space_pos = player_pos;
    _remove_out_of_range_chunk();
    _add_available_chunk_to_viewable();
    _chunk_computation();
}

void
ChunkManager::draw(glm::mat4 const &projection,
                   std::array<glm::vec4, 6> const &frustum_planes,
                   std::array<glm::vec4, 6> const &abs_frustum_planes)
{
    _nb_displayed_chunk = 0;
    _shader.use();
    _shader.setMat4("uniform_mat_perspec_view", projection);
    _shader.setVec3("uniform_vec_camera_pos", _player_space_pos);
    for (auto &it : _chunk) {
        if (!it.isChunkInFrustum(frustum_planes, abs_frustum_planes)) {
            continue;
        }
        _shader.setVec3("uniform_vec_chunk_position", it.getSpaceCoordinate());
        _shader.setVec4("uniform_vec_color_modifier", it.getColorModifier());
        glBindVertexArray(it.getVao());
        glActiveTexture(GL_TEXTURE0);
        _shader.setInt("uniform_tex_diffuse", 0);
        glBindTexture(GL_TEXTURE_2D, _texture.getTextureID());
        glDrawArraysInstanced(GL_POINTS, 0, 1, it.getNbVisibleBlocks());
        ++_nb_displayed_chunk;
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}

void
ChunkManager::addBlock(glm::vec3 const &player_pos, glm::vec3 const &direction)
{
    Chunk *to_add_to = nullptr;
    uint16_t block_index = 0;
    float i = MIN_BLOCK_DISTANCE_ADD;
    uint8_t exit_loop = 0;

    while (i <= MAX_BLOCK_DISTANCE_INTERACTION) {
        auto targeted_pos = player_pos + direction * static_cast<float>(i);
        auto targeted_chunk_coord = _get_chunk_coordinate(targeted_pos);

        for (auto &it : _chunk) {
            if (it.getPosition() == targeted_chunk_coord) {
                auto tmp_block_index = it.getBlockIndex(targeted_pos);
                if (!it.getBlock(tmp_block_index)) {
                    block_index = tmp_block_index;
                    to_add_to = &it;
                } else {
                    exit_loop = 1;
                }
                break;
            }
        }
        if (exit_loop) {
            break;
        }
        i += 0.25f;
    }

    if (to_add_to &&
        !to_add_to->addBlock(block_index,
                             static_cast<BlockType>(_current_player_block)) &&
        !to_add_to->allocateGPUResources()) {
        to_add_to->updateGPUResources();
    }
}

void
ChunkManager::removeBlock(glm::vec3 const &player_pos,
                          glm::vec3 const &direction)
{
    float i = MIN_BLOCK_DISTANCE_REMOVE;
    while (i <= MAX_BLOCK_DISTANCE_INTERACTION) {
        auto targeted_pos = player_pos + direction * static_cast<float>(i);
        auto targeted_chunk_coord = _get_chunk_coordinate(targeted_pos);

        for (auto &it : _chunk) {
            if (it.getPosition() == targeted_chunk_coord) {
                if (!it.removeBlock(targeted_pos)) {
                    if (!it.allocateGPUResources()) {
                        it.updateGPUResources();
                    }
                    return;
                }
            }
        }
        i += 0.25f;
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
    if (_current_player_block >= TOTAL_BLOCKS) {
        _current_player_block = 1;
    }
}

void
ChunkManager::decreaseCurrentPlayerBlock()
{
    _current_player_block -= 1;
    if (_current_player_block <= 0) {
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

uint64_t
ChunkManager::getNbDisplayedChunk() const
{
    return (_nb_displayed_chunk);
}

int32_t
ChunkManager::getCurrentPlayerBlock() const
{
    return (_current_player_block);
}

uint32_t
ChunkManager::getSeed() const
{
    return (_pn.getSeed());
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
        _compute_chunk.emplace_back(std::async(
          std::launch::async, &ChunkManager::_generate_chunk, pos, _pn));
        _chunk_map[pos] = PENDING;
    }
    if (_compute_chunk.size() >= NB_ASYNC_THREAD) {
        return (1);
    }
    return (0);
}

Chunk
ChunkManager::_generate_chunk(glm::ivec2 pos, PerlinNoise pn)
{
    auto new_chunk = Chunk(pos);

    new_chunk.generateChunk(pn);
    return (new_chunk);
}

void
ChunkManager::_init_texture_type_ubo()
{
    glGenBuffers(1, &_ubo_texture_type);
    if (!_ubo_texture_type) {
        throw std::runtime_error("ChunkManager: Failed to create texture ubo");
    }
    glBindBuffer(GL_UNIFORM_BUFFER, _ubo_texture_type);
    glBufferData(GL_UNIFORM_BUFFER,
                 sizeof(BlockTextureType) * NB_SHADER_TEXTURE,
                 &SHADER_TEXTURE_TABLE[0],
                 GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

glm::ivec2
ChunkManager::_get_chunk_coordinate(const glm::vec3 &space_coord)
{
    glm::ivec2 chunk_coord;

    chunk_coord.x = static_cast<int32_t>(glm::round(space_coord.x)) /
                    static_cast<int32_t>(BLOCK_PER_LINE);
    chunk_coord.y = static_cast<int32_t>(glm::round(space_coord.z)) /
                    static_cast<int32_t>(LINE_PER_PLANE);

    // Out of bound checks for box
    if (!(static_cast<int32_t>(glm::round(space_coord.x)) %
          static_cast<int32_t>(BLOCK_PER_LINE)) &&
        space_coord.x < 0.0f) {
        chunk_coord.x += 1;
    }
    if (!(static_cast<int32_t>(glm::round(space_coord.z)) %
          static_cast<int32_t>(LINE_PER_PLANE)) &&
        space_coord.z < 0.0f) {
        chunk_coord.y += 1;
    }

    if (space_coord.x < 0.0f) {
        chunk_coord.x -= 1;
    }
    if (space_coord.z < 0.0f) {
        chunk_coord.y -= 1;
    }
    return (chunk_coord);
}