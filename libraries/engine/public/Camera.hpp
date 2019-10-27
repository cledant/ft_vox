#ifndef FT_VOX_CAMERA_HPP
#define FT_VOX_CAMERA_HPP

#include <iostream>
#include <sstream>
#include <iomanip>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Camera
{
  public:
    Camera() = default;
    virtual ~Camera() = default;
    Camera(const Camera &src);
    Camera &operator=(Camera const &rhs);
    Camera(Camera &&src) = delete;
    Camera &operator=(Camera &&rhs) = delete;

    void update();

    // Setters
    void setWorldUp(glm::vec3 const &world_up);
    void setNearFar(glm::vec2 const &near_far);

    // Getters
    [[nodiscard]] glm::vec3 const &getPosition() const;
    [[nodiscard]] glm::vec3 const &getFront() const;
    [[nodiscard]] glm::mat4 const &getPerspectiveMatrix() const;
    [[nodiscard]] glm::mat4 const &getViewMatrix() const;
    [[nodiscard]] glm::mat4 const &getPerspectiveViewMatrix() const;

  private:
    static float constexpr INITIAL_MOVEMENT_SPEED = 0.075f;
    static float constexpr INITIAL_MOUSE_SENSITIVITY = 0.05f;

    uint8_t _updated;
    glm::vec3 _pos;
    glm::vec3 _world_up;
    glm::vec3 _right;
    glm::vec3 _up;
    glm::vec3 _front;

    glm::mat4 _view;
    glm::vec2 _near_far;
    glm::mat4 _perspective;
    glm::mat4 _perspec_mult_view;

    float _mouse_sensitivity;
    float _movement_speed;

    glm::vec2 _mouse_pos;
    glm::vec2 _mouse_offset;
    float _yaw;
    float _pitch;
};

#endif // FT_VOX_CAMERA_HPP
