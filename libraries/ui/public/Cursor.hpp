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
              glm::vec2 const &cursor_pixel_size);
    void clear();

    void setCursorPixelSize(glm::vec2 const &size);

    void draw(glm::mat4 const &ortho, glm::vec2 const &win_size);

  private:
    inline void _allocate_vbo();
    inline void _allocate_vao();

    uint8_t _is_init;
    GLTexture2D _tex;
    GLShader _shader;
    glm::vec2 _cursor_pixel_size;
    uint32_t _vbo;
    uint32_t _vao;
};

#endif // FT_VOX_CURSOR_HPP
