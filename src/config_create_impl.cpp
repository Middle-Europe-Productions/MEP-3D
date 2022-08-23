#include <MEP-3D/light.hpp>
#include <MEP-3D/point_light.hpp>
#include <MEP-3D/spot_light.hpp>

PointConfig PointConfig::Create() {
  return PointConfig{{0.0, 0.0, 0.0}, 0.1, 0.1, 1.0};
}

SpotConfig SpotConfig::Create() {
  return SpotConfig{{0.0, -1.0, 0.0}, 90};
}

AmbientConfig AmbientConfig::Create() {
  return AmbientConfig{White, 0.1f};
}

DiffuseConfig DiffuseConfig::Create() {
  return DiffuseConfig{Vec3f{0.0, 0.0, 0.0}, 1.0};
}

std::unique_ptr<PointLight> PointLight::Create() {
  return std::make_unique<PointLight>(AmbientConfig::Create(),
                                      PointConfig::Create(), 1.0);
}

std::unique_ptr<SpotLight> SpotLight::Create() {
  return std::make_unique<SpotLight>(AmbientConfig::Create(),
                                     PointConfig::Create(), 1.0,
                                     SpotConfig::Create());
}