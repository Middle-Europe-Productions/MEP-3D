#include <MEP-3D/common_names.hpp>
#include <MEP-3D/spot_light.hpp>

SpotLight::SpotLight(const AmbientConfig& ambient_config,
                     const PointConfig& point_config,
                     float diffuse_intensity,
                     const SpotConfig& spot_config)
    : PointLight(ambient_config, point_config, diffuse_intensity, kSpotLight),
      spot_config_(spot_config) {
  CalcuateEdge();
}

void SpotLight::Use() {
  PointLight::Use();
  if (last_edge_ != spot_config_.edge_deg)
    CalcuateEdge();
  if (Exists(LightUniforms::Direction))
    glUniform3f(GetUniform(LightUniforms::Direction), spot_config_.direction.x_,
                spot_config_.direction.y_, spot_config_.direction.z_);
  if (Exists(LightUniforms::Edge))
    glUniform1f(GetUniform(LightUniforms::Edge), processed_edge_);
}

void SpotLight::CalcuateEdge() {
  processed_edge_ = cosf(glm::radians(spot_config_.edge_deg));
  last_edge_ = spot_config_.edge_deg;
  VLOG(3) << "processed_edge_: " << processed_edge_
          << ", last_edge_:" << last_edge_
          << ", edge_deg:" << spot_config_.edge_deg;
}

SpotConfig& SpotLight::GetSpotConfigRef() {
  return spot_config_;
}