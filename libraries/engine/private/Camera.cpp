#include "Camera.hpp"

Camera::Camera(const Camera &src)
{
    // TODO
    *this = src;
}

Camera &
Camera::operator=(Camera const &rhs)
{
    // TODO
    (void)rhs;
    return (*this);
}

void
Camera::update_position(glm::ivec3 const &mov)
{
    // TODO
    (void)mov;
}

void
Camera::update_front(glm::vec2 const &offsets)
{
    // TODO
    (void)offsets;
}

void
Camera::update_matricies()
{
    // TODO
}

// Setters
void
Camera::setWorldUp(glm::vec3 const &world_up)
{
    _world_up = world_up;
}

void
Camera::setNearFar(glm::vec2 const &near_far)
{
    _near_far = near_far;
}

// Getters
glm::vec3 const &
Camera::getPosition() const
{
    return (_pos);
}

glm::vec3 const &
Camera::getFront() const
{
    return (_front);
}

glm::mat4 const &
Camera::getPerspectiveMatrix() const
{
    return (_perspective);
}

glm::mat4 const &
Camera::getViewMatrix() const
{
    return (_view);
}

glm::mat4 const &
Camera::getPerspectiveViewMatrix() const
{
    return (_perspec_mult_view);
}