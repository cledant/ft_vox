#include "TestBoxDrawer.hpp"

TestBoxDrawer::TestBoxDrawer()
  : _array()
  , _shader()
  , _vao(0)
  , _vbo(0)
{}

TestBoxDrawer::~TestBoxDrawer()
{
    glDeleteVertexArrays(1, &_vao);
    glDeleteBuffers(1, &_vbo);
}

void
TestBoxDrawer::init()
{
    for (uint8_t i = 0; i < 5; ++i) {
        _array[i] = glm::vec3(static_cast<float>(i));
    }
    _shader.init("./ressources/shaders/test_box_drawer/test_bd_vs.glsl",
                 "./ressources/shaders/test_box_drawer/test_bd_gs.glsl",
                 "./ressources/shaders/test_box_drawer/test_bd_fs.glsl",
                 "TestBoxDrawer");
    _allocate_vbo();
    _allocate_vao();
}

void
TestBoxDrawer::draw(glm::mat4 const &projection)
{
    _shader.use();
    _shader.setMat4("uniform_mat_perspec_view", projection);
    glBindVertexArray(_vao);
    glDrawArrays(GL_POINTS, 0, 5);
    glBindVertexArray(0);
}

void
TestBoxDrawer::_allocate_vbo()
{
    glGenBuffers(1, &_vbo);
    if (!_vbo) {
        throw std::runtime_error("TestBoxDrawer: Failed to create buffer");
    }
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(glm::vec3) * MAX_BOXES,
                 _array.data(),
                 GL_STATIC_DRAW);
    if (glGetError() == GL_OUT_OF_MEMORY) {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        throw std::runtime_error("TestBoxDrawer: Failed to allocate buffer");
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void
TestBoxDrawer::_allocate_vao()
{
    glGenVertexArrays(1, &_vao);
    if (!_vao) {
        throw std::runtime_error("TestBoxDrawer: Failed to create vao");
    }
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glVertexAttribPointer(
      0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, reinterpret_cast<void *>(0));
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}