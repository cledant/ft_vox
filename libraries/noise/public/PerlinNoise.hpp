#ifndef FT_VOX_PERLINNOISE_HPP
#define FT_VOX_PERLINNOISE_HPP

#include <cstdint>

#include "glm/glm.hpp"

class PerlinNoise final
{
  public:
    PerlinNoise() = default;
    ~PerlinNoise() = default;
    PerlinNoise(const PerlinNoise &src) = default;
    PerlinNoise &operator=(PerlinNoise const &rhs) = default;
    PerlinNoise(PerlinNoise &&src) noexcept = default;
    PerlinNoise &operator=(PerlinNoise &&rhs) noexcept = default;
    explicit PerlinNoise(uint32_t seed);

    void setSeed(uint32_t seed);

    [[nodiscard]] uint32_t getSeed() const;

    [[nodiscard]] float noise2d(glm::vec2 const &vec) const;

  private:
    uint32_t _seed;
};

#endif // FT_VOX_PERLINNOISE_HPP
