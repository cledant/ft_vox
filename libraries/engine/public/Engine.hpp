#ifndef FT_VOX_ENGINE_HPP
#define FT_VOX_ENGINE_HPP

#include "IOManager.hpp"
#include "Camera.hpp"
#include "EventHandler.hpp"
#include "Perspective.hpp"
#include "GLFont.hpp"

class Engine
{
  public:
    Engine();
    virtual ~Engine() = default;
    Engine(Engine const &src) = delete;
    Engine &operator=(Engine const &rhs) = delete;
    Engine(Engine &&src) = delete;
    Engine &operator=(Engine &&rhs) = delete;

    void init();
    void run();

  private:
    static constexpr glm::vec2 DEFAULT_NEAR_FAR = glm::vec2(0.1f, 100.0f);
    static constexpr float DEFAULT_FOV = 45.0f;

    IOManager _io_manager;
    Camera _camera;
    EventHandler _event_handler;
    Perspective _perspective_data;
};

#endif // FT_VOX_ENGINE_HPP