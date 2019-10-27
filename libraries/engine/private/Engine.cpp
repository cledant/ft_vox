#include "Engine.hpp"

Engine::Engine()
  : _io_manager()
  , _camera()
  , _event_handler()
  , _perspective_data()
{}

void
Engine::init()
{
    _event_handler.setCamera(&_camera);
    _event_handler.setIOManager(&_io_manager);
    _event_handler.setPerspectiveData(&_perspective_data);
    _io_manager.createWindow("ft_vox");
    _perspective_data.near_far = DEFAULT_NEAR_FAR;
    _perspective_data.fov = DEFAULT_FOV;
    _perspective_data.ratio = _io_manager.getWindowRatio();
    _camera.setPerspective(glm::perspective(glm::radians(_perspective_data.fov),
                                            _perspective_data.ratio,
                                            _perspective_data.near_far.x,
                                            _perspective_data.near_far.y));
}

void
Engine::run()
{
    while (!_io_manager.shouldClose()) {
        _event_handler.processEvents(_io_manager.getEvents());
        // Updating perspective
        if (_io_manager.wasResized()) {
            _perspective_data.ratio = _io_manager.getWindowRatio();
            _camera.setPerspective(
              glm::perspective(glm::radians(_perspective_data.fov),
                               _perspective_data.ratio,
                               _perspective_data.near_far.x,
                               _perspective_data.near_far.y));
        }
        _io_manager.clear();
        _io_manager.render();
    }
}