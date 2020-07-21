#ifndef FT_VOX_PERSPECTIVE_HPP
#define FT_VOX_PERSPECTIVE_HPP

#include <glm/glm.hpp>

struct Perspective final
{
    float ratio;
    glm::vec2 near_far;
    float fov;
};

#endif // FT_VOX_PERSPECTIVE_HPP
