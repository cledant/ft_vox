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
    std::array<uint8_t, 3 * 512 * 512> map_buffer = {};

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
    std::array<std::stringstream, NB_DEBUG_UI> sstream_array;

    // Avg Fps
    sstream_array[0].precision(2);
    sstream_array[0] << "Avg FPS: " << info.avg_fps;

    // Camera pos
    sstream_array[1].precision(2);
    sstream_array[1] << "Cam Pos: " << std::fixed << "X = " << info.camera_pos.x
                     << " | Y = " << info.camera_pos.y
                     << " | Z = " << info.camera_pos.z;

    // Camera Direction
    sstream_array[2].precision(2);
    sstream_array[2] << "Cam Direction: " << std::fixed
                     << "X = " << info.camera_direction.x
                     << " | Y = " << info.camera_direction.y
                     << " | Z = " << info.camera_direction.z;

    // Render Distance
    sstream_array[3].precision(2);
    sstream_array[3] << "Render Distance: " << std::fixed << info.render_dist;

    // Player position
    sstream_array[4].precision(2);
    sstream_array[4] << "Player Chunk: X = " << std::fixed << info.player_pos.x
                     << " | Y = " << info.player_pos.y;

    // Chunk in range
    sstream_array[5].precision(2);
    sstream_array[5] << "In Range Chunk: " << std::fixed << info.chunk_in_range;

    // Chunk displayed
    sstream_array[6].precision(2);
    sstream_array[6] << "Displayed Chunk: " << std::fixed
                     << info.displayed_chunk;

    // Player Chunk
    sstream_array[7].precision(2);
    sstream_array[7] << "Player Active Chunk: " << std::fixed
                     << STR_BLOCK_TYPES[info.player_block];

    // Seed
    sstream_array[8] << "Seed: " << info.seed;

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
  std::array<std::stringstream, NB_DEBUG_UI> const &sstream_array)
{
    auto constexpr const start_pos = glm::vec2(15.0f, 30.0f);

    for (uint32_t i = 0; i < NB_DEBUG_UI; ++i) {
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
        "TAB = Show Map",
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