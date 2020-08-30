#include <stdexcept>
#include <cstring>

#include "glad/glad.h"

#include "UiFont.hpp"

UiFont::UiFont()
  : _is_init(0)
  , _font_size(1)
  , _shader()
  , _char_list()
  , _vao(0)
  , _vbo(0)
{}

UiFont::~UiFont()
{
    glDeleteVertexArrays(1, &_vao);
    glDeleteBuffers(1, &_vbo);
    for (auto &it : _char_list) {
        if (it.tex) {
            glDeleteTextures(1, &it.tex);
        }
    }
}

UiFont::UiFont(UiFont &&src) noexcept
  : _is_init(0)
  , _font_size(1)
  , _shader()
  , _char_list()
  , _vao(0)
  , _vbo(0)
{
    *this = std::move(src);
}

UiFont &
UiFont::operator=(UiFont &&rhs) noexcept
{
    _is_init = rhs._is_init;
    _shader = std::move(rhs._shader);
    _char_list = rhs._char_list;
    _vao = rhs._vao;
    _vbo = rhs._vbo;
    rhs._vao = 0;
    rhs._vbo = 0;
    std::memset(&_char_list[0], 0, sizeof(FontChar) * MAX_FONT_CHAR);
    rhs._is_init = 0;
    return (*this);
}

void
UiFont::init(std::string const &path_font,
             std::string const &path_vs,
             std::string const &path_fs,
             uint64_t font_size)
{
    if (!_is_init) {
        _allocate_vbo();
        _allocate_vao();
        _font_size = font_size;
        _shader.init(path_vs, path_fs, "FontShader");
        _loadFont(path_font);
        _is_init = 1;
    }
}

void
UiFont::clear()
{
    _is_init = 0;
    for (auto &it : _char_list) {
        if (it.tex) {
            glDeleteTextures(1, &it.tex);
        }
    }
    std::memset(&_char_list[0], 0, sizeof(FontChar) * MAX_FONT_CHAR);
    _shader.clear();
}

void
UiFont::drawText(const char *str,
                 glm::vec3 const &color,
                 glm::vec2 const &pos,
                 glm::mat4 const &ortho,
                 glm::vec2 const &win_size,
                 float scale)
{
    if (!str) {
        return;
    }
    float pos_x_offseted = pos.x;
    float pos_y = win_size.y - pos.y;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    _shader.use();
    _shader.setMat4("uniform_mat_ortho", ortho);
    _shader.setVec3("uniform_color", color);
    glActiveTexture(GL_TEXTURE0);
    _shader.setInt("uniform_tex", 0);
    while (*str) {
        auto &fchar = _char_list[*str];
        float xpos = pos_x_offseted + fchar.bearing.x * scale;
        float ypos = pos_y - (fchar.size.y - fchar.bearing.y) * scale;
        float w = fchar.size.x * scale;
        float h = fchar.size.y * scale;
        float const vertices[6][4] = { { xpos, ypos + h, 0.0, 0.0 },
                                       { xpos, ypos, 0.0, 1.0 },
                                       { xpos + w, ypos, 1.0, 1.0 },

                                       { xpos, ypos + h, 0.0, 0.0 },
                                       { xpos + w, ypos, 1.0, 1.0 },
                                       { xpos + w, ypos + h, 1.0, 0.0 } };
        glBindTexture(GL_TEXTURE_2D, fchar.tex);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 6 * 4, vertices);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        pos_x_offseted += (fchar.advance >> 6) * scale;
        str++;
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDisable(GL_BLEND);
}

void
UiFont::_loadFont(std::string const &path)
{
    FT_Library lib;
    FT_Face face;

    if (FT_Init_FreeType(&lib)) {
        throw std::runtime_error("GLFont: Freetype init failed");
    }
    if (FT_New_Face(lib, path.c_str(), 0, &face)) {
        FT_Done_FreeType(lib);
        throw std::runtime_error("GLFont: Failed to load font");
    }
    FT_Set_Pixel_Sizes(face, 0, _font_size);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable texture alignment
    for (uint8_t i = 0; i < 128; ++i) {
        if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
            FT_Done_Face(face);
            FT_Done_FreeType(lib);
            throw std::runtime_error("GLFont: Failed to load glyph");
        }
        try {
            this->_char_list[i] = {
                _create_glyph_texture(face->glyph->bitmap.buffer,
                                      face->glyph->bitmap.width,
                                      face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                static_cast<uint32_t>(face->glyph->advance.x)
            };
        } catch (std::exception &e) {
            FT_Done_Face(face);
            FT_Done_FreeType(lib);
            throw;
        }
    }
    FT_Done_Face(face);
    FT_Done_FreeType(lib);
}

void
UiFont::_allocate_vbo()
{
    glGenBuffers(1, &_vbo);
    if (!_vbo) {
        throw std::runtime_error("GLFont: Failed to create buffer");
    }
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(
      GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
    if (glGetError() == GL_OUT_OF_MEMORY) {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        throw std::runtime_error("GLFont: Failed to allocate buffer");
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void
UiFont::_allocate_vao()
{
    glGenVertexArrays(1, &_vao);
    if (!_vao) {
        throw std::runtime_error("GLFont: Failed to create vao");
    }
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glVertexAttribPointer(
      0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, reinterpret_cast<void *>(0));
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

uint32_t
UiFont::_create_glyph_texture(const void *buffer,
                              uint32_t tex_w,
                              uint32_t tex_h)
{
    uint32_t tex_id;

    glGenTextures(1, &tex_id);
    if (!tex_id) {
        throw std::runtime_error("GLFont: Failed to create texture for glyph");
    }
    glBindTexture(GL_TEXTURE_2D, tex_id);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RED,
                 tex_w,
                 tex_h,
                 0,
                 GL_RED,
                 GL_UNSIGNED_BYTE,
                 buffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    return (tex_id);
}