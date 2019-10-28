#include "EventHandler.hpp"

#include <functional>
#include <iostream>

EventHandler::EventHandler()
  : _camera(nullptr)
  , _io_manager(nullptr)
  , _perspective(nullptr)
  , _timers()
  , _movements(0)
  , _mouse_pos(0.0)
  , _previous_mouse_pos(0.0)
{}

void
EventHandler::setCamera(Camera *camera)
{
    _camera = camera;
}

void
EventHandler::setIOManager(IOManager *io_manager)
{
    _io_manager = io_manager;
}

void
EventHandler::setPerspectiveData(Perspective *perspective)
{
    _perspective = perspective;
}

void
EventHandler::processEvents(IOEvents const &events)
{
    assert(_camera);
    assert(_io_manager);
    assert(_perspective);

    // Resetting movement tracking
    _movements = glm::ivec3(0);

    // Mouse position
    static uint8_t first_time_mouse_pos = 1;
    if (first_time_mouse_pos) {
        _previous_mouse_pos = events.mouse_position;
        first_time_mouse_pos = 0;
    }
    _mouse_pos = events.mouse_position;

    static const std::array<void (EventHandler::*)(), NBR_IO_EVENTS> func = {
        &EventHandler::_mouse_exclusive,
        &EventHandler::_close_win_event,
        &EventHandler::_toggle_fullscreen,
        &EventHandler::_jump,
        &EventHandler::_crouch,
        &EventHandler::_front,
        &EventHandler::_back,
        &EventHandler::_right,
        &EventHandler::_left,
        &EventHandler::_add_block,
        &EventHandler::_remove_block,
    };
    _movements = glm::ivec3(0);
    _mouse_pos = glm::ivec2(0);

    // Checking Timers
    auto now = std::chrono::high_resolution_clock::now();
    for (uint8_t i = 0; i < NB_EVENT_TIMER_TYPES; ++i) {
        std::chrono::duration<double> time_diff = now - _timers.time_ref[i];
        _timers.accept_event[i] = (time_diff.count() > _timers.timer_values[i]);
    }
    // Looping over events types
    for (uint8_t i = 0; i < NBR_IO_EVENTS; ++i) {
        if (events.events[i]) {
            std::invoke(func[i], this);
        }
    }
    // Setting timers origin
    for (uint8_t i = 0; i < NB_EVENT_TIMER_TYPES; ++i) {
        if (_timers.updated[i]) {
            _timers.time_ref[i] = now;
        }
        _timers.updated[i] = 0;
    }

    // Updating perspective
    if (_io_manager->wasResized()) {
        _perspective->ratio = _io_manager->getWindowRatio();
        _camera->setPerspective(
          glm::perspective(glm::radians(_perspective->fov),
                           _perspective->ratio,
                           _perspective->near_far.x,
                           _perspective->near_far.y));
    }
    // Camera updating
    if (_io_manager->isMouseExclusive()) {
        _updateCamera();
    }
}

EventHandler::EventTimers::EventTimers()
  : accept_event()
  , updated()
  , time_ref()
  , timer_values()
{
    timer_values[SYSTEM] = SYSTEM_TIMER_SECONDS;
    timer_values[CONFIG] = CONFIG_TIMER_SECONDS;
    timer_values[ACTION] = ACTION_TIMER_SECONDS;
}

void
EventHandler::_mouse_exclusive()
{
    if (_timers.accept_event[SYSTEM]) {
        _previous_mouse_pos = _mouse_pos;
        _io_manager->toggleMouseExclusive();
        _timers.accept_event[SYSTEM] = 0;
        _timers.updated[SYSTEM] = 1;
    }
}

void
EventHandler::_close_win_event()
{
    if (_timers.accept_event[SYSTEM]) {
        _io_manager->triggerClose();
        _timers.accept_event[SYSTEM] = 0;
        _timers.updated[SYSTEM] = 1;
    }
}

void
EventHandler::_toggle_fullscreen()
{
    if (_timers.accept_event[SYSTEM]) {
        _io_manager->toggleFullscreen();
        _timers.accept_event[SYSTEM] = 0;
        _timers.updated[SYSTEM] = 1;
    }
}

void
EventHandler::_jump()
{
    _movements.z += 1;
}

void
EventHandler::_crouch()
{
    _movements.z -= 1;
}

void
EventHandler::_front()
{
    _movements.x += 1;
}

void
EventHandler::_back()
{
    _movements.x -= 1;
}

void
EventHandler::_right()
{
    _movements.y += 1;
}

void
EventHandler::_left()
{
    _movements.y -= 1;
}

void
EventHandler::_add_block()
{
    if (_timers.accept_event[ACTION]) {
        std::cout << "LEFT CLICK" << std::endl;
        _timers.accept_event[ACTION] = 0;
        _timers.updated[ACTION] = 1;
    }
}

void
EventHandler::_remove_block()
{
    if (_timers.accept_event[ACTION]) {
        std::cout << "RIGHT CLICK" << std::endl;
        _timers.accept_event[ACTION] = 0;
        _timers.updated[ACTION] = 1;
    }
}

void
EventHandler::_updateCamera()
{
    glm::vec2 offset = _mouse_pos - _previous_mouse_pos;

    if (_movements != glm::ivec3(0)) {
        _camera->update_position(_movements);
    }
    if (offset != glm::vec2(0.0)) {
        _camera->update_front(offset);
    }
    _camera->update_matricies();
}