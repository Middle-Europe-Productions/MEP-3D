#ifndef SPOT_LIGHT_HPP
#define SPOT_LIGHT_HPP

#include <MEP-3D/point_light.hpp>

struct SpotConfig {
  Vec3f direction;
  float edge_deg;
};

class SpotLight : public PointLight {
 public:
  SpotLight(const AmbientConfig& ambient_config,
            const PointConfig& point_config,
            float diffuse_intensity,
            const SpotConfig& spot_config);
  void Use() override;
  void CalcuateEdge();
  SpotConfig& GetSpotConfigRef();

 private:
  SpotConfig spot_config_;
  GLfloat processed_edge_;
};

#endif