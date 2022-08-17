#include <MEP-3D/common_names.hpp>
#include <MEP-3D/material.hpp>

std::string ToString(MaterialUniform material_uniform) {
  switch (material_uniform) {
    case MaterialUniform::SpecularIntensity:
      return "SpecularIntensity";
    case MaterialUniform::Shininess:
      return "Shininess";
  }
  return "";
}

Material::Material(std::optional<MaterialConfig> material_config)
    : Asset(kMaterial), material_config_(material_config) {}

void Material::SetMaterialConfig(const MaterialConfig& material_config) {
  if (material_config_.has_value()) {
    LOG(INFO) << "Overriding existing config!";
  }
  material_config_ = material_config;
}

void Material::Use() {
  if (!material_config_.has_value())
    return;
  if (Exists(MaterialUniform::SpecularIntensity))
    glUniform1f(GetUniform(MaterialUniform::SpecularIntensity),
                material_config_->specular_intensity);
  if (Exists(MaterialUniform::Shininess))
    glUniform1f(GetUniform(MaterialUniform::Shininess),
                material_config_->shininess);
}

void Material::Stop() {
  if (Exists(MaterialUniform::SpecularIntensity))
    glUniform1f(GetUniform(MaterialUniform::SpecularIntensity), 0.0f);
  if (Exists(MaterialUniform::Shininess))
    glUniform1f(GetUniform(MaterialUniform::Shininess), 0.0f);
}

Material::~Material() {
  LOG(INFO) << __FUNCTION__;
}
