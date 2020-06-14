#ifndef FT_VOX_UI_HPP
#define FT_VOX_UI_HPP

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Font.hpp"
#include "Cursor.hpp"

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
              uint64_t displayed_chunk);

    void setOrthographicProjection(glm::vec2 const &window_size);

  private:
    static constexpr uint32_t NB_KEY_DESCRIPTION = 7;
    static constexpr uint32_t NB_DEBUG_UI = 7;

    Font _font;
    Cursor _cursor;
    glm::vec2 _win_size;
    glm::mat4 _ortho;

    void _print_ui_info(
      std::array<std::stringstream, NB_DEBUG_UI> const &sstream_array);
    void _print_ui_keys();
};

#endif // FT_VOX_UI_HPP
