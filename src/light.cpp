#include<MEP-3D/light.hpp>

std::string ToString(LightUniforms uniform_type) {
    switch(uniform_type) {
        case LightUniforms::AmbientIntensity:
            return "AmbientIntensity";
        case LightUniforms::AmbientColor:
            return "AmbientColor";
        case LightUniforms::DiffuseDirection:
            return "DiffuseDirection";
        case LightUniforms::DiffuseIntensity:
            return "DiffuseIntensity";
    }
    return "Unknown";
}

Light::Light(): Identity(__FUNCTION__), ambient_config_(std::nullopt), diffuse_config_(std::nullopt) {}

Light::Light(std::optional<AmbientConfig> ambient_config, std::optional<DiffuseConfig> diffuse_config): 
				Identity(__FUNCTION__), ambient_config_(ambient_config), diffuse_config_(diffuse_config) {}

void Light::Use() {
	if (!ambient_config_.has_value())
		return;
	if (uniform_cache_.find(LightUniforms::AmbientColor) != uniform_cache_.end()) {
		glUniform3f(uniform_cache_[LightUniforms::AmbientColor], 
					ambient_config_.value().color.Rf(),
					ambient_config_.value().color.Gf(),
					ambient_config_.value().color.Bf());
	}
	if (uniform_cache_.find(LightUniforms::AmbientIntensity) != uniform_cache_.end()) {
		glUniform1f(uniform_cache_[LightUniforms::AmbientIntensity], ambient_config_.value().intensity);
	}
	if (!diffuse_config_.has_value())
		return;
	if (uniform_cache_.find(LightUniforms::DiffuseDirection) != uniform_cache_.end()) {
		if (diffuse_config_.value().direction.has_value()) {
			glUniform3f(uniform_cache_[LightUniforms::DiffuseDirection], 
				diffuse_config_.value().direction.value().x_,
				diffuse_config_.value().direction.value().y_,
				diffuse_config_.value().direction.value().z_);
		}
	}
	if (uniform_cache_.find(LightUniforms::DiffuseIntensity) != uniform_cache_.end()) {
		glUniform1f(uniform_cache_[LightUniforms::DiffuseIntensity], ambient_config_.value().intensity);
	}
}

void Light::BindUniform(GLint uniform_location, LightUniforms type) {
	if (uniform_location < 0) {
		LOG(ERROR) << "Invalid uniform!";
		return;
	}
	if (uniform_cache_.find(type) != uniform_cache_.end()) {
		LOG(INFO) << "Uniform already bound overriding!";
	}
	uniform_cache_[type] = uniform_location;
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
    std::string output = "Light: {\n" + Identity::ToString() + ",\nUniformCache: {\n";
    for (auto it = uniform_cache_.begin(); it != uniform_cache_.end();) {
        output += ::ToString(it->first) + ": " + std::to_string(it->second);
        if (++it != uniform_cache_.end())
            output += ",\n";
    }
    output += "}\n}";
    return output;
}
