#include "PerlinNoise.hpp"

#include <glm/gtc/noise.hpp>

PerlinNoise::PerlinNoise(uint32_t seed)
  : _seed(0)
{
    setSeed(seed);
}

void
PerlinNoise::setSeed(uint32_t seed)
{
    _seed = seed;
}

uint32_t
PerlinNoise::getSeed() const
{
    return (_seed);
}

float
PerlinNoise::noise2d(glm::vec2 const &vec) const
{
    // TODO Do my actual perlin noise
    (void)_seed;
    return ((glm::simplex(vec) / 2.0f) + 0.5f);
}
