#include "Cursor.hpp"

Cursor::Cursor()
  : _is_init(0)
  , _tex()
  , _shader()
  , _size(0.0f)
  , _center(0.0f)
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
    _size = rhs._size;
    _center = rhs._center;
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
  , _size(0.0f)
  , _center(0.0f)
  , _vbo(0)
  , _vao(0)
{
    *this = std::move(src);
}

void
Cursor::init(std::string const &texture_path,
             glm::vec2 const &cursor_size,
             glm::vec2 const &screen_size)
{
    if (_is_init) {
        return;
    }
    _is_init = 1;
    _size = cursor_size;
    _center = _compute_center(screen_size);
    _tex.init(texture_path.c_str(), true);
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
    _size = glm::vec2(0.0f);
    _center = glm::vec2(0.0f);
}

void
Cursor::setCenter(glm::vec2 const &center_pos)
{
    _center = center_pos;
}

void
Cursor::setSize(glm::vec2 const &size)
{
    _size = size;
}

void
Cursor::draw(glm::mat4 const &ortho)
{
    (void)ortho;
}

void
Cursor::_allocate_vbo()
{}

void
Cursor::_allocate_vao()
{}

glm::vec2
Cursor::_compute_center(glm::vec2 const &screen_size)
{
    // TODO
    return (screen_size);
}