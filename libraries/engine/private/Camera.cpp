#include "Camera.hpp"

Camera::Camera()
  : _updated(0)
  , _pos(1.0)
  , _world_up(DEFAULT_WORLD_UP)
  , _right(0.0)
  , _up(0.0)
  , _front(0.0)
  , _view(1.0)
  , _perspective(1.0)
  , _perspec_mult_view(1.0)
  , _mouse_sensitivity(DEFAULT_MOUSE_SENSITIVITY)
  , _movement_speed(DEFAULT_MOVEMENT_SPEED)
  , _yaw(0.0f)
  , _pitch(0.0f)
{}

void
Camera::update_position(glm::ivec3 const &mov)
{
    this->_pos += mov.x * _movement_speed * _front;
    this->_pos += mov.y * _movement_speed * _right;
    this->_pos -= mov.z * _movement_speed * _up;
    _updated = 1;
}

void
Camera::update_front(glm::vec2 const &offsets)
{
    this->_yaw += offsets.x * _mouse_sensitivity;
    this->_pitch += offsets.y * _mouse_sensitivity;
    if (this->_pitch > 89.0f)
        this->_pitch = 89.0f;
    if (this->_pitch < -89.0f)
        this->_pitch = -89.0f;
    _updated = 1;
}

void
Camera::update_matricies()
{
    if (!_updated) {
        return;
    }
    _front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    _front.y = sin(glm::radians(_pitch));
    _front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    glm::normalize(_front);
    _right = glm::normalize(glm::cross(_front, _world_up));
    _up = glm::normalize(glm::cross(_right, _front));
    _view = glm::lookAt(_pos, _pos + _front, _up);
    _perspec_mult_view = _perspective * _view;
    _updated = 0;
}

// Setters
void
Camera::setWorldUp(glm::vec3 const &world_up)
{
    _world_up = world_up;
}

void
Camera::setPerspective(glm::mat4 const &perspective)
{
    _perspective = perspective;
    _perspec_mult_view = _perspective * _view;
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