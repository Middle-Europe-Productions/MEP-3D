#ifndef LIGHT_HPP
#define LIGHT_HPP

#include<MEP-3D/color.hpp>
#include<MEP-3D/vector.hpp>
#include<MEP-3D/asset_observer.hpp>
#include<MEP-3D/asset_observer.hpp>
#include<MEP-3D/observer_list.hpp>

#include<GL/glew.h>

#include<optional>
#include<unordered_map>

struct AmbientConfig {
	Color color;
	float intensity;
};

struct DiffuseConfig {
	std::optional<Vec3f> direction;
	float intensity;
};

enum LightUniforms {
	AmbientIntensity = 0,
	AmbientColor = 1,
	DiffuseDirection,
	DiffuseIntensity
};

std::string ToString(LightUniforms uniform_type);

class Light: public Identity, public ObserverList<AssetObserver> {
public:
	Light();
	Light(std::optional<AmbientConfig> ambient_config, std::optional<DiffuseConfig> diffuse_config);
	virtual void Use();
	void BindUniform(GLint uniform_location, LightUniforms type);
	void SetAmbientConfig(AmbientConfig ambient_config);
	void SetDiffuseConfig(DiffuseConfig diffuse_config);
	std::string ToString() const override;
	virtual ~Light() = default;

private:
	std::unordered_map<LightUniforms, GLfloat> uniform_cache_;
	std::optional<AmbientConfig> ambient_config_;
	std::optional<DiffuseConfig> diffuse_config_;
};

#endif
