#ifndef FT_VOX_GLTEXTURE2D_HPP
#define FT_VOX_GLTEXTURE2D_HPP

#include <string>
#include <cstdint>

class GLTexture2D final
{
  public:
    GLTexture2D() = default;
    ~GLTexture2D();
    GLTexture2D(GLTexture2D &&src) noexcept;
    GLTexture2D &operator=(GLTexture2D &&rhs) noexcept;
    GLTexture2D(GLTexture2D const &src) = delete;
    GLTexture2D &operator=(GLTexture2D const &rhs) = delete;
    explicit GLTexture2D(std::string const &filepath);
    explicit GLTexture2D(char const *filepath);

    void loadTexture(char const *filepath);

    [[nodiscard]] uint32_t getTextureID() const;

  private:
    uint32_t _tex_id;
    int32_t _tex_w;
    int32_t _tex_h;
    int32_t _tex_nb_chan;
};

#endif // FT_VOX_GLTEXTURE2D_HPP
