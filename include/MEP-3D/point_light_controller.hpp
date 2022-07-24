#ifndef POINT_LIGHT_CONTROLLER_HPP
#define POINT_LIGHT_CONTROLLER_HPP

#include <MEP-3D/point_light.hpp>
#include <MEP-3D/shader.hpp>

#include <unordered_map>

using PointLightPtr = std::unique_ptr<PointLight>;
using PointLightContainer = std::vector<PointLightPtr>;

class PointLightController {
 public:
  PointLightController(
      unsigned int max_point_light,
      const std::string& struct_name,
      GLint light_count_location,
      std::unordered_map<LightUniforms, std::string> uniform_map);
  PointLightContainer::iterator MakeAndBind(const AmbientConfig& ambient_config,
                                            const PointConfig& point_config,
                                            float diffuse_intensity,
                                            const Shader& shader_);
  void ForAll(std::function<void(PointLightPtr&)> function);
  void Use();
  const PointLightContainer& GetContainer() const;
  PointLightPtr& operator[](std::size_t element);
  const PointLightPtr& operator[](std::size_t element) const;

 private:
  PointLightContainer point_light_container_;
  std::unordered_map<LightUniforms, std::string> uniform_map_;
  std::string struct_name_;
  GLint light_count_location_;
  const unsigned int max_size_;
};

#endif