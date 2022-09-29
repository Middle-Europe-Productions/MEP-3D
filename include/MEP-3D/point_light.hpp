#ifndef POINT_LIGHT_HPP
#define POINT_LIGHT_HPP

#include <MEP-3D/light.hpp>
#include <MEP-3D/vector.hpp>

#include <memory>

struct PointConfig {
  Vec3f position;
  // https://wiki.ogre3d.org/tiki-index.php?page=-Point+Light+Attenuation
  float constant;
  float linear;
  float quadratic;
  static PointConfig Create();
};

class PointLight : public Light {
 public:
  PointLight(const AmbientConfig& ambient_config,
             const PointConfig& point_config,
             float diffuse_intensity);
  void Use() override;
  AmbientConfig& GetAmbientConfigRef();
  PointConfig& GetPointConfigRef();
  DiffuseConfig& GetDiffuseConfigRef();
  float& GetDiffuseIntensityRef();
  static std::unique_ptr<PointLight> Create();

 protected:
  PointLight(const AmbientConfig& ambient_config,
             const PointConfig& point_config,
             float diffuse_intensity,
             const char* super_class_name);

 private:
  PointConfig point_config_;
};

#endif
