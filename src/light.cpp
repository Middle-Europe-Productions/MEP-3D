#include <MEP-3D/light.hpp>

std::string ToString(LightUniforms uniform_type) {
  switch (uniform_type) {
    case LightUniforms::AmbientIntensity:
      return "AmbientIntensity";
    case LightUniforms::AmbientColor:
      return "AmbientColor";
    case LightUniforms::DiffuseDirection:
      return "DiffuseDirection";
    case LightUniforms::DiffuseIntensity:
      return "DiffuseIntensity";
    case LightUniforms::Position:
      return "Position";
    case LightUniforms::Constant:
      return "Constant";
    case LightUniforms::Linear:
      return "Linear";
    case LightUniforms::Quadratic:
      return "Quadratic";
  }
  return "Unknown";
}

Light::Light()
    : Asset(__FUNCTION__),
      ambient_config_(std::nullopt),
      diffuse_config_(std::nullopt) {}

Light::Light(std::optional<AmbientConfig> ambient_config,
             std::optional<DiffuseConfig> diffuse_config)
    : Asset(__FUNCTION__),
      ambient_config_(ambient_config),
      diffuse_config_(diffuse_config) {}

void Light::Use() {
  if (!ambient_config_.has_value())
    return;
  if (Exists(LightUniforms::AmbientColor)) {
    glUniform3f(GetUniform(LightUniforms::AmbientColor),
                ambient_config_.value().color.Rf(),
                ambient_config_.value().color.Gf(),
                ambient_config_.value().color.Bf());
  }
  if (Exists(LightUniforms::AmbientIntensity)) {
    glUniform1f(GetUniform(LightUniforms::AmbientIntensity),
                ambient_config_.value().intensity);
  }
  if (!diffuse_config_.has_value())
    return;
  if (Exists(LightUniforms::DiffuseDirection)) {
    if (diffuse_config_.value().direction.has_value()) {
      glUniform3f(GetUniform(LightUniforms::DiffuseDirection),
                  diffuse_config_.value().direction.value().x_,
                  diffuse_config_.value().direction.value().y_,
                  diffuse_config_.value().direction.value().z_);
    }
  }
  if (Exists(LightUniforms::DiffuseIntensity)) {
    glUniform1f(GetUniform(LightUniforms::DiffuseIntensity),
                diffuse_config_.value().intensity);
  }
}

void Light::Stop() {
  if (Exists(LightUniforms::AmbientIntensity)) {
    glUniform1f(GetUniform(LightUniforms::AmbientIntensity), 0.0f);
  }
  if (Exists(LightUniforms::DiffuseIntensity)) {
    glUniform1f(GetUniform(LightUniforms::DiffuseIntensity), 0.0f);
  }
}

void Light::SetAmbientConfig(AmbientConfig ambient_config) {
  if (ambient_config_.has_value())
    LOG(INFO) << "Overraiding config: AmbientConfig";
  ambient_config_ = ambient_config;
}

void Light::SetDiffuseConfig(DiffuseConfig diffuse_config) {
  if (diffuse_config_.has_value())
    LOG(INFO) << "Overriding config: DiffuseConfig";
  diffuse_config_ = diffuse_config;
}

std::string Light::ToString() const {
  std::string output =
      "Light: {\n" + Identity::ToString() + ",\nUniformCache: {\n";
  for (auto it = uniform_cache_.begin(); it != uniform_cache_.end();) {
    output += ::ToString(it->first) + ": " + std::to_string(it->second);
    if (++it != uniform_cache_.end())
      output += ",\n";
  }
  output += "}\n}";
  return output;
}

Light::~Light() {
  LOG(INFO) << __FUNCTION__;
}
