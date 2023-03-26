#ifndef VOLUME_RENDERER_HPP
#define VOLUME_RENDERER_HPP

#include <MEP-3D/mep-3d.hpp>

using namespace mep;

class VolumeRenderer final : public Scene {
 public:
  VolumeRenderer();
  void OnAttach() override;
  void OnDetach() override;
  void OnUpdate(float time_delta) override;
  void OnDraw(RenderTarget& render_target) override;

  virtual void OnReceive(const IdentityView& id,
                         const nlohmann::json& message) override;

 private:
  std::unique_ptr<PerspectiveView> view_;
  mep::TextureView transfer_function_texture_;
};

#endif