#include "NoiseUtils.hpp"

float
getElevation(glm::vec2 const &coord, PerlinNoise const &pn)
{
    float elevation = noise2dRemapped(512.0f * coord, pn) -
                      0.35f * noise2dRemapped(1024.0f * coord, pn);
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
            // Display map between 0.0f and 1.0f
            auto pos = glm::vec2(i / static_cast<float>(size.x),
                                 j / static_cast<float>(size.y));
            auto elevation = getElevation(pos, pn);

            glm::u8vec3 *pixel =
              static_cast<glm::u8vec3 *>(buffer) + i + j * size.y;
            if (elevation < 0.0f) {
                *pixel = MAP_COLOR_BLACK;
            } else if (elevation < 0.20f) {
                *pixel = MAP_COLOR_BLUE;
            } else if (elevation < 0.40f) {
                *pixel = MAP_COLOR_YELLOW;
            } else if (elevation < 0.60f) {
                *pixel = MAP_COLOR_BROWN;
            } else if (elevation < 0.80f) {
                *pixel = MAP_COLOR_GREEN;
            } else if (elevation <= 1.0f) {
                *pixel = MAP_COLOR_WHITE;
            } else {
                *pixel = MAP_COLOR_RED;
            }
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