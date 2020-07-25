#ifndef FT_VOX_TESTBOXDRAWER_HPP
#define FT_VOX_TESTBOXDRAWER_HPP

#include <cstdint>
#include <array>

#include "glad/glad.h"
#include "glm/glm.hpp"

#include "GLShader.hpp"

class TestBoxDrawer final
{
  public:
    TestBoxDrawer();
    ~TestBoxDrawer();
    TestBoxDrawer(TestBoxDrawer const &src) = delete;
    TestBoxDrawer &operator=(TestBoxDrawer const &rhs) = delete;
    TestBoxDrawer(TestBoxDrawer &&src) = delete;
    TestBoxDrawer &operator=(TestBoxDrawer &&rhs) = delete;

    static constexpr uint64_t MAX_BOXES = 5;

    void init();

    void draw(glm::mat4 const &projection);

  private:
    inline void _allocate_vbo();
    inline void _allocate_vao();

    std::array<glm::vec3, MAX_BOXES> _array;

    // OpenGL ressources
    GLShader _shader;
    uint32_t _vao;
    uint32_t _vbo;
};

#endif // FT_VOX_TESTBOXDRAWER_HPP
