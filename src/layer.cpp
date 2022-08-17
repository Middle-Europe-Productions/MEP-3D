#include <MEP-3D/common_names.hpp>
#include <MEP-3D/layer.hpp>

Layer::Layer() : Identity(kLayer) {
  LOG(INFO) << kLayer << ", id: " << ToString();
}

Layer::Layer(const std::string& layer_name)
    : Identity(kLayer, layer_name.c_str()) {
  LOG(INFO) << kLayer << ", id: " << ToString();
}

std::shared_ptr<Engine> Layer::GetEngine() const {
  return engine_;
}
Layer::~Layer() {
  LOG(INFO) << __FUNCTION__ << ", " << ToString();
}

void Layer::RegisterEngine(const std::shared_ptr<Engine>& engine) {
  LOG(INFO) << __FUNCTION__ << ", Layer: " << ToString();
  engine_ = engine;
}

bool Layer::ShouldIgnoreLayer() const {
  return false;
}

void Layer::UnregisterEngine() {
  LOG(INFO) << __FUNCTION__ << ", Layer: " << ToString();
  engine_ = nullptr;
}
