#ifndef FT_VOX_ENGINE_HPP
#define FT_VOX_ENGINE_HPP

#include "IOManager.hpp"
#include "Camera.hpp"
#include "EventHandler.hpp"

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
    IOManager _io_manager;
    Camera _camera;
    EventHandler _event_handler;
};

#endif // FT_VOX_ENGINE_HPP