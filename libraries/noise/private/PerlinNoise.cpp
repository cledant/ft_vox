#include "PerlinNoise.hpp"

#include <random>

#include "glm/gtx/norm.hpp"

PerlinNoise::PerlinNoise()
  : _seed(0)
  , _grad_table_3d()
  , _perm_table()
{}

PerlinNoise::PerlinNoise(uint32_t seed)
  : _seed(0)
  , _grad_table_3d()
  , _perm_table()
{
    setSeed(seed);
}

void
PerlinNoise::setSeed(uint32_t seed)
{
    _seed = seed;
    _init_tables();
}

uint32_t
PerlinNoise::getSeed() const
{
    return (_seed);
}

float
PerlinNoise::noise2d(glm::vec2 const &vec) const
{
    // TODO real 2d noise
    return (noise3d(glm::vec3(vec, 0.5f)));
}

float
PerlinNoise::noise3d(glm::vec3 const &vec) const
{
    auto i0 =
      glm::ivec3(glm::floor(vec)) % glm::ivec3(GRADIENT_TABLE_MAX_INDEX);
    auto i1 = glm::ivec3(i0 + 1) % glm::ivec3(GRADIENT_TABLE_MAX_INDEX);
    auto t = vec - glm::floor(vec);
    float u = glm::smoothstep(0.0f, 1.0f, t.x);
    float v = glm::smoothstep(0.0f, 1.0f, t.y);
    float w = glm::smoothstep(0.0f, 1.0f, t.z);

    glm::vec3 const &c000 = _grad_table_3d[_hash(i0)];
    glm::vec3 const &c100 = _grad_table_3d[_hash(glm::ivec3(i1.x, i0.y, i0.z))];
    glm::vec3 const &c010 = _grad_table_3d[_hash(glm::ivec3(i0.x, i1.y, i0.z))];
    glm::vec3 const &c110 = _grad_table_3d[_hash(glm::ivec3(i1.x, i1.y, i0.z))];
    glm::vec3 const &c001 = _grad_table_3d[_hash(glm::ivec3(i0.x, i0.y, i1.z))];
    glm::vec3 const &c101 = _grad_table_3d[_hash(glm::ivec3(i1.x, i0.y, i1.z))];
    glm::vec3 const &c011 = _grad_table_3d[_hash(glm::ivec3(i0.x, i1.y, i1.z))];
    glm::vec3 const &c111 = _grad_table_3d[_hash(i1)];

    auto v0 = glm::vec3(t);
    auto v1 = glm::vec3(t - 1.0f);

    auto p000 = v0;
    auto p100 = glm::vec3(v1.x, v0.y, v0.z);
    auto p010 = glm::vec3(v0.x, v1.y, v0.z);
    auto p110 = glm::vec3(v1.x, v1.y, v0.z);
    auto p001 = glm::vec3(v0.x, v0.y, v1.z);
    auto p101 = glm::vec3(v1.x, v0.y, v1.z);
    auto p011 = glm::vec3(v0.x, v1.y, v1.z);
    auto p111 = v1;

    // lerp = mix in glsl
    float a = glm::mix(glm::dot(c000, p000), glm::dot(c100, p100), u);
    float b = glm::mix(glm::dot(c010, p010), glm::dot(c110, p110), u);
    float c = glm::mix(glm::dot(c001, p001), glm::dot(c101, p101), u);
    float d = glm::mix(glm::dot(c011, p011), glm::dot(c111, p111), u);

    float e = glm::mix(a, b, v);
    float f = glm::mix(c, d, v);

    return (glm::mix(e, f, w));
}

void
PerlinNoise::_init_tables()
{
    std::mt19937_64 gen(_seed);
    std::uniform_real_distribution real_rd;

    for (uint32_t i = 0; i < GRADIENT_TABLE_SIZE; ++i) {
        float theta = glm::acos(2 * real_rd(gen) - 1);
        float phi = 2.0f * real_rd(gen) * M_PI;

        _grad_table_3d[i] = glm::vec3(glm::cos(phi) * glm::sin(theta),
                                      glm::sin(phi) * glm::sin(theta),
                                      glm::cos(theta));
        _perm_table[i] = i;
    }

    std::uniform_int_distribution int_rd;
    for (uint32_t i = 0; i < GRADIENT_TABLE_SIZE; ++i) {
        std::swap(_perm_table[i],
                  _perm_table[int_rd(gen) % GRADIENT_TABLE_MAX_INDEX]);
    }
    for (uint32_t i = 0; i < GRADIENT_TABLE_SIZE; ++i) {
        _perm_table[i + GRADIENT_TABLE_SIZE] = _perm_table[i];
    }
}

int32_t
PerlinNoise::_hash(glm::ivec3 const &vec) const
{
    return (_perm_table[_perm_table[_perm_table[vec.x] + vec.y] + vec.z]);
}
