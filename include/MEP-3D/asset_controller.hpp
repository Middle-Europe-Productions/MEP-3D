#ifndef ASSET_CONTROLLER_HPP
#define ASSET_CONTROLLER_HPP

#include <MEP-3D/asset_observer.hpp>
#include <MEP-3D/material.hpp>
#include <MEP-3D/shader.hpp>
#include <MEP-3D/texture_base.hpp>

namespace mep {
class AssetController : private AssetObserver {
 public:
  AssetController();
  void Bind(TextureBase* object);
  void Bind(Shader* object);
  void Bind(Material* object);
  template <typename What>
  What* Get() const;
  void OnDelete(Identity& caller_identity);
  void RemoveAssets();
  std::string ToString() const;
  ~AssetController();

 private:
  TextureBase* texture_;
  Shader* shader_;
  Material* material_;
};
}  // namespace mep

#include <MEP-3D/asset_controller.inl>

#endif