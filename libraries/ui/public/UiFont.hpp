#ifndef FT_VOX_UIFONT_HPP
#define FT_VOX_UIFONT_HPP

#include <map>
#include <string>

#include "ft2build.h"
#include "glm/glm.hpp"

#include "GLShader.hpp"

#include FT_FREETYPE_H

class UiFont final
{
  public:
    UiFont();
    ~UiFont();
    UiFont(UiFont const &src) = delete;
    UiFont &operator=(UiFont const &rhs) = delete;
    UiFont(UiFont &&src) noexcept;
    UiFont &operator=(UiFont &&rhs) noexcept;

    void init(std::string const &path_font,
              std::string const &path_vs,
              std::string const &path_fs,
              uint64_t font_size);
    void clear();

    void drawText(std::string const &str,
                  glm::vec3 const &color,
                  glm::vec2 const &pos,
                  glm::mat4 const &ortho,
                  glm::vec2 const &win_size,
                  float scale);

  private:
    struct FontChar
    {
        uint32_t tex;
        glm::ivec2 size;
        glm::ivec2 bearing;
        size_t advance;
    };

    inline void _loadFont(std::string const &path);
    inline void _allocate_vbo();
    inline void _allocate_vao();
    static inline uint32_t _create_glyph_texture(const void *buffer,
                                          uint32_t tex_w,
                                          uint32_t tex_h);

    uint8_t _is_init;
    uint64_t _font_size;
    GLShader _shader;
    std::map<char, FontChar> _char_list;
    uint32_t _vao;
    uint32_t _vbo;
};

#endif // FT_VOX_UIFONT_HPP
