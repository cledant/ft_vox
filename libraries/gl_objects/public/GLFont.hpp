#ifndef FT_VOX_GLFONT_HPP
#define FT_VOX_GLFONT_HPP

#include "glad/glad.h"
#include "ft2build.h"
#include "glm/glm.hpp"

#include "GLShader.hpp"

#include FT_FREETYPE_H

class GLFont
{
  public:
    GLFont() = default;
    virtual ~GLFont() = default;
    GLFont(GLFont const &src) = delete;
    GLFont &operator=(GLFont const &rhs) = delete;
    GLFont(GLFont &&src) = delete;
    GLFont &operator=(GLFont &&rhs) = delete;

  private:

};

#endif // FT_VOX_GLFONT_HPP
