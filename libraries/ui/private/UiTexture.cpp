#include "UiTexture.hpp"

#include <stdexcept>

UiTexture::UiTexture()
  : _is_init(0)
  , _tex()
  , _shader()
  , _pixel_size(0.0f)
  , _center(0.0f)
  , _vbo(0)
  , _vao(0)
{}

UiTexture::~UiTexture()
{
    if (_vao) {
        glDeleteVertexArrays(1, &_vao);
    }
    if (_vbo) {
        glDeleteBuffers(1, &_vbo);
    }
}

UiTexture &
UiTexture::operator=(UiTexture &&rhs) noexcept
{
    _is_init = rhs._is_init;
    _vbo = rhs._vbo;
    _vao = rhs._vao;
    _pixel_size = rhs._pixel_size;
    _tex = std::move(rhs._tex);
    _shader = std::move(rhs._shader);
    rhs._vbo = 0;
    rhs._vao = 0;
    return (*this);
}

UiTexture::UiTexture(UiTexture &&src) noexcept
  : _is_init(0)
  , _tex()
  , _shader()
  , _pixel_size(0.0f)
  , _center(0.0f)
  , _vbo(0)
  , _vao(0)
{
    *this = std::move(src);
}

void
UiTexture::init(std::string const &texture_path,
                glm::vec2 const &pixel_size,
                glm::vec2 const &center)
{
    if (_is_init) {
        return;
    }
    _is_init = 1;
    _pixel_size = pixel_size;
    _allocate_vbo();
    _allocate_vao();
    _tex.init(texture_path.c_str(), true);
    _shader.init("./ressources/shaders/cursor/cursor_vs.glsl",
                 "./ressources/shaders/cursor/cursor_fs.glsl",
                 "Cursor");
    _center = center;
}

void
UiTexture::clear()
{
    _tex.clear();
    _shader.clear();
    if (_vao) {
        glDeleteVertexArrays(1, &_vao);
    }
    if (_vbo) {
        glDeleteBuffers(1, &_vbo);
    }
    _vao = 0;
    _vbo = 0;
    _pixel_size = glm::vec2(0.0f);
    _is_init = 0;
}

void
UiTexture::setPixelSize(glm::vec2 const &size)
{
    _pixel_size = size;
}

void
UiTexture::setCenter(const glm::vec2 &center)
{
    _center = center;
}

void
UiTexture::draw(glm::mat4 const &ortho)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    _shader.use();
    _shader.setMat4("uniform_mat_ortho", ortho);
    _shader.setVec3("uniform_color", glm::vec3(1.0f));

    float xpos = _center.x - (_pixel_size.x / 2);
    float ypos = _center.y - (_pixel_size.y / 2);
    float vertices[6][4] = {
        { xpos, ypos + _pixel_size.y, 0.0, 0.0 },
        { xpos, ypos, 0.0, 1.0 },
        { xpos + _pixel_size.x, ypos, 1.0, 1.0 },

        { xpos, ypos + _pixel_size.y, 0.0, 0.0 },
        { xpos + _pixel_size.x, ypos, 1.0, 1.0 },
        { xpos + _pixel_size.x, ypos + _pixel_size.y, 1.0, 0.0 }
    };
    glActiveTexture(GL_TEXTURE0);
    _shader.setInt("uniform_tex", 0);
    glBindTexture(GL_TEXTURE_2D, _tex.getTextureID());
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 6 * 4, vertices);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDisable(GL_BLEND);
}

void
UiTexture::_allocate_vbo()
{
    glGenBuffers(1, &_vbo);
    if (!_vbo) {
        throw std::runtime_error("Cursor: Failed to create vbo");
    }
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(
      GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void
UiTexture::_allocate_vao()
{
    glGenVertexArrays(1, &_vao);
    if (!_vao) {
        throw std::runtime_error("Cursor: Failed to create vao");
    }
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glVertexAttribPointer(
      0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, reinterpret_cast<void *>(0));
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}