#ifndef ENGINE_DATA_UI_LAYER_HPP
#define ENGINE_DATA_UI_LAYER_HPP

#include <MEP-3D/layer.hpp>
#include <MEP-3D/window_observer.hpp>

class EngineDataUILayer : public Layer, public WindowObserver {
 public:
  static std::unique_ptr<EngineDataUILayer> Create();
  ~EngineDataUILayer() = default;

 protected:
  EngineDataUILayer();
};

#endif