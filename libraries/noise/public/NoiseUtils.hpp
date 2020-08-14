#ifndef FT_VOX_NOISEUTILS_HPP
#define FT_VOX_NOISEUTILS_HPP

#include <array>

#include "glm/glm.hpp"

#include "PerlinNoise.hpp"

template<typename Iterator, typename T>
constexpr T
sum_array(Iterator begin, Iterator end, T init_val)
{
    for (; begin != end; ++begin) {
        init_val = std::move(init_val) + *begin;
    }
    return (init_val);
}

constexpr std::array<float, 4> const OCTAVE_COEFF = { 0.8f,
                                                      0.65f,
                                                      1.00f,
                                                      0.65f };

constexpr float const OCTAVE_COEFF_SUM =
  sum_array(OCTAVE_COEFF.begin(), OCTAVE_COEFF.end(), 0.0f);

[[nodiscard]] float generateValueWithOctave(glm::vec2 const &coord,
                                            PerlinNoise const &pn);

void generateMap(glm::ivec2 const &size, uint32_t seed, void *buffer);

#endif // FT_VOX_NOISEUTILS_HPP