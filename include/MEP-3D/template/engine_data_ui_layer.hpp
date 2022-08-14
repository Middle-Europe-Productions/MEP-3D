#ifndef ENGINE_DATA_UI_LAYER_HPP
#define ENGINE_DATA_UI_LAYER_HPP

#include <MEP-3D/layer.hpp>

class EngineDataUILayer final : public Layer {
 public:
  EngineDataUILayer();
  void OnAttach() override;
  void OnDetach() override;
  void OnUpdate(float time_delta);
  void OnDraw(RenderTarget& render_target);
  ~EngineDataUILayer() = default;
};

#endif