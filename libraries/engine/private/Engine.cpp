#include "Engine.hpp"

Engine::Engine()
  : _io_manager()
  , _camera()
  , _event_handler()
  , _perspective_data()
  , _font()
  , _nb_frame(0)
  , _fps_count_timeref()
  , _str_fps("0")
{}

void
Engine::init()
{
    _event_handler.setCamera(&_camera);
    _event_handler.setIOManager(&_io_manager);
    _event_handler.setPerspectiveData(&_perspective_data);
    _event_handler.setFont(&_font);
    _event_handler.setChunkManager(&_cm);
    _io_manager.createWindow("ft_vox");
    _perspective_data.near_far = DEFAULT_NEAR_FAR;
    _perspective_data.fov = DEFAULT_FOV;
    _perspective_data.ratio = _io_manager.getWindowRatio();
    _camera.setPerspective(glm::perspective(glm::radians(_perspective_data.fov),
                                            _perspective_data.ratio,
                                            _perspective_data.near_far.x,
                                            _perspective_data.near_far.y));
    _camera.setPosition(glm::vec3(-1.0f));
    _font.init("./ressources/fonts/Roboto-Light.ttf",
               "./ressources/shaders/font/font_vs.glsl",
               "./ressources/shaders/font/font_fs.glsl",
               glm::vec2(IOManager::WIN_W, IOManager::WIN_H),
               24);
    _cm.init();
    _fps_count_timeref = std::chrono::high_resolution_clock::now();
}

void
Engine::run()
{
    while (!_io_manager.shouldClose()) {
        _io_manager.clear();
        _event_handler.processEvents(_io_manager.getEvents());
        _cm.draw(_camera.getPerspectiveViewMatrix());
        if (_event_handler.printUi()) {
            _print_ui_info();
            _print_ui_keys();
        }
        _io_manager.render();
        while (glGetError()) {
            std::cout << "Cleaning gl error" << std::endl;
        }
        _compute_fps();
    }
}

void
Engine::_compute_fps()
{
    ++_nb_frame;
    auto now = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = now - _fps_count_timeref;
    if (diff.count() > 1.0f) {
        _str_fps = std::to_string(_nb_frame);
        _nb_frame = 0;
        _fps_count_timeref = now;
    }
}

void
Engine::_print_ui_info()
{
    // Draw Avg Fps
    std::stringstream ss_fps;
    ss_fps.precision(2);
    ss_fps << "Avg FPS: " << _str_fps;
    _font.drawText(
      ss_fps.str(), glm::vec3(1.0f), glm::vec2(15.0f, 30.0f), 1.0f);

    // Draw Camera Pos
    std::stringstream ss_pos;
    ss_pos.precision(2);
    auto camera_pos = _camera.getPosition();
    ss_pos << "Cam Pos: " << std::fixed << "X = " << camera_pos.x
           << " | Y = " << camera_pos.y << " | Z = " << camera_pos.z;
    _font.drawText(
      ss_pos.str(), glm::vec3(1.0f), glm::vec2(15.0f, 60.0f), 1.0f);

    // Draw Direction vector
    std::stringstream ss_dir;
    ss_dir.precision(2);
    auto camera_direction = _camera.getFront();
    ss_dir << "Cam Direction: " << std::fixed << "X = " << camera_direction.x
           << " | Y = " << camera_direction.y
           << " | Z = " << camera_direction.z;
    _font.drawText(
      ss_dir.str(), glm::vec3(1.0f), glm::vec2(15.0f, 90.0f), 1.0f);

    // Render distance
    std::stringstream ss_render_dist;
    ss_render_dist.precision(2);
    ss_render_dist << "Render Distance: " << std::fixed
                   << _cm.getRenderDistance();
    _font.drawText(
      ss_render_dist.str(), glm::vec3(1.0f), glm::vec2(15.0f, 120.0f), 1.0f);
}

void
Engine::_print_ui_keys()
{
    static const std::array<std::string, NB_KEY_DESCRIPTION> key_description = {
        "ESC = Close",
        "WASD = Movements",
        "QE = Down / Up",
        "- = Decrease Render Dist",
        "= = Increase Render Dist",
        "P = Toggle Mouse",
        "H = Toggle UI"
    };
    auto win_size = _io_manager.getWindowSize();

    for (uint16_t i = 0; i < NB_KEY_DESCRIPTION; ++i) {
        _font.drawText(key_description[i],
                       glm::vec3(1.0f),
                       glm::vec2(win_size.x - 300.0f, 30.0f * (i + 1.0f)),
                       1.0f);
    }
}