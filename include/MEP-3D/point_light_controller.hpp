#ifndef POINT_LIGHT_CONTROLLER_HPP
#define POINT_LIGHT_CONTROLLER_HPP

#include <MEP-3D/point_light.hpp>
#include <MEP-3D/shader.hpp>

#include <unordered_map>

using PointLightPtr = std::unique_ptr<PointLight>;
using PointLightContainer = std::vector<PointLightPtr>;

class PointLightController {
 public:
  PointLightController(unsigned int max_light,
                       const Shader& shader,
                       const std::string& struct_name,
                       std::unordered_map<LightUniforms, std::string> content)
      : max_size_(max_light) {}

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
  const unsigned int max_size_;
};

#endif