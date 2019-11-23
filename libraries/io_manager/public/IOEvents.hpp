#ifndef FT_VOX_IOEVENTS_HPP
#define FT_VOX_IOEVENTS_HPP

#include <cstdint>
#include <array>

#include "glm/glm.hpp"

constexpr uint16_t NBR_IO_EVENTS = 15;

enum IOEventsTypes
{
    MOUSE_EXCLUSIVE = 0,
    ESCAPE,
    FULLSCREEN,
    JUMP,
    CROUCH,
    FRONT,
    BACK,
    RIGHT,
    LEFT,
    ADD_BLOCK,
    REMOVE_BLOCK,
    INCREASE_RENDER_DISTANCE,
    DECREASE_RENDER_DISTANCE,
    SHOW_UI,
    SPEED_UP,
};

struct IOEvents
{
    IOEvents()
      : events()
      , mouse_position(0.0){};
    virtual ~IOEvents() = default;

    std::array<uint8_t, NBR_IO_EVENTS> events;
    glm::vec2 mouse_position;
};

#endif // FT_VOX_IOEVENTS_HPP
