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
    Camera();
    virtual ~Camera() = default;
    Camera(const Camera &src) = default;
    Camera &operator=(Camera const &rhs) = default;
    Camera(Camera &&src) = delete;
    Camera &operator=(Camera &&rhs) = delete;

    void update_position(glm::ivec3 const &mov);
    void update_front(glm::vec2 const &offsets);
    void update_matricies();

    // Setters
    void setWorldUp(glm::vec3 const &world_up);
    void setPerspective(glm::mat4 const &perspective);

    // Getters
    [[nodiscard]] glm::vec3 const &getPosition() const;
    [[nodiscard]] glm::vec3 const &getFront() const;
    [[nodiscard]] glm::mat4 const &getPerspectiveMatrix() const;
    [[nodiscard]] glm::mat4 const &getViewMatrix() const;
    [[nodiscard]] glm::mat4 const &getPerspectiveViewMatrix() const;

  private:
    static float constexpr DEFAULT_MOVEMENT_SPEED = 0.075f;
    static float constexpr DEFAULT_MOUSE_SENSITIVITY = 0.05f;
    static constexpr glm::vec3 DEFAULT_WORLD_UP = glm::vec3(0.0f, 1.0f, 0.0f);

    uint8_t _updated;
    glm::vec3 _pos;
    glm::vec3 _world_up;
    glm::vec3 _right;
    glm::vec3 _up;
    glm::vec3 _front;

    glm::mat4 _view;
    glm::mat4 _perspective;
    glm::mat4 _perspec_mult_view;

    float _mouse_sensitivity;
    float _movement_speed;

    float _yaw;
    float _pitch;
};

#endif // FT_VOX_CAMERA_HPP
