#ifndef FT_VOX_PERSPECTIVE_HPP
#define FT_VOX_PERSPECTIVE_HPP

#include <glm/glm.hpp>

struct Perspective
{
    Perspective()
      : ratio(0.0)
      , near_far(0.0)
      , fov(0.0){};
    virtual ~Perspective() = default;

    float ratio;
    glm::vec2 near_far;
    float fov;
};

#endif // FT_VOX_PERSPECTIVE_HPP
