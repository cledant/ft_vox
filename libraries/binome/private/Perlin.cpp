#include "Perlin.hpp"

#include <random>
#include <functional>

#include <cstring>

#include "glm/gtx/norm.hpp"

PerlinNoise::PerlinNoise()
  : _seed(0)
  , _gradient_table()
  , _permutation_table()
{
    std::random_device rd;
    std::mt19937_64 generator(rd());
    std::uniform_int_distribution<uint64_t> int_distri;
    _seed = int_distri(generator);

    _initialize_tables();
}

PerlinNoise::PerlinNoise(uint64_t seed)
  : _seed(seed)
  , _gradient_table()
  , _permutation_table()
{
    _initialize_tables();
}

double
PerlinNoise::noise(glm::vec3 const &vec)
{
    glm::ivec3 i0 = glm::mod(glm::floor(vec), 255.0f);
    (void)i0;
    return (0.0f);
}

void
PerlinNoise::_initialize_tables()
{
    std::mt19937_64 generator(_seed);
    std::uniform_real_distribution real_distri(0.0f, 1.0f);
    auto do_random = std::bind(real_distri, generator);

    uint32_t i = 0;
    for (auto &grad : _gradient_table) {
        double length2 = 2.0f;
        while (length2 > 1.0f) {
            grad = glm::vec3(2.0f * do_random() - 1.0f,
                             2.0f * do_random() - 1.0f,
                             2.0f * do_random() - 1.0f);
            length2 = glm::length2(grad);
        }
        _permutation_table[i] = i;
        ++i;
    }

    std::uniform_int_distribution int_distri;
    auto do_int_random = std::bind(int_distri, generator);

    for (uint32_t j = 0; j < TABLE_SIZE; ++j) {
        std::swap(_permutation_table[j],
                  _permutation_table[do_int_random() % 255]);
    }
    std::memcpy(
      &_permutation_table[TABLE_SIZE], &_permutation_table[0], TABLE_SIZE);
}

uint32_t
PerlinNoise::_hash(uint32_t const &x,
                   uint32_t const &y,
                   uint32_t const &z) const
{
    return (
      _permutation_table[_permutation_table[_permutation_table[x] + y] + z]);
}