#ifndef FT_VOX_SKYBOX_HPP
#define FT_VOX_SKYBOX_HPP

#include "glm/glm.hpp"

#include "GLShader.hpp"
#include "GLTexture2D.hpp"

class Skybox final
{
  public:
    Skybox();
    ~Skybox() = default;
    Skybox(const Skybox &src) = delete;
    Skybox &operator=(Skybox const &rhs) = delete;
    Skybox(Skybox &&src) noexcept;
    Skybox &operator=(Skybox &&rhs) noexcept;

    void init(std::string const &skybox_filepath);
    void update(glm::vec3 const &player_pos);
    void draw(glm::mat4 const &projection, float skybox_size);

  private:
    GLShader _shader;
    GLTexture2D _texture;

    uint32_t _vao;
    uint32_t _vbo;

    void _allocate_vao();
    void _allocate_vbo();
};

#endif // FT_VOX_SKYBOX_HPP
