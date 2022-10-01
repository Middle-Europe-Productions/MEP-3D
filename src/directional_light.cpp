#include <MEP-3D/common_names.hpp>
#include <MEP-3D/directional_light.hpp>

DirectionalLight::DirectionalLight(const AmbientConfig& ambient_config,
                                   const DiffuseConfig& diffuse_config)
    : Light(ambient_config, diffuse_config, kDirectionalLight) {
  VLOG(1) << __FUNCTION__ << ", " << ToString();
}

AmbientConfig& DirectionalLight::GetAmbientConfigRef() {
  assert(ambient_config_.has_value());
  return ambient_config_.value();
}
DiffuseConfig& DirectionalLight::GetDiffuseConfigRef() {
  assert(diffuse_config_.has_value());
  return diffuse_config_.value();
}
