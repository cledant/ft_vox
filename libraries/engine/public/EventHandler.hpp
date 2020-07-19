#ifndef FT_VOX_EVENTHANDLER_HPP
#define FT_VOX_EVENTHANDLER_HPP

#include <chrono>

#include "IOEvents.hpp"
#include "Camera.hpp"
#include "IOManager.hpp"
#include "Perspective.hpp"
#include "Ui.hpp"
#include "ChunkManager.hpp"
#include "Skybox.hpp"

class EventHandler
{
  public:
    EventHandler();
    virtual ~EventHandler() = default;
    EventHandler(EventHandler const &src) = delete;
    EventHandler &operator=(EventHandler const &rhs) = delete;
    EventHandler(EventHandler &&src) = delete;
    EventHandler &operator=(EventHandler &&rhs) = delete;

    void setCamera(Camera *camera);
    void setIOManager(IOManager *io_manager);
    void setPerspectiveData(Perspective *perspective);
    void setUi(Ui *Ui);
    void setChunkManager(ChunkManager *cm);
    void setSkybox(Skybox *skybox);

    [[nodiscard]] uint8_t printUi() const;

    void processEvents(IOEvents const &events);

  private:
    static constexpr double TARGET_PLAYER_TICK = 20.0f;

    // Timer related
    static constexpr double SYSTEM_TIMER_SECONDS = 1.0;
    static constexpr double CONFIG_TIMER_SECONDS = 0.5;
    static constexpr double ACTION_TIMER_SECONDS = 0.5;
    static constexpr double FAST_ACTION_TIMER_SECONDS = 0.5;
    static constexpr double TARGET_PLAYER_TICK_DURATION =
      1 / TARGET_PLAYER_TICK;

    enum EventTimersTypes
    {
        ET_SYSTEM = 0,
        ET_CONFIG,
        ET_ADD_BLOCK,
        ET_REMOVE_BLOCK,
        ET_CAMERA,
        ET_RENDER_DISTANCE,
        ET_PLAYER_BLOCK_CHANGE,
        ET_NB_EVENT_TIMER_TYPES
    };

    struct EventTimers final
    {
        EventTimers();
        ~EventTimers() = default;

        std::array<uint8_t, ET_NB_EVENT_TIMER_TYPES> accept_event;
        std::array<uint8_t, ET_NB_EVENT_TIMER_TYPES> updated;
        std::array<std::chrono::steady_clock::time_point,
                   ET_NB_EVENT_TIMER_TYPES>
          time_ref;
        std::array<double, ET_NB_EVENT_TIMER_TYPES> timer_diff;
        std::array<double, ET_NB_EVENT_TIMER_TYPES> timer_values;
    };

    // Event handling functions
    void _mouse_exclusive();
    void _close_win_event();
    void _toggle_fullscreen();
    void _jump();
    void _crouch();
    void _front();
    void _back();
    void _right();
    void _left();
    void _add_block();
    void _remove_block();
    void _increase_render_distance();
    void _decrease_render_distance();
    void _toggle_ui();
    void _speed_up();

    // Camera Related
    void _updateCamera(glm::vec2 const &mouse_pos);

    Camera *_camera;
    IOManager *_io_manager;
    Perspective *_perspective;
    ChunkManager *_cm;
    Ui *_ui;
    Skybox *_skybox;

    EventTimers _timers;

    glm::ivec3 _movements;
    glm::vec2 _mouse_pos;
    glm::vec2 _previous_mouse_pos;

    uint8_t _print_ui;
};

#endif // FT_VOX_EVENTHANDLER_HPP
