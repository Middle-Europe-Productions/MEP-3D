#ifndef POINT_LIGHT_CONTROLLER_HPP
#define POINT_LIGHT_CONTROLLER_HPP

#include <MEP-3D/asset_controller.hpp>
#include <MEP-3D/point_light.hpp>
#include <MEP-3D/shader.hpp>
#include <MEP-3D/spot_light.hpp>

#include <unordered_map>

template <typename LightPtr>
class LightController : public AssetController {
 public:
  LightController(unsigned int max_point_light,
                  const std::string& struct_name,
                  GLint light_count_location,
                  std::unordered_map<LightUniforms, std::string> uniform_map);
  typename std::vector<LightPtr>::iterator MakeAndBind(LightPtr light_ptr);
  bool IsValid(const typename std::vector<LightPtr>::iterator& iter) const;
  void Remove(const Identity& id);
  void ForAll(std::function<void(LightPtr&)> function);
  void Use();
  const std::vector<LightPtr>& GetContainer() const;
  LightPtr& operator[](std::size_t element);
  const LightPtr& operator[](std::size_t element) const;

 private:
  std::vector<LightPtr> point_light_container_;
  std::unordered_map<LightUniforms, std::string> uniform_map_;
  std::string struct_name_;
  GLint light_count_location_;
  const unsigned int max_size_;
};

using PointLightPtr = std::unique_ptr<PointLight>;
using PointLightController = LightController<std::unique_ptr<PointLight>>;

using SpotLightPtr = std::unique_ptr<SpotLight>;
using SpotLightController = LightController<std::unique_ptr<SpotLight>>;

#include <MEP-3D/light_controller.inl>

#endif