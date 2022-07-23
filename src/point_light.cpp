#include<MEP-3D/point_light.hpp>

PointLight::PointLight(const AmbientConfig& ambient_config, const PointConfig& point_config, float diffuse_intensity) :
                    Light(ambient_config, DiffuseConfig{std::nullopt, diffuse_intensity}), point_config_(point_config) {

}

void PointLight::Use() {
    Light::Use();
    if (Exists(LightUniforms::Position))
        glUniform3f(GetUniform(LightUniforms::Position), 
				point_config_.position.x_,
				point_config_.position.y_,
				point_config_.position.z_);
    if (Exists(LightUniforms::Constant))
        glUniform1f(GetUniform(LightUniforms::Constant), point_config_.constant);
    else
        glUniform1f(GetUniform(LightUniforms::Constant), 0.0f);

    if (Exists(LightUniforms::Linear))
        glUniform1f(GetUniform(LightUniforms::Linear), point_config_.linear);
    else
        glUniform1f(GetUniform(LightUniforms::Linear), 0.0f);

    if (Exists(LightUniforms::Quadratic))
        glUniform1f(GetUniform(LightUniforms::Quadratic), point_config_.quadratic);
    else
        glUniform1f(GetUniform(LightUniforms::Quadratic), 0.0f);
}