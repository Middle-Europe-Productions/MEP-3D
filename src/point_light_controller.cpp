#include <MEP-3D/point_light_controller.hpp>

PointLightController::PointLightController(
    unsigned int max_point_light,
    const std::string& struct_name,
    GLint light_count_location,
    std::unordered_map<LightUniforms, std::string> uniform_map)
    : uniform_map_(uniform_map),
      struct_name_(struct_name),
      light_count_location_(light_count_location),
      max_size_(max_point_light) {}

PointLightContainer::iterator PointLightController::MakeAndBind(
    const AmbientConfig& ambient_config,
    const PointConfig& point_config,
    float diffuse_intensity,
    const Shader& shader_) {
  if (point_light_container_.size() >= max_size_) {
    LOG(INFO) << "Reached maximal number of point lights (" << max_size_ << ")";
    return point_light_container_.end();
  }
  point_light_container_.emplace_back(std::make_unique<PointLight>(
      ambient_config, point_config, diffuse_intensity));
  point_light_container_.back()->BindUniforms(
      shader_, point_light_container_.size() - 1, struct_name_, uniform_map_);
  return point_light_container_.end() - 1;
}

void PointLightController::ForAll(
    std::function<void(PointLightPtr&)> function) {
  for (auto& light : point_light_container_) {
    function(light);
  }
}

void PointLightController::Use() {
  glUniform1i(light_count_location_, point_light_container_.size());
  for (auto& light : point_light_container_) {
    light->Use();
  }
}

const PointLightContainer& PointLightController::GetContainer() const {
  return point_light_container_;
}

PointLightPtr& PointLightController::operator[](std::size_t element) {
  return point_light_container_[element];
}

const PointLightPtr& PointLightController::operator[](
    std::size_t element) const {
  return point_light_container_[element];
}
