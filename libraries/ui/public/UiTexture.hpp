#ifndef FT_VOX_UITEXTURE_HPP
#define FT_VOX_UITEXTURE_HPP

#include <cstdint>
#include <string>

#include "glm/glm.hpp"
#include "glad/glad.h"

#include "GLTexture2D.hpp"
#include "GLShader.hpp"

class UiTexture final
{
  public:
    UiTexture();
    ~UiTexture();
    UiTexture(UiTexture const &src) = delete;
    UiTexture &operator=(UiTexture const &rhs) = delete;
    UiTexture(UiTexture &&src) noexcept;
    UiTexture &operator=(UiTexture &&rhs) noexcept;

    void init(std::string const &texture_path,
              glm::vec2 const &pixel_size,
              glm::vec2 const &center,
              std::string const &vs_path,
              std::string const &fs_path,
              std::string const &shader_name);
    void init(void const *texture_buffer,
              glm::ivec2 const &tex_size,
              int32_t tex_nb_chan,
              glm::vec2 const &pixel_size,
              glm::vec2 const &center,
              std::string const &vs_path,
              std::string const &fs_path,
              std::string const &shader_name);
    void clear();

    void setPixelSize(glm::vec2 const &size);
    void setCenter(glm::vec2 const &center);

    void draw(glm::mat4 const &ortho);

  private:
    inline void _allocate_vbo();
    inline void _allocate_vao();

    GLTexture2D _tex;
    GLShader _shader;
    glm::vec2 _pixel_size;
    glm::vec2 _center;
    uint32_t _vbo;
    uint32_t _vao;
};

#endif // FT_VOX_UITEXTURE_HPP
