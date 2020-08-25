#include "Ui.hpp"

#include <sstream>

#include "NoiseUtils.hpp"

Ui::Ui()
  : _font()
  , _cursor()
  , _map()
  , _win_size(1.0f)
  , _ortho(1.0f)
  , _show_map(0)
{}

void
Ui::init(glm::vec2 const &window_size, uint32_t seed)
{
    std::array<uint8_t, 3 *MAP_SIZE.x *MAP_SIZE.y> map_buffer = {};

    _font.init("./ressources/fonts/Roboto-Light.ttf",
               "./ressources/shaders/font/font_vs.glsl",
               "./ressources/shaders/font/font_fs.glsl",
               24);
    _cursor.init("./ressources/textures/crosshair.png",
                 glm::vec2(25.0f),
                 window_size,
                 "./ressources/shaders/cursor/cursor_vs.glsl",
                 "./ressources/shaders/cursor/cursor_fs.glsl",
                 "Cursor");
    generateMap(MAP_SIZE, seed, map_buffer.data());
    _map.init(map_buffer.data(),
              MAP_SIZE,
              3,
              glm::ivec2(window_size.y - OFFSET_MAP),
              glm::vec2(window_size / 2.0f),
              "./ressources/shaders/ui_texture/ui_texture_vs.glsl",
              "./ressources/shaders/ui_texture/ui_texture_fs.glsl",
              "Ui_texture");
    _win_size = window_size;
    _ortho = glm::ortho(0.0f, _win_size.x, 0.0f, _win_size.y);
}

void
Ui::draw(UiInfo const &info)
{
    // Setup data to be displayed
    std::array<std::stringstream, UI_DBG_TOTAL> sstream_array;

    sstream_array[UI_DBG_AVG_FPS].precision(2);
    sstream_array[UI_DBG_AVG_FPS] << "Avg FPS: " << info.avg_fps;

    sstream_array[UI_DBG_GPU_INFO] << "GPU: " << info.gpu_info;

    sstream_array[UI_DBG_CAM_POSITION].precision(2);
    sstream_array[UI_DBG_CAM_POSITION]
      << "Cam Pos: " << std::fixed << "X = " << info.camera_pos.x
      << " | Y = " << info.camera_pos.y << " | Z = " << info.camera_pos.z;

    sstream_array[UI_DBG_CAM_DIRECTION].precision(2);
    sstream_array[UI_DBG_CAM_DIRECTION] << "Cam Direction: " << std::fixed
                                        << "X = " << info.camera_direction.x
                                        << " | Y = " << info.camera_direction.y
                                        << " | Z = " << info.camera_direction.z;

    sstream_array[UI_DBG_RENDER_DISTANCE].precision(2);
    sstream_array[UI_DBG_RENDER_DISTANCE] << "Render Distance: " << std::fixed
                                          << info.render_dist;

    sstream_array[UI_DBG_PLAYER_POSITION].precision(2);
    sstream_array[UI_DBG_PLAYER_POSITION] << "Player Chunk: X = " << std::fixed
                                          << info.player_pos.x
                                          << " | Y = " << info.player_pos.y;
    sstream_array[UI_DBG_CHUNK_IN_RANGE].precision(2);
    sstream_array[UI_DBG_CHUNK_IN_RANGE] << "In Range Chunk: " << std::fixed
                                         << info.chunk_in_range;

    sstream_array[UI_DBG_CHUNK_DISPLAYED].precision(2);
    sstream_array[UI_DBG_CHUNK_DISPLAYED] << "Displayed Chunk: " << std::fixed
                                          << info.displayed_chunk;

    sstream_array[UI_DBG_PLAYER_CHUNK].precision(2);
    sstream_array[UI_DBG_PLAYER_CHUNK] << "Player Active Chunk: " << std::fixed
                                       << STR_BLOCK_TYPES[info.player_block];

    sstream_array[UI_DBG_SEED] << "Seed: " << info.seed;

    if (!_show_map) {
        _cursor.draw(_ortho);
    } else {
        _map.draw(_ortho);
    }
    _print_ui_info(sstream_array);
    _print_ui_keys();
    _show_map = 0;
}

void
Ui::setOrthographicProjection(glm::vec2 const &window_size)
{
    _win_size = window_size;
    _cursor.setCenter(window_size / 2.0f);
    _map.setCenter(window_size / 2.0f);
    _map.setPixelSize(glm::ivec2(window_size.y - OFFSET_MAP));
    _ortho = glm::ortho(0.0f, _win_size.x, 0.0f, _win_size.y);
}

void
Ui::displayMap()
{
    _show_map = 1;
}

void
Ui::_print_ui_info(
  std::array<std::stringstream, UI_DBG_TOTAL> const &sstream_array)
{
    auto constexpr const start_pos = glm::vec2(15.0f, 30.0f);

    for (uint32_t i = 0; i < UI_DBG_TOTAL; ++i) {
        _font.drawText(sstream_array[i].str(),
                       glm::vec3(1.0f),
                       glm::vec2(start_pos.x, start_pos.y + 30.0f * i),
                       _ortho,
                       _win_size,
                       1.0f);
    }
}

void
Ui::_print_ui_keys()
{
    static const std::array<std::string, NB_KEY_DESCRIPTION> key_description = {
        "ESC = Close",
        "WASD = Movements",
        "QE = Down / Up",
        "- = Decrease Render Dist",
        "= = Increase Render Dist",
        "P = Toggle Mouse",
        "F5 = Toggle Fullscreen",
        "H = Toggle UI",
    };

    for (uint16_t i = 0; i < NB_KEY_DESCRIPTION; ++i) {
        _font.drawText(key_description[i],
                       glm::vec3(1.0f),
                       glm::vec2(_win_size.x - 300.0f, 30.0f * (i + 1.0f)),
                       _ortho,
                       _win_size,
                       1.0f);
    }
}