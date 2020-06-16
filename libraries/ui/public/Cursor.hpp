#ifndef FT_VOX_CURSOR_HPP
#define FT_VOX_CURSOR_HPP

#include <cstdint>
#include <string>

#include "glm/glm.hpp"
#include "glad/glad.h"

#include "GLTexture2D.hpp"
#include "GLShader.hpp"

class Cursor final
{
  public:
    Cursor();
    ~Cursor();
    Cursor(Cursor const &src) = delete;
    Cursor &operator=(Cursor const &rhs) = delete;
    Cursor(Cursor &&src) noexcept;
    Cursor &operator=(Cursor &&rhs) noexcept;

    void init(std::string const &texture_path,
              glm::vec2 const &size,
              glm::vec2 const &center);
    void clear();

    void setCenter(glm::vec2 const &center_pos);
    void setSize(glm::vec2 const &size);

    void draw(glm::mat4 const &ortho);

    void _allocate_vbo();
    void _allocate_vao();
    glm::vec2 _compute_center(glm::vec2 const &screen_size);

    uint8_t _is_init;
    GLTexture2D _tex;
    GLShader _shader;
    glm::vec2 _size;
    glm::vec2 _center;
    uint32_t _vbo;
    uint32_t _vao;
};

#endif // FT_VOX_CURSOR_HPP
