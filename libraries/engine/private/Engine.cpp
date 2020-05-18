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
    _camera.setPosition(START_POS);
    _font.init("./ressources/fonts/Roboto-Light.ttf",
               "./ressources/shaders/font/font_vs.glsl",
               "./ressources/shaders/font/font_fs.glsl",
               glm::vec2(IOManager::WIN_W, IOManager::WIN_H),
               24);
    _cm.init();
    _fps_count_timeref = std::chrono::steady_clock::now();
}

void
Engine::run()
{
    while (!_io_manager.shouldClose()) {
        _io_manager.clear();
        _event_handler.processEvents(_io_manager.getEvents());
        _cm.update(_camera.getPosition());
        _cm.draw(_camera.getPerspectiveViewMatrix(),
                 _camera.getFrustumPlanes(),
                 _camera.getAbsFrustumPlanes());
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
    auto now = std::chrono::steady_clock::now();
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
    auto constexpr start_pos = glm::vec2(15.0f, 30.0f);
    static const std::array<void (Engine::*)(glm::vec2 const &screen_pos),
                            NB_DEBUG_UI>
      func = {
          &Engine::_print_ui_avg_fps,
          &Engine::_print_ui_camera_pos,
          &Engine::_print_ui_direction_vector,
          &Engine::_print_ui_player_chunk,
          &Engine::_print_ui_in_range_chunks,
          &Engine::_print_ui_nb_displayed_chunks,
          &Engine::_print_ui_render_dist,

      };

    for (uint32_t i = 0; i < NB_DEBUG_UI; ++i) {
        std::invoke(
          func[i], this, glm::vec2(start_pos.x, start_pos.y + 30.0f * i));
    }
}

void
Engine::_print_ui_avg_fps(glm::vec2 const &screen_pos)
{
    std::stringstream ss_fps;
    ss_fps.precision(2);
    ss_fps << "Avg FPS: " << _str_fps;
    _font.drawText(ss_fps.str(), glm::vec3(1.0f), screen_pos, 1.0f);
}

void
Engine::_print_ui_camera_pos(glm::vec2 const &screen_pos)
{
    std::stringstream ss_pos;
    ss_pos.precision(2);
    auto camera_pos = _camera.getPosition();
    ss_pos << "Cam Pos: " << std::fixed << "X = " << camera_pos.x
           << " | Y = " << camera_pos.y << " | Z = " << camera_pos.z;
    _font.drawText(ss_pos.str(), glm::vec3(1.0f), screen_pos, 1.0f);
}

void
Engine::_print_ui_direction_vector(glm::vec2 const &screen_pos)
{
    std::stringstream ss_dir;
    ss_dir.precision(2);
    auto camera_direction = _camera.getFront();
    ss_dir << "Cam Direction: " << std::fixed << "X = " << camera_direction.x
           << " | Y = " << camera_direction.y
           << " | Z = " << camera_direction.z;
    _font.drawText(ss_dir.str(), glm::vec3(1.0f), screen_pos, 1.0f);
}

void
Engine::_print_ui_render_dist(glm::vec2 const &screen_pos)
{
    std::stringstream ss_render_dist;
    ss_render_dist.precision(2);
    ss_render_dist << "Render Distance: " << std::fixed
                   << _cm.getRenderDistance();
    _font.drawText(ss_render_dist.str(), glm::vec3(1.0f), screen_pos, 1.0f);
}

void
Engine::_print_ui_player_chunk(glm::vec2 const &screen_pos)
{
    std::stringstream ss_ui;
    auto player_pos = _cm.getPlayerPosition();
    ss_ui.precision(2);
    ss_ui << "Player Chunk: X = " << std::fixed << player_pos.x
          << " | Y = " << player_pos.y;
    _font.drawText(ss_ui.str(), glm::vec3(1.0f), screen_pos, 1.0f);
}

void
Engine::_print_ui_in_range_chunks(glm::vec2 const &screen_pos)
{
    std::stringstream ss_ui;
    ss_ui.precision(2);
    ss_ui << "In Range Chunk: " << std::fixed << _cm.getNbInRangeChunks();
    _font.drawText(ss_ui.str(), glm::vec3(1.0f), screen_pos, 1.0f);
}

void
Engine::_print_ui_nb_displayed_chunks(glm::vec2 const &screen_pos)
{
    std::stringstream ss_ui;
    ss_ui.precision(2);
    ss_ui << "Displayed Chunk: " << std::fixed << _cm.getNbDisplayedChunk();
    _font.drawText(ss_ui.str(), glm::vec3(1.0f), screen_pos, 1.0f);
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