#ifndef FT_VOX_PERLIN_HPP
#define FT_VOX_PERLIN_HPP

#include <cstdint>
#include <array>

#include "glm/glm.hpp"

class PerlinNoise
{
  public:
    PerlinNoise();
    virtual ~PerlinNoise() = default;
    PerlinNoise(PerlinNoise const &src) = delete;
    PerlinNoise &operator=(PerlinNoise const &rhs) = delete;
    PerlinNoise(PerlinNoise &&src) = delete;
    PerlinNoise &operator=(PerlinNoise &&rhs) = delete;
    explicit PerlinNoise(uint64_t seed);

    double noise(glm::vec3 const &vec);

  private:
    static constexpr uint32_t TABLE_SIZE = 256;

    uint64_t _seed;
    // Gradient = random 3d vector
    std::array<glm::vec3, TABLE_SIZE> _gradient_table;
    std::array<uint32_t, 2 * TABLE_SIZE> _permutation_table;

    void _initialize_tables();
    uint32_t _hash(uint32_t const &x,
                   uint32_t const &y,
                   uint32_t const &z) const;
};

#endif // FT_VOX_PERLIN_HPP
