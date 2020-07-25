#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "glad/glad.h"

#include "GLTexture2D.hpp"

GLTexture2D::~GLTexture2D()
{
    if (_tex_id) {
        glDeleteTextures(1, &_tex_id);
    }
}

GLTexture2D::GLTexture2D(std::string const &filepath,
                         uint8_t use_nearest_filtering)
  : _tex_id(0)
  , _tex_size(0)
  , _tex_nb_chan(0)
{
    init(filepath.c_str(), use_nearest_filtering);
}

GLTexture2D::GLTexture2D(const char *filepath, uint8_t use_nearest_filtering)
  : _tex_id(0)
  , _tex_size(0)
  , _tex_nb_chan(0)
{
    init(filepath, use_nearest_filtering);
}

GLTexture2D::GLTexture2D(GLTexture2D &&src) noexcept
  : _tex_id(0)
  , _tex_size(0)
  , _tex_nb_chan(0)
{
    *this = std::move(src);
}

GLTexture2D &
GLTexture2D::operator=(GLTexture2D &&rhs) noexcept
{
    _tex_id = rhs._tex_id;
    rhs._tex_id = 0;
    _tex_nb_chan = rhs._tex_nb_chan;
    _tex_size = rhs._tex_size;
    return (*this);
}

void
GLTexture2D::init(const char *filepath, uint8_t use_nearest_filtering)
{
    uint8_t *data;

    assert(filepath);
#ifndef NDEBUG
    std::cout << "Loading Texture 2d: " << filepath << std::endl;
#endif
    if (!(data = stbi_load(
            filepath, &_tex_size.x, &_tex_size.y, &_tex_nb_chan, 0))) {
        throw std::runtime_error("Failed to load texture 2d: " +
                                 std::string(filepath));
    }
    if (!_tex_size.x || !_tex_size.y || _tex_nb_chan < 3 || _tex_nb_chan > 4) {
        stbi_image_free(data);
        throw std::runtime_error("Invalid texture 2d: " +
                                 std::string(filepath));
    }

    _creating_gpu_tex(data, use_nearest_filtering);
    stbi_image_free(data);
    if (glGetError() != GL_NO_ERROR) {
        glDeleteTextures(1, &_tex_id);
        throw std::runtime_error("OpenGL Error for texture 2d: " +
                                 std::string(filepath));
    }
}

void
GLTexture2D::init(void const *buffer,
                  glm::ivec2 size,
                  int32_t nb_chan,
                  uint8_t use_nearest_filtering)
{
    _tex_size = size;
    _tex_nb_chan = nb_chan;

    _creating_gpu_tex(buffer, use_nearest_filtering);
    if (glGetError() != GL_NO_ERROR) {
        glDeleteTextures(1, &_tex_id);
        throw std::runtime_error("OpenGL Error for texture 2d from buffer");
    }
}

void
GLTexture2D::clear()
{
    if (_tex_id) {
        glDeleteTextures(1, &_tex_id);
    }
    _tex_size = glm::ivec2(0);
    _tex_nb_chan = 0;
}

uint32_t
GLTexture2D::getTextureID() const
{
    return (_tex_id);
}
void
GLTexture2D::_creating_gpu_tex(void const *data, uint8_t use_nearest_filtering)
{
    glGenTextures(1, &_tex_id);
    glBindTexture(GL_TEXTURE_2D, _tex_id);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 _tex_nb_chan == 3 ? GL_RGB : GL_RGBA,
                 _tex_size.x,
                 _tex_size.y,
                 0,
                 _tex_nb_chan == 3 ? GL_RGB : GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(
      GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    GLenum filtering_type =
      (use_nearest_filtering == 0) ? GL_LINEAR : GL_NEAREST;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering_type);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}
