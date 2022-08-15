#include <MEP-3D/layer.hpp>

Layer::Layer()
    : Identity(__FUNCTION__), layer_name_("layer_" + std::to_string(GetId())) {
  LOG(INFO) << __FUNCTION__ << ", id: " << ToString();
}

Layer::Layer(const std::string& layer_name)
    : Identity(__FUNCTION__), layer_name_(layer_name) {
  LOG(INFO) << __FUNCTION__ << ", id: " << ToString();
}

const std::string& Layer::GetName() const {
  return layer_name_;
}

std::shared_ptr<Engine> Layer::GetEngine() const {
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

bool Layer::ShouldIgnoreLayer() const {
  return false;
}