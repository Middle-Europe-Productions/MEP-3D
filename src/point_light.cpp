
#include <MEP-3D/common_names.hpp>
#include <MEP-3D/point_light.hpp>

namespace mep {
PointLight::PointLight(const AmbientConfig& ambient_config,
                       const PointConfig& point_config,
                       float diffuse_intensity)
    : Light(ambient_config,
            DiffuseConfig{std::nullopt, diffuse_intensity},
            kPointLight),
      point_config_(point_config) {
  VLOG(1) << __FUNCTION__ << ", " << ToString();
}

PointLight::PointLight(const AmbientConfig& ambient_config,
                       const PointConfig& point_config,
                       float diffuse_intensity,
                       const char* super_class_name)
    : Light(ambient_config,
            DiffuseConfig{std::nullopt, diffuse_intensity},
            super_class_name),
      point_config_(point_config) {
  VLOG(1) << __FUNCTION__ << ", " << ToString();
}

void PointLight::Use() {
  Light::Use();
  if (Exists(LightUniforms::Position))
    glUniform3f(GetUniform(LightUniforms::Position), point_config_.position.x_,
                point_config_.position.y_, point_config_.position.z_);
  if (Exists(LightUniforms::Constant))
    glUniform1f(GetUniform(LightUniforms::Constant), point_config_.constant);

  if (Exists(LightUniforms::Linear))
    glUniform1f(GetUniform(LightUniforms::Linear), point_config_.linear);

  if (Exists(LightUniforms::Quadratic))
    glUniform1f(GetUniform(LightUniforms::Quadratic), point_config_.quadratic);
}

AmbientConfig& PointLight::GetAmbientConfigRef() {
  assert(ambient_config_.has_value());
  return ambient_config_.value();
}
PointConfig& PointLight::GetPointConfigRef() {
  return point_config_;
}

DiffuseConfig& PointLight::GetDiffuseConfigRef() {
  assert(diffuse_config_.has_value());
  return diffuse_config_.value();
}

float& PointLight::GetDiffuseIntensityRef() {
  assert(diffuse_config_.has_value());
  return diffuse_config_.value().intensity;
}
}  // namespace mep
