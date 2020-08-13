#include "NoiseUtils.hpp"

float
generateValueWithOctave(glm::vec2 const &coord, PerlinNoise const &pn)
{
    float elevation = OCTAVE_COEFF[0] * pn.noise2d(coord) +
                      OCTAVE_COEFF[1] * pn.noise2d(2.0f * coord) +
                      OCTAVE_COEFF[2] * pn.noise2d(4.0f * coord) +
                      OCTAVE_COEFF[3] * pn.noise2d(8.0f * coord);

    elevation /= (OCTAVE_COEFF_SUM);
    elevation = glm::pow(elevation, 3.85f);
    return (elevation);
}

void
generateMap(const glm::ivec2 &size, uint32_t seed, void *buffer)
{
    if (!buffer) {
        return;
    }

    PerlinNoise pn(seed);
    for (int32_t i = 0; i < size.x; ++i) {
        for (int32_t j = 0; j < size.y; ++j) {
            auto pos = glm::vec2(i / static_cast<float>(size.x) + 0.5f,
                                 j / static_cast<float>(size.y) + 0.5f);
            auto elevation = generateValueWithOctave(pos, pn);

            glm::i8vec3 *pixel =
              static_cast<glm::i8vec3 *>(buffer) + i + j * size.y;
            pixel->r = elevation * 255.0f;
            pixel->g = elevation * 255.0f;
            pixel->b = elevation * 255.0f;
        }
    }
}