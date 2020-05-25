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
  , _tex_w(0)
  , _tex_h(0)
  , _tex_nb_chan(0)
{
    loadTexture(filepath.c_str(), use_nearest_filtering);
}

GLTexture2D::GLTexture2D(const char *filepath, uint8_t use_nearest_filtering)
  : _tex_id(0)
  , _tex_w(0)
  , _tex_h(0)
  , _tex_nb_chan(0)
{
    loadTexture(filepath, use_nearest_filtering);
}

GLTexture2D::GLTexture2D(GLTexture2D &&src) noexcept
  : _tex_id(0)
  , _tex_w(0)
  , _tex_h(0)
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
    _tex_h = rhs._tex_h;
    _tex_w = rhs._tex_w;
    return (*this);
}

void
GLTexture2D::loadTexture(const char *filepath, uint8_t use_nearest_filtering)
{
    uint8_t *data;

    assert(filepath);
    std::cout << "Loading Texture 2d: " << filepath << std::endl;
    if (!(data = stbi_load(filepath, &_tex_w, &_tex_h, &_tex_nb_chan, 0))) {
        throw std::runtime_error("Failed to load texture 2d: " +
                                 std::string(filepath));
    }
    if (!_tex_h || !_tex_w || _tex_nb_chan < 3 || _tex_nb_chan > 4) {
        stbi_image_free(data);
        throw std::runtime_error("Invalid texture 2d: " +
                                 std::string(filepath));
    }

    // OpenGL stuff
    glGenTextures(1, &_tex_id);
    glBindTexture(GL_TEXTURE_2D, _tex_id);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 _tex_nb_chan == 3 ? GL_RGB : GL_RGBA,
                 _tex_w,
                 _tex_h,
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
    stbi_image_free(data);
    if (glGetError() != GL_NO_ERROR) {
        glDeleteTextures(1, &_tex_id);
        throw std::runtime_error("OpenGL Error for texture 2d: " +
                                 std::string(filepath));
    }
}

uint32_t
GLTexture2D::getTextureID() const
{
    return (_tex_id);
}