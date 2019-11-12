#ifndef FT_VOX_ENGINE_HPP
#define FT_VOX_ENGINE_HPP

#include "IOManager.hpp"
#include "Camera.hpp"
#include "EventHandler.hpp"
#include "Perspective.hpp"
#include "GLFont.hpp"
#include "TestBoxDrawer.hpp"
#include "ChunkManager.hpp"

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

    void _compute_fps();
    void _print_ui_info();

    IOManager _io_manager;
    Camera _camera;
    EventHandler _event_handler;
    Perspective _perspective_data;
    GLFont _font;
    TestBoxDrawer _bd;
    ChunkManager _cm;

    // Fps related
    uint64_t _nb_frame;
    std::chrono::high_resolution_clock::time_point _fps_count_timeref;
    std::string _str_fps;
};

#endif // FT_VOX_ENGINE_HPP