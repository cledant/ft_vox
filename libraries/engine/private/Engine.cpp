#include "Engine.hpp"

Engine::Engine()
  : _io_manager()
  , _camera()
  , _event_handler()
{}

void
Engine::init()
{
    _event_handler.setCamera(&_camera);
    _event_handler.setIOManager(&_io_manager);
    _io_manager.createWindow("ft_vox");
}

void
Engine::run()
{
    while (!_io_manager.shouldClose()) {
        _event_handler.processEvents(_io_manager.getEvents());
        _io_manager.clear();
        _io_manager.render();
    }
}