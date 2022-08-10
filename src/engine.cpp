#include <MEP-3D/engine.hpp>

std::string EngineMonitor::ToDebugJSON() {
  std::string layer_json = "\"layer_data\": [\n";
  for (auto& layer : frame_id_.layer_data_) {
    layer_json += layer.first.ToString() + ",\n";
    layer_json += "{\n\"layer_update_time_ms\":" +
                  std::to_string(layer.second.layer_update_time_ms) + ",\n" +
                  "\"layer_draw_time_ms\":" +
                  std::to_string(layer.second.layer_draw_time_ms) + ",\n" +
                  "\"frame_time\":" + std::to_string(layer.second.frame_time) +
                  "\n" + "}";
  }
  layer_json += "],\n\"fps\":" + std::to_string(fps);
  return layer_json;
}

Engine::Engine() : window_(nullptr) {
  LOG(INFO) << "Engine created: " << ToString();
}

void Engine::AttachWindow(WindowPtr window) {
  LOG(INFO) << "Window attached to engine, id: " << ToString();
  window_ = std::move(window);
}

const WindowPtr& Engine::GetWindow() const {
  return window_;
}

WindowPtr& Engine::GetWindow() {
  return window_;
}

void Engine::Run() {
  LOG(INFO) << "Engine initialized, id: " << ToString();
  auto time = TimeDelta::GetInstance();
  while (window_ && window_->IsOpen()) {
    for (auto& layer : layers_) {
      glfwPollEvents();
      layer->OnUpdate(10);
      window_->Clear(White);
      layer->OnDraw(*window_);
      window_->FinishLoop();
    }
  }
}

void Engine::AttachLayer(std::unique_ptr<Layer> obs) {
  LOG(INFO) << __FUNCTION__;
  layers_.emplace_back(std::move(obs));
  layers_.back()->RegisterEngine(std::move(shared_from_this()));
  layers_.back()->OnAttach();
}

std::unique_ptr<Layer> Engine::Detachlayer(const Identity& id) {
  for (std::size_t i = 0; i < layers_.size(); ++i) {
    if (*layers_[i].get() == id) {
      auto ele = std::move(layers_[i]);
      layers_.erase(layers_.begin() + i);
      ele->OnDetach();
      return ele;
    }
  }
  return nullptr;
}

const EngineMonitor& Engine::GetEngineMonitor() const {
  return engine_monitor_;
}

bool Engine::operator==(const Identity& id) {
  return *this == id;
}
