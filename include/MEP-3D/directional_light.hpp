#ifndef DIRECTIONAL_LIGHT_HPP
#define DIRECTIONAL_LIGHT_HPP

#include <MEP-3D/light.hpp>

class DirectionalLight : public Light {
 public:
  DirectionalLight(const AmbientConfig& ambient_config,
                   const DiffuseConfig& diffuse_config);
};

#endif