#include <MEP-3D/arcball_camera.hpp>
#include <MEP-3D/light.hpp>
#include <MEP-3D/perspective_camera.hpp>
#include <MEP-3D/point_light.hpp>
#include <MEP-3D/spot_light.hpp>

PointConfig PointConfig::Create() {
  return PointConfig{{0.0f, 0.0f, 0.0f}, 0.1f, 0.1f, 1.0f};
}

SpotConfig SpotConfig::Create() {
  return SpotConfig{{0.0f, -1.0f, 0.0f}, 90};
}

AmbientConfig AmbientConfig::Create() {
  return AmbientConfig{White, 0.1f};
}

DiffuseConfig DiffuseConfig::Create() {
  return DiffuseConfig{Vec3f{0.0f, 0.0f, 0.0f}, 1.0f};
}

std::unique_ptr<PointLight> PointLight::Create() {
  return std::make_unique<PointLight>(AmbientConfig::Create(),
                                      PointConfig::Create(), 1.0f);
}

std::unique_ptr<SpotLight> SpotLight::Create() {
  return std::make_unique<SpotLight>(AmbientConfig::Create(),
                                     PointConfig::Create(), 1.0f,
                                     SpotConfig::Create());
}

PerspectiveCameraConfig PerspectiveCameraConfig::Create() {
  return {glm::vec3(10.0f, 10.0f, -5.0f),
          glm::vec3(0.0f, 1.0f, 0.0f),
          145.0f,
          -45,
          5.0f,
          0.5f};
}

ArcballCameraConfig ArcballCameraConfig::Create() {
  return {glm::vec3(10.0f, 10.0f, -5.0f),
          glm::vec3(0.0f, 1.0f, 0.0f),
          glm::vec3(0.0f, 0.0f, 0.0f),
          {1280, 720},
          1.5f};
}
