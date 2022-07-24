#ifndef POINT_LIGHT_HPP
#define POINT_LIGHT_HPP

#include <MEP-3D/light.hpp>
#include <MEP-3D/vector.hpp>

struct PointConfig {
  Vec3f position;
  // https://wiki.ogre3d.org/tiki-index.php?page=-Point+Light+Attenuation
  float constant;
  float linear;
  float quadratic;
};

class PointLight : public Light {
 public:
  PointLight(const AmbientConfig& ambient_config,
             const PointConfig& point_config,
             float diffuse_intensity);
  void Use() override;

 private:
  PointConfig point_config_;
};

#endif
