#ifndef LIGHT_HPP
#define LIGHT_HPP

#include<MEP-3D/color.hpp>
#include<MEP-3D/vector.hpp>
#include<MEP-3D/asset_observer.hpp>
#include<MEP-3D/asset_observer.hpp>
#include<MEP-3D/observer_list.hpp>

#include<optional>

struct AmbientConfig {
	Color color;
	float intensity;
};

struct DiffuseConfig {
	Vec3f direction;
	float intensity;
};

class Light: public ObserverList<AssetObserver> {
public:
	Light(): ambient_config_(std::nullopt), diffuse_config_(std::nullopt) {} 
	Light(std::optional<AmbientConfig> ambient_config, std::optional<DiffuseConfig> diffuse_config): 
					ambient_config_(ambient_config), diffuse_config_(diffuse_config) {}

	bool UseAmbient(GLfloat ambient_intensity_loc, GLfloat ambient_color_loc) const {
		if (!ambient_config_.has_value())
			return false;
		glUniform3f(ambient_color_loc, 
					ambient_config_.value().color.Rf(),
					ambient_config_.value().color.Gf(),
					ambient_config_.value().color.Bf());
		glUniform1f(ambient_intensity_loc, ambient_config_.value().intensity);
	}
	bool UseDiffuse(GLfloat diffuse_intensity_loc, GLfloat diffuse_direction_loc) const {
		if (!diffuse_config_.has_value())
			return false;
		glUniform3f(diffuse_direction_loc, 
					diffuse_config_.value().direction.x_,
					diffuse_config_.value().direction.y_,
					diffuse_config_.value().direction.z_);
		glUniform1f(diffuse_intensity_loc, diffuse_config_.value().intensity);
	}
	void SetAmbientConfig(AmbientConfig ambient_config) {
		ambient_config_ = ambient_config;
	}
	void SetDiffuseConfig(DiffuseConfig diffuse_config) {
		diffuse_config_ = diffuse_config;
	}

private:
	std::optional<AmbientConfig> ambient_config_;
	std::optional<DiffuseConfig> diffuse_config_;
};

#endif
