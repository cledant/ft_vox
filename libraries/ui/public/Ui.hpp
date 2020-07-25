#ifndef FT_VOX_UI_HPP
#define FT_VOX_UI_HPP

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "UiFont.hpp"
#include "UiTexture.hpp"

class Ui final
{
  public:
    Ui();
    ~Ui() = default;
    Ui(Ui const &src) = delete;
    Ui &operator=(Ui const &rhs) = delete;
    Ui(Ui &&src) = delete;
    Ui &operator=(Ui &&rhs) = delete;

    void init(glm::vec2 const &window_size);
    void draw(std::string const &avg_fps,
              glm::vec3 const &camera_pos,
              glm::vec3 const &camera_direction,
              uint64_t render_dist,
              glm::ivec2 const &player_pos,
              uint64_t chunk_in_range,
              uint64_t displayed_chunk,
              int32_t player_block);

    void setOrthographicProjection(glm::vec2 const &window_size);
    void displayMap();

  private:
    static constexpr uint32_t const NB_KEY_DESCRIPTION = 8;
    static constexpr uint32_t const NB_DEBUG_UI = 8;

    static constexpr char const *STR_BLOCK_TYPES[] = {
        "EMPTY",   "STONE", "GRASS",  "DIRT",  "COBBLESTONE", "PLANKS",
        "BEDROCK", "SAND",  "WATER",  "SNOW",  "SNOW_GRASS",  "WOOD",
        "FOLIAGE", "TNT",   "BRICKS", "GLASS", "ICE",
    };

    UiFont _font;
    UiTexture _cursor;
    glm::vec2 _win_size;
    glm::mat4 _ortho;
    uint8_t _show_map;

    inline void _print_ui_info(
      std::array<std::stringstream, NB_DEBUG_UI> const &sstream_array);
    inline void _print_ui_keys();
};

#endif // FT_VOX_UI_HPP
