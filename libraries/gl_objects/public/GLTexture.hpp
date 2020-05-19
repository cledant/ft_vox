#ifndef FT_VOX_GLTEXTURE_HPP
#define FT_VOX_GLTEXTURE_HPP

#include <string>
#include <cstdint>

class GLTexture final
{
  public:
    GLTexture() = default;
    ~GLTexture();
    explicit GLTexture(std::string const &filepath);
    explicit GLTexture(char const *filepath);
    GLTexture(GLTexture &&src) noexcept;
    GLTexture &operator=(GLTexture &&rhs) noexcept;
    GLTexture(GLTexture const &src) = delete;
    GLTexture &operator=(GLTexture const &rhs) = delete;

    uint32_t getTextureID() const;

  private:
    uint32_t _tex_id;
    int32_t _tex_w;
    int32_t _tex_h;
    int32_t _tex_nb_chan;

    inline void _load_texture(char const *filepath);
};

#endif // FT_VOX_GLTEXTURE_HPP
