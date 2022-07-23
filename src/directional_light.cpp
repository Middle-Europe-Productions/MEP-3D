#include <MEP-3D/directional_light.hpp>

DirectionalLight::DirectionalLight(const AmbientConfig& ambient_config,
                                   const DiffuseConfig& diffuse_config)
    : Light(ambient_config, diffuse_config) {}
