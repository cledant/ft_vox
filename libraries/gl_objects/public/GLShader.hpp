#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>
#include <unordered_map>

#include "glm/glm.hpp"

class GLShader final
{
  public:
    GLShader();
    ~GLShader() = default;
    GLShader(GLShader &&src) noexcept;
    GLShader &operator=(GLShader &&rhs) noexcept;
    GLShader(GLShader const &src) = delete;
    GLShader &operator=(GLShader const &rhs) = delete;

    void init(std::string const &path_vs,
              std::string const &path_fs,
              std::string const &prog_name);
    void init(std::string const &path_vs,
              std::string const &path_gs,
              std::string const &path_fs,
              std::string const &prog_name);
    void clear();
    void use() const;
    void setVec2(std::string const &name, glm::vec2 const &data);
    void setVec3(std::string const &name, glm::vec3 const &data);
    void setVec4(std::string const &name, glm::vec4 const &data);
    void setVec4Array(std::string const &name,
                      glm::vec4 const &data,
                      uint64_t array_size);
    void setMat4(std::string const &name, glm::mat4 const &data);
    void setInt(std::string const &name, int data);
    void setFloat(std::string const &name, float data);
    void setUbo(std::string const &name,
                uint32_t index,
                uint32_t ubo,
                size_t size);

  private:
    inline void _readFile(std::string const &path, std::string &content) const;
    [[nodiscard]] inline uint32_t _loadShader(std::string const &path,
                                              int32_t shader_type) const;
    [[nodiscard]] inline uint32_t _linkShaders(int32_t vs, int32_t fs) const;
    [[nodiscard]] inline uint32_t _linkShaders(int32_t vs,
                                               int32_t gs,
                                               int32_t fs) const;
    [[nodiscard]] inline std::string _shaderError(uint32_t shader) const;
    inline int32_t _setGetUniform(std::string const &name);
    inline uint32_t _setGetUboUniform(std::string const &name);

    uint8_t _is_init;
    uint32_t _program;
    std::unordered_map<std::string, int32_t> _uniform_id;
    std::unordered_map<std::string, uint32_t> _ubo_uniform_id;
    std::string _prog_name;
};

#endif