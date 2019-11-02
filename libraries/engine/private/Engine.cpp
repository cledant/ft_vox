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
               glm::vec2(IOManager::WIN_W, IOManager::WIN_H),
               32);
}

void
Engine::run()
{
    while (!_io_manager.shouldClose()) {
        _event_handler.processEvents(_io_manager.getEvents());
        _io_manager.clear();
        _print_ui_info();
        _io_manager.render();
    }
}

void
Engine::_print_ui_info()
{
    // Draw Camera Pos
    std::stringstream ss_pos;
    ss_pos.precision(2);
    auto camera_pos = _camera.getPosition();
    ss_pos << "Cam Pos: " << std::fixed << "X = " << camera_pos.x
           << " | Y = " << camera_pos.y << " | Z = " << camera_pos.z;
    _font.drawText(
      ss_pos.str(), glm::vec3(1.0f), glm::vec2(20.0f, 40.0f), 1.0f);

    // Draw Direction vector
    std::stringstream ss_dir;
    ss_dir.precision(2);
    auto camera_direction = _camera.getFront();
    ss_dir << "Cam Direction: " << std::fixed << "X = " << camera_direction.x
           << " | Y = " << camera_direction.y
           << " | Z = " << camera_direction.z;
    _font.drawText(
      ss_dir.str(), glm::vec3(1.0f), glm::vec2(20.0f, 80.0f), 1.0f);
}