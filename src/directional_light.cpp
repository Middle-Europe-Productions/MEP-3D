#include <MEP-3D/directional_light.hpp>

DirectionalLight::DirectionalLight(const AmbientConfig& ambient_config,
                                   const DiffuseConfig& diffuse_config)
    : Light(ambient_config, diffuse_config, __FUNCTION__) {}

AmbientConfig& DirectionalLight::GetAmbientConfigRef() {
  assert(ambient_config_.has_value());
  return ambient_config_.value();
}
DiffuseConfig& DirectionalLight::GetDiffuseConfigRef() {
  assert(diffuse_config_.has_value());
  return diffuse_config_.value();
}
