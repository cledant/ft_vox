#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "glad/glad.h"

#include "GLTexture.hpp"

GLTexture::GLTexture(std::string const &filepath)
  : _tex_id(0)
  , _tex_w(0)
  , _tex_h(0)
  , _tex_nb_chan(0)
{
    _load_texture(filepath.c_str());
}

GLTexture::~GLTexture()
{
    if (_tex_id) {
        glDeleteTextures(1, &_tex_id);
    }
}

GLTexture::GLTexture(const char *filepath)
  : _tex_id(0)
  , _tex_w(0)
  , _tex_h(0)
  , _tex_nb_chan(0)
{
    _load_texture(filepath);
}

GLTexture::GLTexture(GLTexture &&src) noexcept
  : _tex_id(0)
  , _tex_w(0)
  , _tex_h(0)
  , _tex_nb_chan(0)
{
    *this = std::move(src);
}

GLTexture &
GLTexture::operator=(GLTexture &&rhs) noexcept
{
    _tex_id = rhs._tex_id;
    rhs._tex_id = 0;
    _tex_nb_chan = rhs._tex_nb_chan;
    _tex_h = rhs._tex_h;
    _tex_w = rhs._tex_w;
    return (*this);
}

uint32_t
GLTexture::getTextureID() const
{
    return (_tex_id);
}

void
GLTexture::_load_texture(const char *filepath)
{
    // TODO
    (void)filepath;
}