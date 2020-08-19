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

glm::vec2
getCaveLimits(glm::vec2 const &coord, PerlinNoise const &pn)
{
    float limits_1 = 0.275f * noise2dRemapped(4096.0f * coord, pn);
    float limits_2 = 0.175f * noise2dRemapped(768.0f * coord, pn);
    if (limits_1 < limits_2) {
        return (glm::vec2(limits_1, limits_2));
    }
    return (glm::vec2(limits_2, limits_1));
}

void
generateMap(const glm::ivec2 &size, uint32_t seed, void *buffer)
{
    (void)size;
    (void)seed;
    (void)buffer;
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