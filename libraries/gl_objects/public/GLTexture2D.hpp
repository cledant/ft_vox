#ifndef FT_VOX_GLTEXTURE2D_HPP
#define FT_VOX_GLTEXTURE2D_HPP

#include <string>
#include <array>
#include <cstdint>

#include "glm/glm.hpp"

class GLTexture2D final
{
  public:
    GLTexture2D() = default;
    ~GLTexture2D();
    GLTexture2D(GLTexture2D &&src) noexcept;
    GLTexture2D &operator=(GLTexture2D &&rhs) noexcept;
    GLTexture2D(GLTexture2D const &src) = delete;
    GLTexture2D &operator=(GLTexture2D const &rhs) = delete;
    explicit GLTexture2D(std::string const &filepath,
                         uint8_t use_nearest_filtering);
    explicit GLTexture2D(char const *filepath, uint8_t use_nearest_filtering);

    void init(char const *filepath, uint8_t use_nearest_filtering);
    void init(void const *buffer,
              glm::ivec2 size,
              int32_t nb_chan,
              uint8_t use_nearest_filtering);
    void clear();

    [[nodiscard]] uint32_t getTextureID() const;

  private:
    inline void _creating_gpu_tex(void const *data,
                                  uint8_t use_nearest_filtering);

    uint32_t _tex_id;
    // x = w | y = h
    glm::ivec2 _tex_size;
    int32_t _tex_nb_chan;
};

#endif // FT_VOX_GLTEXTURE2D_HPP
