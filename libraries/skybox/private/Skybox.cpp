#include <stdexcept>

#include "glad/glad.h"

#include "Skybox.hpp"

Skybox::Skybox(Skybox &&src)
{
    *this = std::move(src);
}

Skybox &
Skybox::operator=(Skybox &&rhs)
{
    _vao = rhs._vao;
    _vbo = rhs._vbo;
    rhs._vao = 0;
    rhs._vbo = 0;
    _shader = std::move(rhs._shader);
    _texture = std::move(rhs._texture);
    return (*this);
}

void
Skybox::init(const std::string &skybox_filepath)
{
    _texture.loadTexture(skybox_filepath.c_str());
    _shader.init("./ressources/shaders/skybox/skybox_vs.glsl",
                 "./ressources/shaders/skybox/skybox_gs.glsl",
                 "./ressources/shaders/skybox/skybox_fs.glsl",
                 "Skybox");
    _allocate_vbo();
    _allocate_vao();
}

void
Skybox::update(const glm::vec3 &player_pos)
{
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 3, &player_pos);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void
Skybox::draw(const glm::mat4 &projection, float skybox_size)
{
    _shader.use();
    _shader.setMat4("uniform_mat_perspec_view", projection);
    _shader.setFloat("uniform_skybox_size", skybox_size * 0.5f);
    glBindVertexArray(_vao);
    glDrawArrays(GL_POINTS, 0, 1);
    glBindVertexArray(0);
}

void
Skybox::_allocate_vao()
{
    glGenVertexArrays(1, &_vao);
    if (!_vao) {
        throw std::runtime_error("Skybox: Failed to allocate vao");
    }
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glVertexAttribPointer(
      0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), reinterpret_cast<void *>(0));
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(0, 1);
    glBindVertexArray(0);
}

void
Skybox::_allocate_vbo()
{
    glGenBuffers(1, &_vbo);
    if (!_vbo) {
        throw std::runtime_error("Skybox: Failed to allocate vbo");
    }
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3, nullptr, GL_DYNAMIC_DRAW);
    if (glGetError() == GL_OUT_OF_MEMORY) {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &_vbo);
        throw std::runtime_error("Skybox: Failed to allocate vbo buffer");
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}