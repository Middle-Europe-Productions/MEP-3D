#ifndef POINT_LIGHT_CONTROLLER_HPP
#define POINT_LIGHT_CONTROLLER_HPP

#include <MEP-3D/point_light.hpp>
#include <MEP-3D/shader.hpp>

#include <unordered_map>

using PointLightPtr = std::unique_ptr<PointLight>;
using PointLightContainer = std::vector<PointLightPtr>;

class PointLightFactory {
 public:
  PointLightFactory(unsigned int max_point_light,
                    const std::string& struct_name,
                    std::unordered_map<LightUniforms, std::string> uniform_map)
      : uniform_map_(uniform_map),
        struct_name_(struct_name),
        max_size_(max_point_light) {}

  int MakeAndBind(const AmbientConfig& ambient_config,
                  const PointConfig& point_config,
                  float diffuse_intensity,
                  const Shader& shader_) {
    if (point_light_container_.size() >= max_size_) {
      LOG(INFO) << "Reached maximal number of point lights (" << max_size_
                << ")";
      return -1;
    }
    point_light_container_.emplace_back(std::make_unique<PointLight>(
        ambient_config, point_config, diffuse_intensity));
    point_light_container_.back()->BindUniforms(
        shader_, point_light_container_.size(), struct_name_, uniform_map_);
  }
  void ForAll(std::function<void(PointLightPtr&)> function) {
    for (auto& light : point_light_container_) {
      function(light);
    }
  }
  const PointLightContainer& GetContainer() const {
    return point_light_container_;
  }
  PointLightPtr& operator[](std::size_t element) {
    return point_light_container_[element];
  }
  const PointLightPtr& operator[](std::size_t element) const {
    return point_light_container_[element];
  }

 private:
  PointLightContainer point_light_container_;
  std::unordered_map<LightUniforms, std::string> uniform_map_;
  std::string struct_name_;
  const unsigned int max_size_;
};

#endif