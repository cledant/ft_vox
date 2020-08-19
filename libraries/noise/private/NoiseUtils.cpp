#include "NoiseUtils.hpp"

glm::vec2
getElevationTemperature(glm::vec2 const &coord, PerlinNoise const &pn)
{
    glm::vec2 em = {};

    em.x = noise2dRemapped(512.0f * coord, pn) -
           0.35f * noise2dRemapped(1024.0f * coord, pn);
    em.y = noise2dRemapped(320.0f * coord, pn);
    return (em);
}

float
getCave(glm::vec2 const &coord, PerlinNoise const &pn)
{
    float cave = noise2dRemapped(2048.0f * coord, pn);
    return (cave);
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
            // Display map between 0.0f and 1.0f
            auto pos = glm::vec2(i / static_cast<float>(size.x),
                                 j / static_cast<float>(size.y));
            auto elevation = getElevationTemperature(pos, pn);
            (void)elevation;

            glm::u8vec3 *pixel =
              static_cast<glm::u8vec3 *>(buffer) + i + j * size.y;
            *pixel = MAP_COLOR_BLACK;
        }
    }
}

float
noise3dRemapped(glm::vec3 const &vec, PerlinNoise const &pn)
{
    return ((pn.noise3d(vec) / 2.0f) + 0.5f);
}

float
noise2dRemapped(glm::vec2 const &vec, PerlinNoise const &pn)
{
    return ((pn.noise2d(vec) / 2.0f) + 0.5f);
}