#include <MEP-3D/layer.hpp>

Layer::Layer() : Identity(__FUNCTION__) {
  LOG(INFO) << __FUNCTION__;
}

std::shared_ptr<Engine> Layer::GetEngine() const {
  LOG(INFO) << engine_.use_count();
  return engine_;
}
Layer::~Layer() {
  LOG(INFO) << __FUNCTION__;
}

void Layer::RegisterEngine(const std::shared_ptr<Engine>& engine) {
  LOG(INFO) << __FUNCTION__;
  engine_ = engine;
  LOG(INFO) << engine.use_count();
}
