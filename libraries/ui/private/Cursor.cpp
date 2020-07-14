#include "Cursor.hpp"

#include <stdexcept>

Cursor::Cursor()
  : _is_init(0)
  , _tex()
  , _shader()
  , _cursor_pixel_size(0.0f)
  , _vbo(0)
  , _vao(0)
{}

Cursor::~Cursor()
{
    if (_vao) {
        glDeleteVertexArrays(1, &_vao);
    }
    if (_vbo) {
        glDeleteBuffers(1, &_vbo);
    }
}

Cursor &
Cursor::operator=(Cursor &&rhs) noexcept
{
    _is_init = rhs._is_init;
    _vbo = rhs._vbo;
    _vao = rhs._vao;
    _cursor_pixel_size = rhs._cursor_pixel_size;
    _tex = std::move(rhs._tex);
    _shader = std::move(rhs._shader);
    rhs._vbo = 0;
    rhs._vao = 0;
    return (*this);
}

Cursor::Cursor(Cursor &&src) noexcept
  : _is_init(0)
  , _tex()
  , _shader()
  , _cursor_pixel_size(0.0f)
  , _vbo(0)
  , _vao(0)
{
    *this = std::move(src);
}

void
Cursor::init(std::string const &texture_path,
             glm::vec2 const &cursor_pixel_size)
{
    if (_is_init) {
        return;
    }
    _is_init = 1;
    _cursor_pixel_size = cursor_pixel_size;
    _allocate_vbo();
    _allocate_vao();
    _tex.init(texture_path.c_str(), true);
    _shader.init("./ressources/shaders/cursor/cursor_vs.glsl",
                 "./ressources/shaders/cursor/cursor_fs.glsl",
                 "Cursor");
}

void
Cursor::clear()
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
    _cursor_pixel_size = glm::vec2(0.0f);
    _is_init = 0;
}

void
Cursor::setCursorPixelSize(glm::vec2 const &size)
{
    _cursor_pixel_size = size;
}

void
Cursor::draw(glm::mat4 const &ortho, glm::vec2 const &win_size)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    _shader.use();
    _shader.setMat4("uniform_mat_ortho", ortho);
    _shader.setVec3("uniform_color", glm::vec3(1.0f));

    float xpos = (win_size.x / 2) - (_cursor_pixel_size.x / 2);
    float ypos = (win_size.y / 2) - (_cursor_pixel_size.y / 2);
    float vertices[6][4] = {
        { xpos, ypos + _cursor_pixel_size.y, 0.0, 0.0 },
        { xpos, ypos, 0.0, 1.0 },
        { xpos + _cursor_pixel_size.x, ypos, 1.0, 1.0 },

        { xpos, ypos + _cursor_pixel_size.y, 0.0, 0.0 },
        { xpos + _cursor_pixel_size.x, ypos, 1.0, 1.0 },
        { xpos + _cursor_pixel_size.x, ypos + _cursor_pixel_size.y, 1.0, 0.0 }
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
Cursor::_allocate_vbo()
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
Cursor::_allocate_vao()
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