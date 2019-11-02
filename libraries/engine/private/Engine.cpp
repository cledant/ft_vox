#include "Engine.hpp"

Engine::Engine()
  : _io_manager()
  , _camera()
  , _event_handler()
  , _perspective_data()
  , _font()
{}

void
Engine::init()
{
    _event_handler.setCamera(&_camera);
    _event_handler.setIOManager(&_io_manager);
    _event_handler.setPerspectiveData(&_perspective_data);
    _event_handler.setFont(&_font);
    _io_manager.createWindow("ft_vox");
    _perspective_data.near_far = DEFAULT_NEAR_FAR;
    _perspective_data.fov = DEFAULT_FOV;
    _perspective_data.ratio = _io_manager.getWindowRatio();
    _camera.setPerspective(glm::perspective(glm::radians(_perspective_data.fov),
                                            _perspective_data.ratio,
                                            _perspective_data.near_far.x,
                                            _perspective_data.near_far.y));
    _font.init("./ressources/fonts/Roboto-Light.ttf",
               "./ressources/shaders/font/font_vs.glsl",
               "./ressources/shaders/font/font_fs.glsl",
               glm::ortho(0.0f,
                          static_cast<float>(IOManager::WIN_W),
                          0.0f,
                          static_cast<float>(IOManager::WIN_H)),
               24);
}

void
Engine::run()
{
    while (!_io_manager.shouldClose()) {
        _event_handler.processEvents(_io_manager.getEvents());
        _io_manager.clear();
        _font.drawText(
          "ft_vox", glm::vec3(1.0f), glm::vec2(20.0f, 20.0f), 1.0f);
        _io_manager.render();
    }
}