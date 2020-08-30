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
    UiFont() = default;
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

    void drawText(const char *str,
                  glm::vec3 const &color,
                  glm::vec2 const &pos,
                  glm::mat4 const &ortho,
                  glm::vec2 const &win_size,
                  float scale);

  private:
    static constexpr uint32_t MAX_FONT_CHAR = 128;
    static constexpr uint32_t MAX_DISPLAYED_CHAR = 128;
    typedef struct FontChar
    {
        float tex_x;
        glm::ivec2 size;    // bw bh
        glm::ivec2 bearing; // bl bt
        glm::ivec2 advance; // ax ay
    } FontChar;
    typedef struct StrBuffer
    {
        std::array<glm::vec4, 6> point;
    } StrBuffer;

    inline void _loadFont(std::string const &path);
    inline void _allocate_vbo();
    inline void _allocate_vao();
    static inline uint32_t _create_font_texture(uint32_t tex_w, uint32_t tex_h);

    uint8_t _is_init;
    uint64_t _font_size;
    GLShader _shader;
    std::array<FontChar, MAX_FONT_CHAR> _char_list;
    uint32_t _char_list_tex_w;
    uint32_t _char_list_tex_h;
    uint32_t _vao;
    uint32_t _vbo;
    uint32_t _font_tex;
};

#endif // FT_VOX_UIFONT_HPP
