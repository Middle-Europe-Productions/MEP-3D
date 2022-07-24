#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <MEP-3D/asset.hpp>
#include <MEP-3D/asset_observer.hpp>
#include <MEP-3D/uniform_container.hpp>

#include <optional>

#include <GL/glew.h>

struct MaterialConfig {
  float specular_intensity;
  float shininess;
};

enum MaterialUniform { SpecularIntensity, Shininess };

std::string ToString(MaterialUniform material_uniform);

class Material : public Asset, public UniformContainer<MaterialUniform> {
 public:
  Material(std::optional<MaterialConfig> material_config);
  void SetMaterialConfig(const MaterialConfig& material_config);
  void Use() override;
  void Stop() override;
  virtual ~Material();

 private:
  std::optional<MaterialConfig> material_config_;
};

#endif