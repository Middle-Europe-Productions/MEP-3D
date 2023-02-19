#ifndef SHADER_HPP
#define SHADER_HPP

#include <GL/glew.h>
#include <MEP-3D/asset.hpp>
#include <MEP-3D/asset_observer.hpp>
#include <MEP-3D/observer_list.hpp>
#include <glm/glm.hpp>
#include <optional>
#include <string>

struct ShaderStatus {
  bool created_from_file = false;
  std::optional<std::string> vertex_path;
  std::optional<std::string> fragment_path;
  bool contains_vertex_code = false;
  bool contains_fragment_code = false;
  std::optional<std::string> vertex_code;
  std::optional<std::string> fragment_code;
  bool is_vertex_compiled = false;
  bool is_fragment_compiled = false;
  bool shader_status = false;
};

class Shader : public Asset {
 public:
  Shader();
  Shader(const std::string& name);
  bool Compile(const std::string& vertex_code);
  bool Compile(const std::string& vertex_code,
               const std::string& fragment_code);
  bool IsCompiled() const;
  void Use() override;
  void Stop() override;
  bool CreateFromFile(const std::string& vertex_location);
  bool CreateFromString(const std::string& vertex_code);
  bool CreateFromFile(const std::string& vertex_location,
                      const std::string& fragment_location);
  bool CreateFromString(const std::string& vertex_code,
                        const std::string& fragment_code);
  void Clear();
  bool SetUniform(const std::string& name, const glm::mat4& matrix);
  bool SetUniform(const std::string& name, const glm::vec3& matrix);
  bool SetUniform(const std::string& name, float value);
  bool SetUniform(const std::string& name, int value);
  bool SetUniform(unsigned int id, const glm::mat4& matrix);
  bool SetUniform(unsigned int id, const glm::vec3& matrix);
  bool SetUniform(unsigned int id, float value);
  bool SetUniform(unsigned int id, int value);
  bool Cache(const std::string& name, unsigned int memory_id);
  const std::unordered_map<unsigned int, GLint>& GetUserCacheForDebug() const;
  const std::unordered_map<std::string, GLint>& GetAutoCacheForDebug() const;
  GLint GetUniform(const std::string& name) const;
  const ShaderStatus& GetShaderStatus() const;
  virtual ~Shader();

 protected:
  mutable std::unordered_map<unsigned int, GLint> uniform_user_memory_;
  mutable std::unordered_map<std::string, GLint> uniform_auto_memory_;
  GLuint shader_id_;

 private:
  bool CompileImpl(const std::string& vertex_code,
                   const std::string& fragment_code);
  bool SetUniformExt(GLuint uniform_location, const glm::mat4& matrix);
  bool SetUniformExt(GLuint uniform_location, const glm::vec3& matrix);
  bool SetUniformExt(GLuint uniform_location, float value);
  bool SetUniformExt(GLuint uniform_location, int value);
  bool AddShader(GLuint program,
                 const std::string& shader_code,
                 GLenum shader_type);
  bool GetUniformLocation(GLint& location, const std::string& name);
  ShaderStatus status_;
};

#endif