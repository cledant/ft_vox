#include "Engine.hpp"

#include <random>

Engine::Engine()
  : _io_manager()
  , _camera()
  , _event_handler()
  , _perspective_data()
  , _ui()
  , _cm()
  , _skybox()
  , _nb_frame(0)
  , _fps_count_timeref()
  , _str_fps("0")
{}

void
Engine::init(Options const &opts)
{
    _event_handler.setCamera(&_camera);
    _event_handler.setIOManager(&_io_manager);
    _event_handler.setPerspectiveData(&_perspective_data);
    _event_handler.setUi(&_ui);
    _event_handler.setChunkManager(&_cm);
    _event_handler.setSkybox(&_skybox);
    _event_handler.setInvertYAxis(opts.invert_y_axis);
    _io_manager.createWindow("ft_vox");
    _perspective_data.near_far = DEFAULT_NEAR_FAR;
    _perspective_data.fov = DEFAULT_FOV;
    _perspective_data.ratio = _io_manager.getWindowRatio();
    _camera.setPerspective(glm::perspective(glm::radians(_perspective_data.fov),
                                            _perspective_data.ratio,
                                            _perspective_data.near_far.x,
                                            _perspective_data.near_far.y));
    _camera.setPosition(START_POS);
    _cm.init(opts.seed.value_or(_generate_seed()));
    _ui.init(glm::vec2(IOManager::WIN_W, IOManager::WIN_H), _cm.getSeed());
    _skybox.init("./ressources/textures/skybox.png");
    _fps_count_timeref = std::chrono::steady_clock::now();
}

void
Engine::run()
{
    while (!_io_manager.shouldClose()) {
        _io_manager.clear();
        _event_handler.processEvents(_io_manager.getEvents());
        _cm.draw(_camera.getPerspectiveViewMatrix(),
                 _camera.getFrustumPlanes(),
                 _camera.getAbsFrustumPlanes());
        _skybox.draw(_camera.getPerspectiveViewMatrix(),
                     _perspective_data.near_far.y);
        if (_event_handler.printUi()) {
            UiInfo info = { _str_fps,
                            _camera.getPosition(),
                            _camera.getFront(),
                            _cm.getRenderDistance(),
                            _cm.getPlayerPosition(),
                            _cm.getNbInRangeChunks(),
                            _cm.getNbDisplayedChunk(),
                            _cm.getCurrentPlayerBlock(),
                            _cm.getSeed(),
                            _io_manager.getGpuInfo() };
            _ui.draw(info);
        }
        _io_manager.render();
        while (glGetError()) {
            std::cout << "Cleaning gl error" << std::endl;
        }
        _compute_fps();
    }
    _io_manager.deleteWindow();
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

uint32_t
Engine::_generate_seed()
{
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<uint32_t> dist;
    return (dist(gen));
}