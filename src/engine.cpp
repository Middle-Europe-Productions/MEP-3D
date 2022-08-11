#include <MEP-3D/engine.hpp>

std::string EngineMonitor::ToDebugJSON() {
  std::string layer_json = "\"engine_monitor\": {\"layer_data\": [\n";
  for (auto& layer : frame_data.layer_data) {
    layer_json += layer.first.ToString() + ", ";
    layer_json += "\n\"layer_update_time_ms\":" +
                  std::to_string(layer.second.layer_update_time_ms) + ",\n" +
                  "\"layer_draw_time_ms\":" +
                  std::to_string(layer.second.layer_draw_time_ms) + "\n" + "}";
  }
  layer_json += "],\n\"frame_time\":" + std::to_string(frame_data.frame_time) +
                ",\n" + "\"fps\":" + std::to_string(fps) + "\n}";
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
  double time_controller = 0.0;
  unsigned int frames = 0;
  while (window_ && window_->IsOpen()) {
    engine_monitor_.frame_data.frame_time = time->GetTickCount();
    glfwPollEvents();
    auto time_delta = time->GetTimeDelta();
    time_controller += time_delta;
    frames++;
    window_->Clear(White);

    engine_monitor_.frame_data.layer_data.clear();
    for (auto& layer : layers_) {
      EngineMonitor::LayerData layer_data;

      layer_data.layer_update_time_ms = time->GetTickCount();
      layer->OnUpdate(time_delta);
      layer_data.layer_update_time_ms =
          time->GetTickCount() - layer_data.layer_update_time_ms;

      layer_data.layer_draw_time_ms = time->GetTickCount();
      layer->OnDraw(*window_);
      layer_data.layer_draw_time_ms =
          time->GetTickCount() - layer_data.layer_draw_time_ms;

      engine_monitor_.frame_data.layer_data.push_back({*layer, layer_data});
    }
    engine_monitor_.frame_data.frame_time =
        time->GetTickCount() - engine_monitor_.frame_data.frame_time;
    if (time_controller > 1.0) {
      engine_monitor_.fps = frames;
      LOG(INFO) << engine_monitor_.ToDebugJSON();
      time_controller = 0.0;
      frames = 0;
    }
    window_->FinishLoop();
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
