#ifndef UNIFORM_CONTAINER_HPP
#define UNIFORM_CONTAINER_HPP

#include <GL/glew.h>
#include <glog/logging.h>
#include <MEP-3D/shader.hpp>
#include <unordered_map>

namespace mep {
template <typename Type>
class UniformContainer {
 public:
  void BindUniform(GLint uniform_location, Type type);
  void BindUniforms(const Shader& shader,
                    std::unordered_map<Type, std::string> names_bind);
  void BindUniforms(const Shader& shader,
                    unsigned int id,
                    std::string structure_name,
                    std::unordered_map<Type, std::string> names_bind);
  bool Exists(Type type) const;
  GLint GetUniform(Type type) const;

 public:
  std::unordered_map<Type, GLint> uniform_cache_;
};

template <typename Type>
void UniformContainer<Type>::BindUniform(GLint uniform_location, Type type) {
  if (uniform_location < 0) {
    LOG(ERROR) << "Invalid uniform!";
    return;
  }
  if (uniform_cache_.find(type) != uniform_cache_.end()) {
    LOG(INFO) << "Uniform already bound overriding!";
  }
  uniform_cache_[type] = uniform_location;
}

template <typename Type>
void UniformContainer<Type>::BindUniforms(
    const Shader& shader,
    std::unordered_map<Type, std::string> names_bind) {
  for (auto& ele : names_bind) {
    GLint location = shader.GetUniform(ele.second);
    if (uniform_cache_.find(ele.first) != uniform_cache_.end()) {
      LOG(INFO) << "Overriding " << ele.second;
    }
    uniform_cache_[ele.first] = location;
  }
}
template <typename Type>
void UniformContainer<Type>::BindUniforms(
    const Shader& shader,
    unsigned int id,
    std::string structure_name,
    std::unordered_map<Type, std::string> names_bind) {
  std::string prefix = structure_name + "[" + std::to_string(id) + "].";
  for (auto& ele : names_bind) {
    GLint location = shader.GetUniform(prefix + ele.second);
    if (uniform_cache_.find(ele.first) != uniform_cache_.end()) {
      LOG(INFO) << "Overriding " << structure_name + ele.second;
    }
    uniform_cache_[ele.first] = location;
  }
}

template <typename Type>
bool UniformContainer<Type>::Exists(Type type) const {
  return uniform_cache_.find(type) != uniform_cache_.end();
}

template <typename Type>
GLint UniformContainer<Type>::GetUniform(Type type) const {
  if (!Exists(type))
    return -1;
  return uniform_cache_.find(type)->second;
}
}  // namespace mep

#endif