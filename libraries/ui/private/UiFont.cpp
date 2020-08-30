#include <stdexcept>
#include <algorithm>

#include "glad/glad.h"

#include "UiFont.hpp"

UiFont::~UiFont()
{
    glDeleteVertexArrays(1, &_vao);
    glDeleteBuffers(1, &_vbo);
    if (_font_tex) {
        glDeleteTextures(1, &_font_tex);
    }
}

UiFont::UiFont(UiFont &&src) noexcept
  : _is_init()
  , _font_size()
  , _shader()
  , _char_list()
  , _char_list_tex_w()
  , _char_list_tex_h()
  , _vao()
  , _vbo()
  , _font_tex()
{
    *this = std::move(src);
}

UiFont &
UiFont::operator=(UiFont &&rhs) noexcept
{
    _is_init = rhs._is_init;
    _font_size = rhs._font_size;
    _shader = std::move(rhs._shader);
    _char_list = rhs._char_list;
    _char_list_tex_w = rhs._char_list_tex_w;
    _char_list_tex_h = rhs._char_list_tex_h;
    _vao = rhs._vao;
    _vbo = rhs._vbo;
    _font_tex = rhs._font_tex;
    rhs._vao = 0;
    rhs._vbo = 0;
    rhs._font_tex = 0;
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
    glDeleteVertexArrays(1, &_vao);
    glDeleteBuffers(1, &_vbo);
    if (_font_tex) {
        glDeleteTextures(1, &_font_tex);
    }
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

    // OpenGL init
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    _shader.use();
    _shader.setMat4("uniform_mat_ortho", ortho);
    _shader.setVec3("uniform_color", color);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _font_tex);

    // Bufferization
    uint32_t i = 0;
    float pos_x_offseted = pos.x;
    float pos_y = win_size.y - pos.y;
    StrBuffer char_buff[MAX_DISPLAYED_CHAR] = {};
    while (*str && i < MAX_DISPLAYED_CHAR) {
        auto &fchar = _char_list[*str];

        float xpos = pos_x_offseted + fchar.bearing.x * scale;
        float ypos = pos_y - (fchar.size.y - fchar.bearing.y) * scale;
        float w = fchar.size.x * scale;
        float h = fchar.size.y * scale;

        char_buff[i] = StrBuffer{
            glm::vec4(xpos, ypos + h, fchar.tex_x, 0.0),
            glm::vec4(xpos,
                      ypos,
                      fchar.tex_x,
                      fchar.size.y / static_cast<float>(_char_list_tex_h)),
            glm::vec4(xpos + w,
                      ypos,
                      fchar.tex_x +
                        (fchar.size.x / static_cast<float>(_char_list_tex_w)),
                      fchar.size.y / static_cast<float>(_char_list_tex_h)),

            glm::vec4(xpos, ypos + h, fchar.tex_x, 0.0),
            glm::vec4(xpos + w,
                      ypos,
                      fchar.tex_x +
                        (fchar.size.x / static_cast<float>(_char_list_tex_w)),
                      fchar.size.y / static_cast<float>(_char_list_tex_h)),
            glm::vec4(xpos + w,
                      ypos + h,
                      fchar.tex_x +
                        (fchar.size.x / static_cast<float>(_char_list_tex_w)),
                      0.0)
        };

        pos_x_offseted += (fchar.advance.x >> 6) * scale;
        ++str;
        ++i;
    }
    // Draw
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(StrBuffer) * i, &char_buff[0]);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawArrays(GL_TRIANGLES, 0, 6 * i);

    // Exit
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDisable(GL_BLEND);
}

void
UiFont::_loadFont(std::string const &path)
{
#ifndef NDEBUG
    std::cout << "Loading Font : " << path << std::endl;
#endif
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

    // Parsing max texture w and h
    uint32_t font_tex_w = 0;
    uint32_t font_text_h = 0;
    for (uint8_t i = 32; i < 128; ++i) {
        if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
            FT_Done_Face(face);
            FT_Done_FreeType(lib);
            throw std::runtime_error("GLFont: Failed to load glyph");
        }
        font_tex_w += face->glyph->bitmap.width;
        font_text_h = std::max(font_text_h, face->glyph->bitmap.rows);
    }
    _char_list_tex_w = font_tex_w;
    _char_list_tex_h = font_text_h;
    _font_tex = _create_font_texture(font_tex_w, font_text_h);

    // Printing char buffers into texture
    uint32_t x = 0;
    glBindTexture(GL_TEXTURE_2D, _font_tex);
    for (uint8_t i = 32; i < 128; ++i) {
        if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
            FT_Done_Face(face);
            FT_Done_FreeType(lib);
            throw std::runtime_error("GLFont: Failed to load glyph");
        }
        glTexSubImage2D(GL_TEXTURE_2D,
                        0,
                        x,
                        0,
                        face->glyph->bitmap.width,
                        face->glyph->bitmap.rows,
                        GL_RED,
                        GL_UNSIGNED_BYTE,
                        face->glyph->bitmap.buffer);
        this->_char_list[i] = {
            x / static_cast<float>(_char_list_tex_w),
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            glm::ivec2(face->glyph->advance.x, face->glyph->advance.y)
        };
        x += face->glyph->bitmap.width;
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    FT_Done_Face(face);
    FT_Done_FreeType(lib);

    if (glGetError() != GL_NO_ERROR) {
        throw std::runtime_error("OpenGL Error for Font 2d: " +
                                 std::string(path));
    }
}

void
UiFont::_allocate_vbo()
{
    glGenBuffers(1, &_vbo);
    if (!_vbo) {
        throw std::runtime_error("GLFont: Failed to create buffer");
    }
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(float) * 6 * 4 * MAX_DISPLAYED_CHAR,
                 nullptr,
                 GL_DYNAMIC_DRAW);
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
UiFont::_create_font_texture(uint32_t tex_w, uint32_t tex_h)
{
    uint32_t tex_id;

    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &tex_id);
    if (!tex_id) {
        throw std::runtime_error("GLFont: Failed to create texture for glyph");
    }
    glBindTexture(GL_TEXTURE_2D, tex_id);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable texture alignment
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RED,
                 tex_w,
                 tex_h,
                 0,
                 GL_RED,
                 GL_UNSIGNED_BYTE,
                 nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    return (tex_id);
}