#ifndef FT_VOX_ENGINE_HPP
#define FT_VOX_ENGINE_HPP

#include "IOManager.hpp"
#include "Camera.hpp"
#include "EventHandler.hpp"
#include "Perspective.hpp"
#include "Ui.hpp"
#include "TestBoxDrawer.hpp"
#include "ChunkManager.hpp"
#include "Skybox.hpp"

class Engine final
{
  public:
    Engine();
    ~Engine() = default;
    Engine(Engine const &src) = delete;
    Engine &operator=(Engine const &rhs) = delete;
    Engine(Engine &&src) = delete;
    Engine &operator=(Engine &&rhs) = delete;

    void init();
    void run();

  private:
    static constexpr glm::vec2 const DEFAULT_NEAR_FAR =
      glm::vec2(0.1f, 5000.0f);
    static constexpr float const DEFAULT_FOV = 45.0f;
    static constexpr glm::vec3 const START_POS =
      glm::vec3(-1000.0f, 10.0f, -1000.0f);

    void _compute_fps();

    IOManager _io_manager;
    Camera _camera;
    EventHandler _event_handler;
    Perspective _perspective_data;
    Ui _ui;
    ChunkManager _cm;
    Skybox _skybox;

    // Fps related
    uint64_t _nb_frame;
    std::chrono::steady_clock::time_point _fps_count_timeref;
    std::string _str_fps;
};

#endif // FT_VOX_ENGINE_HPP