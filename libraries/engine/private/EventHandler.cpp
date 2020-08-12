#include "EventHandler.hpp"

#include <functional>
#include <iostream>

EventHandler::EventHandler()
  : _camera(nullptr)
  , _io_manager(nullptr)
  , _perspective(nullptr)
  , _cm(nullptr)
  , _ui(nullptr)
  , _skybox(nullptr)
  , _timers()
  , _movements(0)
  , _mouse_pos(0.0)
  , _previous_mouse_pos(0.0)
  , _print_ui(1)
  , _invert_y_axis(0)
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
EventHandler::setUi(Ui *Ui)
{
    _ui = Ui;
}

void
EventHandler::setChunkManager(ChunkManager *cm)
{
    _cm = cm;
}

void
EventHandler::setSkybox(Skybox *skybox)

{
    _skybox = skybox;
}

void
EventHandler::setInvertYAxis(uint8_t val)
{
    _invert_y_axis = val;
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
    assert(_ui);
    assert(_cm);
    assert(_skybox);
    assert(_ui);

    // Resetting movement tracking
    _movements = glm::ivec3(0);

    static const std::array<void (EventHandler::*)(), NB_IO_EVENTS>
      keyboard_events = {
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
          &EventHandler::_show_map,
      };

    // Checking Timers
    auto now = std::chrono::steady_clock::now();
    for (uint8_t i = 0; i < ET_NB_EVENT_TIMER_TYPES; ++i) {
        std::chrono::duration<double> time_diff = now - _timers.time_ref[i];
        _timers.timer_diff[i] = time_diff.count();
        _timers.accept_event[i] = (time_diff.count() > _timers.timer_values[i]);
    }

    // Looping over events types
    for (uint8_t i = 0; i < NB_IO_EVENTS; ++i) {
        if (events.events[i]) {
            std::invoke(keyboard_events[i], this);
        }
    }

    // Camera updating
    if (_io_manager->isMouseExclusive()) {
        _updateCamera(events.mouse_position);
    }
    _timers.updated[ET_CAMERA] = 1;

    // Updating perspective + ortho
    if (_timers.updated[ET_RENDER_DISTANCE]) {
        auto new_render_dist =
          static_cast<float>(_cm->getRenderDistance() + 1) * 20.0f;
        if (new_render_dist > (_perspective->near_far.y * 0.5f)) {
            _perspective->near_far.y *= 2.0f;
        }
    }
    if (_io_manager->wasResized()) {
        _ui->setOrthographicProjection(_io_manager->getWindowSize());
        _perspective->ratio = _io_manager->getWindowRatio();
    }
    if (_io_manager->wasResized() || _timers.updated[ET_RENDER_DISTANCE]) {
        _camera->setPerspective(
          glm::perspective(glm::radians(_perspective->fov),
                           _perspective->ratio,
                           _perspective->near_far.x,
                           _perspective->near_far.y));
    }

    // Player position related update
    _cm->update(_camera->getPosition());
    _skybox->update(_camera->getPosition());

    // Change player block
    if (_timers.accept_event[ET_PLAYER_BLOCK_CHANGE]) {
        if (events.mouse_scroll > 0.001f) {
            _cm->increaseCurrentPlayerBlock();
        } else if (events.mouse_scroll < -0.001f) {
            _cm->decreaseCurrentPlayerBlock();
        }
        _io_manager->resetMouseScroll();
        _timers.accept_event[ET_PLAYER_BLOCK_CHANGE] = 0;
        _timers.updated[ET_PLAYER_BLOCK_CHANGE] = 1;
    }

    // Interaction related update (Needs to be done after camera update)
    if (_timers.updated[ET_ADD_BLOCK]) {
        _cm->addBlock(_camera->getPosition(), _camera->getFront());
    }
    if (_timers.updated[ET_REMOVE_BLOCK]) {
        _cm->removeBlock(_camera->getPosition(), _camera->getFront());
    }

    // Setting timers origin
    for (uint8_t i = 0; i < ET_NB_EVENT_TIMER_TYPES; ++i) {
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
    timer_values[ET_SYSTEM] = SYSTEM_TIMER_SECONDS;
    timer_values[ET_CONFIG] = CONFIG_TIMER_SECONDS;
    timer_values[ET_ADD_BLOCK] = FAST_ACTION_TIMER_SECONDS;
    timer_values[ET_REMOVE_BLOCK] = FAST_ACTION_TIMER_SECONDS;
    timer_values[ET_CAMERA] = TARGET_PLAYER_TICK_DURATION;
    timer_values[ET_RENDER_DISTANCE] = CONFIG_TIMER_SECONDS;
    timer_values[ET_PLAYER_BLOCK_CHANGE] = ACTION_TIMER_SECONDS;
}

void
EventHandler::_mouse_exclusive()
{
    if (_timers.accept_event[ET_SYSTEM]) {
        _previous_mouse_pos = _mouse_pos;
        _io_manager->toggleMouseExclusive();
        _timers.accept_event[ET_SYSTEM] = 0;
        _timers.updated[ET_SYSTEM] = 1;
    }
}

void
EventHandler::_close_win_event()
{
    if (_timers.accept_event[ET_SYSTEM]) {
        _io_manager->triggerClose();
        _timers.accept_event[ET_SYSTEM] = 0;
        _timers.updated[ET_SYSTEM] = 1;
    }
}

void
EventHandler::_toggle_fullscreen()
{
    if (_timers.accept_event[ET_SYSTEM]) {
        _io_manager->toggleFullscreen();
        _timers.accept_event[ET_SYSTEM] = 0;
        _timers.updated[ET_SYSTEM] = 1;
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
    if (_timers.accept_event[ET_ADD_BLOCK] &&
        _timers.accept_event[ET_REMOVE_BLOCK]) {
        _timers.accept_event[ET_ADD_BLOCK] = 0;
        _timers.updated[ET_ADD_BLOCK] = 1;
    }
}

void
EventHandler::_remove_block()
{
    if (_timers.accept_event[ET_ADD_BLOCK] &&
        _timers.accept_event[ET_REMOVE_BLOCK]) {
        _timers.accept_event[ET_REMOVE_BLOCK] = 0;
        _timers.updated[ET_REMOVE_BLOCK] = 1;
    }
}

void
EventHandler::_increase_render_distance()
{
    if (_timers.accept_event[ET_RENDER_DISTANCE]) {
        _cm->increaseRenderDistance();
        _timers.accept_event[ET_RENDER_DISTANCE] = 0;
        _timers.updated[ET_RENDER_DISTANCE] = 1;
    }
}

void
EventHandler::_decrease_render_distance()
{
    if (_timers.accept_event[ET_RENDER_DISTANCE]) {
        _cm->decreaseRenderDistance();
        _timers.accept_event[ET_RENDER_DISTANCE] = 0;
        _timers.updated[ET_RENDER_DISTANCE] = 1;
    }
}

void
EventHandler::_toggle_ui()
{
    if (_timers.accept_event[ET_SYSTEM]) {
        _print_ui = !_print_ui;
        _timers.accept_event[ET_SYSTEM] = 0;
        _timers.updated[ET_SYSTEM] = 1;
    }
}

void
EventHandler::_speed_up()
{
    _movements *= 20.0f;
}

void
EventHandler::_show_map()
{}

void
EventHandler::_updateCamera(glm::vec2 const &mouse_pos)
{
    static uint8_t first_run = 1;

    _mouse_pos = mouse_pos;
    if (_invert_y_axis) {
        _mouse_pos.y = -mouse_pos.y;
    }
    if (first_run) {
        _previous_mouse_pos = _mouse_pos;
        first_run = 0;
    }
    glm::vec2 offset = _mouse_pos - _previous_mouse_pos;

    if (_movements != glm::ivec3(0)) {
        _camera->update_position(_movements,
                                 _timers.timer_diff[ET_CAMERA] /
                                   _timers.timer_values[ET_CAMERA]);
    }
    if (offset != glm::vec2(0.0)) {
        _camera->update_front(offset, 0.5f);
        _previous_mouse_pos = _mouse_pos;
    }
    _camera->update_matricies();
}