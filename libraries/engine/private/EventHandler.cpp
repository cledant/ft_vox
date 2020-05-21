#include "EventHandler.hpp"

#include <functional>
#include <iostream>

EventHandler::EventHandler()
  : _camera(nullptr)
  , _io_manager(nullptr)
  , _perspective(nullptr)
  , _cm(nullptr)
  , _font(nullptr)
  , _timers()
  , _movements(0)
  , _mouse_pos(0.0)
  , _previous_mouse_pos(0.0)
  , _print_ui(1)
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
EventHandler::setFont(GLFont *font)
{
    _font = font;
}

void
EventHandler::setChunkManager(ChunkManager *cm)
{
    _cm = cm;
}

uint8_t
EventHandler::printUi() const
{
    return (_print_ui);
}

void
EventHandler::processEvents(IOEvents const &events)
{
    assert(_camera);
    assert(_io_manager);
    assert(_perspective);
    assert(_font);
    assert(_cm);

    // Resetting movement tracking
    _movements = glm::ivec3(0);

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
        &EventHandler::_increase_render_distance,
        &EventHandler::_decrease_render_distance,
        &EventHandler::_toggle_ui,
        &EventHandler::_speed_up,
    };

    // Checking Timers
    auto now = std::chrono::steady_clock::now();
    for (uint8_t i = 0; i < NB_EVENT_TIMER_TYPES; ++i) {
        std::chrono::duration<double> time_diff = now - _timers.time_ref[i];
        _timers.timer_diff[i] = time_diff.count();
        _timers.accept_event[i] = (time_diff.count() > _timers.timer_values[i]);
    }

    // Looping over events types
    for (uint8_t i = 0; i < NBR_IO_EVENTS; ++i) {
        if (events.events[i]) {
            std::invoke(func[i], this);
        }
    }

    // Camera updating
    if (_io_manager->isMouseExclusive()) {
        _updateCamera(events.mouse_position);
    }
    _timers.updated[CAMERA] = 1;

    // Updating perspective + ortho
    if (_timers.updated[RENDER_DISTANCE]) {
        auto new_render_dist =
          static_cast<float>(_cm->getRenderDistance() + 1) * 20.0f;
        if (new_render_dist > (_perspective->near_far.y * 0.5f)) {
            _perspective->near_far.y *= 2.0f;
        }
    }
    if (_io_manager->wasResized()) {
        _font->setOrthographicProjection(_io_manager->getWindowSize());
        _perspective->ratio = _io_manager->getWindowRatio();
    }
    if (_io_manager->wasResized() || _timers.updated[RENDER_DISTANCE]) {
        _camera->setPerspective(
          glm::perspective(glm::radians(_perspective->fov),
                           _perspective->ratio,
                           _perspective->near_far.x,
                           _perspective->near_far.y));
    }

    // Setting timers origin
    for (uint8_t i = 0; i < NB_EVENT_TIMER_TYPES; ++i) {
        if (_timers.updated[i]) {
            _timers.time_ref[i] = now;
        }
        _timers.updated[i] = 0;
    }
}

EventHandler::EventTimers::EventTimers()
  : accept_event()
  , updated()
  , time_ref()
  , timer_diff()
  , timer_values()
{
    timer_values[SYSTEM] = SYSTEM_TIMER_SECONDS;
    timer_values[CONFIG] = CONFIG_TIMER_SECONDS;
    timer_values[ACTION] = ACTION_TIMER_SECONDS;
    timer_values[CAMERA] = TARGET_PLAYER_TICK_DURATION;
    timer_values[RENDER_DISTANCE] = CONFIG_TIMER_SECONDS;
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
EventHandler::_increase_render_distance()
{
    if (_timers.accept_event[RENDER_DISTANCE]) {
        _cm->increaseRenderDistance();
        _timers.accept_event[RENDER_DISTANCE] = 0;
        _timers.updated[RENDER_DISTANCE] = 1;
    }
}

void
EventHandler::_decrease_render_distance()
{
    if (_timers.accept_event[RENDER_DISTANCE]) {
        _cm->decreaseRenderDistance();
        _timers.accept_event[RENDER_DISTANCE] = 0;
        _timers.updated[RENDER_DISTANCE] = 1;
    }
}

void
EventHandler::_toggle_ui()
{
    if (_timers.accept_event[SYSTEM]) {
        _print_ui = !_print_ui;
        _timers.accept_event[SYSTEM] = 0;
        _timers.updated[SYSTEM] = 1;
    }
}

void
EventHandler::_speed_up()
{
    _movements *= 20.0f;
}

void
EventHandler::_updateCamera(glm::vec2 const &mouse_pos)
{
    static uint8_t first_run = 1;

    _mouse_pos = mouse_pos;
    if (first_run) {
        _previous_mouse_pos = _mouse_pos;
        first_run = 0;
    }
    glm::vec2 offset = _mouse_pos - _previous_mouse_pos;

    if (_movements != glm::ivec3(0)) {
        _camera->update_position(_movements,
                                 _timers.timer_diff[CAMERA] /
                                   _timers.timer_values[CAMERA]);
    }
    if (offset != glm::vec2(0.0)) {
        _camera->update_front(
          offset, _timers.timer_diff[CAMERA] / _timers.timer_values[CAMERA]);
        _previous_mouse_pos = _mouse_pos;
    }
    _camera->update_matricies();
}