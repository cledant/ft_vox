#ifndef FT_VOX_CURSOR_HPP
#define FT_VOX_CURSOR_HPP

#include <cstdint>
#include <string>

#include "glm/glm.hpp"

class Cursor final
{
  public:
    Cursor() = default;
    ~Cursor() = default;
    Cursor(Cursor const &src) = delete;
    Cursor &operator=(Cursor const &rhs) = delete;
    Cursor(Cursor &&src) noexcept;
    Cursor &operator=(Cursor &&rhs) noexcept;

    void init(std::string const &texture_path);
    void clear();

    void setCenter(glm::vec2 const &center_pos);
    void setSize(glm::vec2 const &size);

    void setOrthographicProjection(glm::vec2 const &window_size);

    void draw();

    void _allocate_vbo();
    void _allocate_vao();

    glm::mat4 _ortho;
    glm::vec2 _size;
    glm::vec2 _center;
    uint32_t _vbo;
    uint32_t _vao;

    static float constexpr _tex_vertices[] = {
        -1.0f, 1.0f,  0.5f, 0.0f, 1.0f, 1.0f,  1.0f,  0.5f, 1.0f, 1.0f,
        -1.0f, -1.0f, 0.5f, 0.0f, 0.0f, -1.0f, -1.0f, 0.5f, 0.0f, 0.0f,
        1.0f,  1.0f,  0.5f, 1.0f, 1.0f, 1.0f,  -1.0f, 0.5f, 1.0f, 0.0f
    };
};

#endif // FT_VOX_CURSOR_HPP
