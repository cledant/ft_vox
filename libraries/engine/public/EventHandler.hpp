#ifndef FT_VOX_EVENTHANDLER_HPP
#define FT_VOX_EVENTHANDLER_HPP

#include <chrono>

#include "IOEvents.hpp"
#include "Camera.hpp"
#include "IOManager.hpp"
#include "Perspective.hpp"
#include "GLFont.hpp"

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
    void setFont(GLFont *font);

    void processEvents(IOEvents const &events);

  private:
    // Timer related
    static double constexpr SYSTEM_TIMER_SECONDS = 1.0;
    static double constexpr CONFIG_TIMER_SECONDS = 0.5;
    static double constexpr ACTION_TIMER_SECONDS = 0.25;
    static uint8_t constexpr NB_EVENT_TIMER_TYPES = 4;
    static constexpr double TARGET_PLAYER_TICK = 30.0f;
    static constexpr double TARGET_PLAYER_TICK_DURATION =
      1 / TARGET_PLAYER_TICK;

    enum EventTimersTypes
    {
        SYSTEM = 0,
        CONFIG,
        ACTION,
        CAMERA
    };

    struct EventTimers
    {
        EventTimers();
        virtual ~EventTimers() = default;

        std::array<uint8_t, NB_EVENT_TIMER_TYPES> accept_event;
        std::array<uint8_t, NB_EVENT_TIMER_TYPES> updated;
        std::array<std::chrono::high_resolution_clock::time_point,
                   NB_EVENT_TIMER_TYPES>
          time_ref;
        std::array<double, NB_EVENT_TIMER_TYPES> timer_values;
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

    // Camera Related
    void _updateCamera(glm::vec2 const &mouse_pos);

    Camera *_camera;
    IOManager *_io_manager;
    Perspective *_perspective;
    GLFont *_font;

    EventTimers _timers;

    glm::ivec3 _movements;
    glm::vec2 _mouse_pos;
    glm::vec2 _previous_mouse_pos;
};

#endif // FT_VOX_EVENTHANDLER_HPP
