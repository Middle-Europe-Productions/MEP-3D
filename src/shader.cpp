#include <glog/logging.h>
#include <MEP-3D/common_names.hpp>
#include <MEP-3D/shader.hpp>
#include <MEP-3D/utils.hpp>
#include <array>
#include <fstream>
#include <glm/gtc/type_ptr.hpp>

namespace mep {
Shader::Shader() : Asset(kShader) {
  LOG(INFO) << __FUNCTION__ << ", " << ToString();
  shader_id_ = 0;
}

Shader::Shader(const std::string& name) : Asset(kShader, name.c_str()) {
  LOG(INFO) << __FUNCTION__ << ", " << ToString();
  shader_id_ = 0;
}

bool Shader::Compile(const std::string& vertex_code) {
  auto error = CompileImpl(vertex_code, "");
  if (error.error_occured) {
    LOG(ERROR) << "[GLSL] Error occured: " << error.error_message;
  }
  return error.error_occured;
}

bool Shader::Compile(const std::string& vertex_code,
                     const std::string& fragment_code) {
  auto error = CompileImpl(vertex_code, fragment_code);
  if (error.error_occured) {
    LOG(ERROR) << "[GLSL] Error occured: " << error.error_message;
  }
  return error.error_occured;
}

ShaderError Shader::ReCompile(const std::string& vertex_code) {
  Clear();
  return CompileImpl(vertex_code, "");
}

ShaderError Shader::ReCompile(const std::string& vertex_code,
                              const std::string& fragment_code) {
  Clear();
  return CompileImpl(vertex_code, fragment_code);
}

void Shader::Use() {
  glUseProgram(shader_id_);
}

void Shader::Stop() {
  glUseProgram(0);
}

bool Shader::CreateFromFile(const std::string& vertex_location) {
  status_.created_from_file = true;
  std::string vertex_code = utils::LoadFromFile(vertex_location);
  status_.vertex_path = vertex_location;
  if (vertex_code == "")
    return false;
  return CreateFromString(vertex_code);
}

bool Shader::CreateFromFile(const std::string& vertex_location,
                            const std::string& fragment_location) {
  status_.created_from_file = true;
  std::string vertex_code = utils::LoadFromFile(vertex_location);
  status_.vertex_path = vertex_location;
  std::string fragment_code = utils::LoadFromFile(fragment_location);
  status_.fragment_path = fragment_location;
  if (vertex_code == "" || fragment_code == "")
    return false;
  return CreateFromString(vertex_code, fragment_code);
}
bool Shader::CreateFromString(const std::string& vertex_code) {
  return Compile(vertex_code);
}

bool Shader::CreateFromString(const std::string& vertex_code,
                              const std::string& fragment_code) {
  return Compile(vertex_code, fragment_code);
}

void Shader::Clear() {
  if (shader_id_) {
    glDeleteProgram(shader_id_);
    shader_id_ = 0;
  }
  status_.shader_status = false;
}

bool Shader::Cache(const std::string& name, unsigned int memory_id) {
  GLint uniform_value_loc = glGetUniformLocation(shader_id_, name.c_str());
  if (uniform_value_loc == -1) {
    LOG(ERROR) << "Could not find a variable: " << name << "!";
    return false;
  }
  uniform_user_memory_[memory_id] = uniform_value_loc;
  return true;
}

const std::unordered_map<unsigned int, GLint>& Shader::GetUserCacheForDebug()
    const {
  return uniform_user_memory_;
}

const std::unordered_map<std::string, GLint>& Shader::GetAutoCacheForDebug()
    const {
  return uniform_auto_memory_;
}

bool Shader::SetUniform(const std::string& name, const glm::mat4& matrix) {
  GLint uniform_value_loc = glGetUniformLocation(shader_id_, name.c_str());
  if (uniform_value_loc == -1) {
    LOG(ERROR) << "Could not find a variable: " << name << "!";
    return false;
  }
  return SetUniformExt(uniform_value_loc, matrix);
}

bool Shader::SetUniform(const std::string& name, const glm::vec3& matrix) {
  GLint uniform_value_loc = glGetUniformLocation(shader_id_, name.c_str());
  if (uniform_value_loc == -1) {
    LOG(ERROR) << "Could not find a variable: " << name << "!";
    return false;
  }
  return SetUniformExt(uniform_value_loc, matrix);
}

bool Shader::SetUniform(const std::string& name, float value) {
  GLint uniform_value_loc = glGetUniformLocation(shader_id_, name.c_str());
  if (uniform_value_loc == -1) {
    LOG(ERROR) << "Could not find a variable: " << name << "!";
    return false;
  }
  return SetUniformExt(uniform_value_loc, value);
}

bool Shader::SetUniform(const std::string& name, int value) {
  GLint uniform_value_loc = glGetUniformLocation(shader_id_, name.c_str());
  if (uniform_value_loc == -1) {
    LOG(ERROR) << "Could not find a variable: " << name << "!";
    return false;
  }
  SetUniformExt(uniform_value_loc, value);
  return true;
}

bool Shader::SetUniform(unsigned int id, const glm::mat4& matrix) {
  auto local_uniform = uniform_user_memory_.find(id);
  if (local_uniform == uniform_user_memory_.end()) {
    return false;
  }
  SetUniformExt(local_uniform->second, matrix);
  return true;
}

bool Shader::SetUniform(unsigned int id, const glm::vec3& matrix) {
  auto local_uniform = uniform_user_memory_.find(id);
  if (local_uniform == uniform_user_memory_.end()) {
    return false;
  }
  SetUniformExt(local_uniform->second, matrix);
  return true;
}

bool Shader::SetUniform(unsigned int id, float value) {
  auto local_uniform = uniform_user_memory_.find(id);
  if (local_uniform == uniform_user_memory_.end()) {
    return false;
  }
  SetUniformExt(local_uniform->second, value);
  return true;
}

bool Shader::SetUniform(unsigned int id, int value) {
  auto local_uniform = uniform_user_memory_.find(id);
  if (local_uniform == uniform_user_memory_.end()) {
    return false;
  }
  SetUniformExt(local_uniform->second, value);
  return true;
}

GLint Shader::GetUniform(const std::string& name) const {
  auto memory_address = uniform_auto_memory_.find(name);
  if (memory_address != uniform_auto_memory_.end()) {
    return memory_address->second;
  }
  GLint uniform_value_loc = glGetUniformLocation(shader_id_, name.c_str());
  if (uniform_value_loc == -1) {
    LOG(ERROR) << "Could not find a variable: " << name << "!";
  }
  uniform_auto_memory_[name] = uniform_value_loc;
  return uniform_value_loc;
}

const ShaderStatus& Shader::GetShaderStatus() const {
  return status_;
}

Shader::~Shader() {
  LOG(INFO) << __FUNCTION__ << ", " << ToString();
  ForAllObservers(
      [this](AssetObserver* observer) { observer->OnDelete(*this); });
  Clear();
}

bool Shader::IsCompiled() const {
  if (status_.contains_fragment_code && !status_.is_fragment_compiled) {
    return false;
  }
  if (status_.contains_vertex_code && !status_.is_vertex_compiled) {
    return false;
  }
  return true;
}

ShaderError Shader::CompileImpl(const std::string& vertex_code,
                                const std::string& fragment_code) {
  LOG(INFO) << __FUNCTION__ << ", " << ToString();
  status_.shader_status = false;
  shader_id_ = glCreateProgram();

  if (!shader_id_) {
    return {true, "Could not create a program!"};
  }

  auto error = AddShader(shader_id_, vertex_code, GL_VERTEX_SHADER);
  if (error.error_occured) {
    error.vertex_error = std::move(error.error_message);
    error.error_message = "Compilation error";
    return error;
  } else {
    status_.is_vertex_compiled = true;
    status_.vertex_code = vertex_code;
  }

  if (fragment_code != "") {
    auto error = AddShader(shader_id_, fragment_code, GL_FRAGMENT_SHADER);
    if (error.error_occured) {
      error.fragment_error = std::move(error.error_message);
      error.error_message = "Compilation error";
      return error;
    } else {
      status_.is_fragment_compiled = true;
      status_.fragment_code = fragment_code;
    }
  }

  GLint result = 0;
  GLchar log[1024] = {0};

  glLinkProgram(shader_id_);
  glGetProgramiv(shader_id_, GL_LINK_STATUS, &result);
  if (!result) {
    glGetProgramInfoLog(shader_id_, sizeof(log), NULL, log);
    return {true, log};
  }

  glValidateProgram(shader_id_);
  glGetProgramiv(shader_id_, GL_VALIDATE_STATUS, &result);
  if (!result) {
    glGetProgramInfoLog(shader_id_, sizeof(log), NULL, log);
    return {true, log};
  }
  status_.shader_status = true;
  LOG(INFO) << __FUNCTION__ << ", " << ToString() << ", compilation finished!";
  return {false};
}

bool Shader::SetUniformExt(GLuint uniform_location, const glm::mat4& matrix) {
  glUniformMatrix4fv(uniform_location, 1, GL_FALSE, glm::value_ptr(matrix));
  return true;
}

bool Shader::SetUniformExt(GLuint uniform_location, const glm::vec3& matrix) {
  glUniform3f(uniform_location, matrix.x, matrix.y, matrix.z);
  return true;
}

bool Shader::SetUniformExt(GLuint uniform_location, float value) {
  glUniform1f(uniform_location, value);
  return true;
}

bool Shader::SetUniformExt(GLuint uniform_location, int value) {
  glUniform1i(uniform_location, value);
  return true;
}

ShaderError Shader::AddShader(GLuint program,
                              const std::string& shader_code,
                              GLenum shader_type) {
  GLuint gl_shader = glCreateShader(shader_type);

  const GLchar* code[1] = {shader_code.c_str()};
  GLint code_lenght[1] = {static_cast<GLint>(shader_code.size())};

  glShaderSource(gl_shader, 1, code, code_lenght);
  glCompileShader(gl_shader);

  GLint result = 0;
  std::array<GLchar, 1024> log;
  glGetShaderiv(gl_shader, GL_COMPILE_STATUS, &result);
  if (!result) {
    glGetShaderInfoLog(gl_shader, 1024, NULL, log.data());
    LOG(ERROR) << __FUNCTION__ << log.data();
    return {true, log.data()};
  }
  glAttachShader(program, gl_shader);
  return {false};
}

bool Shader::GetUniformLocation(GLint& location, const std::string& name) {
  GLint local_loc = glGetUniformLocation(shader_id_, name.c_str());
  if (local_loc == -1) {
    LOG(ERROR) << "Value: " + name + " does not exist in shader id: " +
                      std::to_string(GetId());
    return false;
  }
  location = local_loc;
  return true;
}
}  // namespace mep
