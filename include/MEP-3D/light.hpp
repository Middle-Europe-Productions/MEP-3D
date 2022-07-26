#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <MEP-3D/asset.hpp>
#include <MEP-3D/asset_observer.hpp>
#include <MEP-3D/color.hpp>
#include <MEP-3D/observer_list.hpp>
#include <MEP-3D/uniform_container.hpp>
#include <MEP-3D/vector.hpp>

#include <GL/glew.h>

#include <optional>
#include <unordered_map>

struct AmbientConfig {
  Color color;
  float intensity;
};

struct DiffuseConfig {
  std::optional<Vec3f> direction;
  float intensity;
};

enum class LightUniforms : unsigned int {
  AmbientIntensity = 0,
  AmbientColor = 1,
  DiffuseDirection,
  DiffuseIntensity,
  Position,
  Constant,
  Linear,
  Quadratic,
  Direction,
  Edge,
};

std::string ToString(LightUniforms uniform_type);

class Light : public Asset, public UniformContainer<LightUniforms> {
 public:
  Light();
  Light(std::optional<AmbientConfig> ambient_config,
        std::optional<DiffuseConfig> diffuse_config);
  void Use() override;
  void Stop() override;
  std::string ToString() const override;
  virtual ~Light();

 protected:
  Light(std::optional<AmbientConfig> ambient_config,
        std::optional<DiffuseConfig> diffuse_config,
        const char* super_class_name);
  std::optional<AmbientConfig> ambient_config_;
  std::optional<DiffuseConfig> diffuse_config_;
};

#endif
