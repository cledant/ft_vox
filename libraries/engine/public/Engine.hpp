#ifndef FT_VOX_ENGINE_HPP
#define FT_VOX_ENGINE_HPP

#include "IOManager.hpp"
#include "Camera.hpp"
#include "EventHandler.hpp"
#include "Perspective.hpp"
#include "GLFont.hpp"
#include "TestBoxDrawer.hpp"

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
    static constexpr float FPS_CAP = 120.0f;
    static constexpr float FPS_CAP_DURATION = 1 / FPS_CAP;

    void _print_ui_info();

    IOManager _io_manager;
    Camera _camera;
    EventHandler _event_handler;
    Perspective _perspective_data;
    GLFont _font;
    std::chrono::high_resolution_clock::time_point _fps_cap_timeref;
    TestBoxDrawer _bd;
};

#endif // FT_VOX_ENGINE_HPP