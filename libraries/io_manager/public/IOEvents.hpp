#ifndef FT_VOX_IOEVENTS_HPP
#define FT_VOX_IOEVENTS_HPP

#include <cstdint>
#include <array>

#include "glm/glm.hpp"

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
    NB_IO_EVENTS,
};

struct IOEvents
{
    std::array<uint8_t, NB_IO_EVENTS> events;
    glm::vec2 mouse_position;
    float mouse_scroll;
};

#endif // FT_VOX_IOEVENTS_HPP
