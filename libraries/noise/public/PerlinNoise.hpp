#ifndef FT_VOX_PERLINNOISE_HPP
#define FT_VOX_PERLINNOISE_HPP

#include <array>
#include <cstdint>

#include "glm/glm.hpp"

class PerlinNoise final
{
  public:
    PerlinNoise();
    ~PerlinNoise() = default;
    PerlinNoise(const PerlinNoise &src) = default;
    PerlinNoise &operator=(PerlinNoise const &rhs) = default;
    PerlinNoise(PerlinNoise &&src) noexcept = default;
    PerlinNoise &operator=(PerlinNoise &&rhs) noexcept = default;
    explicit PerlinNoise(uint32_t seed);

    void setSeed(uint32_t seed);

    [[nodiscard]] uint32_t getSeed() const;

    [[nodiscard]] float noise2d(glm::vec2 const &vec) const;
    [[nodiscard]] float noise3d(glm::vec3 const &vec) const;

  private:
    static constexpr uint32_t const GRADIENT_TABLE_SIZE = 256;
    static constexpr uint32_t const GRADIENT_TABLE_MAX_INDEX =
      GRADIENT_TABLE_SIZE - 1;
    static constexpr uint32_t const PERMUTATION_TABLE_SIZE =
      GRADIENT_TABLE_SIZE * 2;

    inline void _init_tables();
    [[nodiscard]] inline int32_t _hash(glm::ivec3 const &vec) const;

    uint32_t _seed;
    std::array<glm::vec3, GRADIENT_TABLE_SIZE> _grad_table;
    std::array<uint32_t, PERMUTATION_TABLE_SIZE> _perm_table;
};

#endif // FT_VOX_PERLINNOISE_HPP
