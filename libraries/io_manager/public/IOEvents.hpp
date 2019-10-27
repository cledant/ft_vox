#ifndef FT_VOX_IOEVENTS_HPP
#define FT_VOX_IOEVENTS_HPP

#include <cstdint>
#include <array>

constexpr uint16_t NBR_IO_EVENTS = 11;

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
};

struct IOEvents
{
    IOEvents()
      : events()
      , mouse_w(0.0)
      , mouse_h(0.0){};
    virtual ~IOEvents() = default;

    std::array<uint8_t, NBR_IO_EVENTS> events;
    double mouse_w;
    double mouse_h;
};

#endif // FT_VOX_IOEVENTS_HPP
