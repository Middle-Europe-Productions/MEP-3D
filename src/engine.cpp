#include <MEP-3D/engine.hpp>

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
  unsigned int last_fps = 0;
  while (window_ && window_->IsOpen()) {
    window_->StartLoop();
    EngineMonitorData engine_monitor;
    engine_monitor.frame_data.frame_time = time->GetTickCount();
    auto time_delta = time->GetTimeDelta();
    time_controller += time_delta;
    frames++;
    window_->Clear(White);

    for (auto& layer : layers_) {
      EngineMonitorData::LayerData layer_data;

      layer_data.layer_update_time_ms = time->GetTickCount();
      layer->OnUpdate(time_delta);
      layer_data.layer_update_time_ms =
          (time->GetTickCount() - layer_data.layer_update_time_ms) * 1000.0f;

      layer_data.layer_draw_time_ms = time->GetTickCount();
      layer->OnDraw(*window_);
      layer_data.layer_draw_time_ms =
          (time->GetTickCount() - layer_data.layer_draw_time_ms) * 1000.0f;
      layer_data.identity = *layer;
      layer_data.layer_name = layer->layer_name_;
      engine_monitor.frame_data.layer_data.push_back(layer_data);
    }

    for (auto& ele : custom_layers_) {
      EngineMonitorData::Structure custom_layer_data;
      custom_layer_data.identity = *ele;
      custom_layer_data.structure_name = ele->structure_name;
      custom_layer_data.total_update_time = time->GetTickCount();
      ele->before_run();
      for (auto& layer : ele->layer_array) {
        EngineMonitorData::LayerData layer_data;

        layer_data.layer_update_time_ms = time->GetTickCount();
        layer->OnUpdate(time_delta);
        layer_data.layer_update_time_ms =
            (time->GetTickCount() - layer_data.layer_update_time_ms) * 1000.0f;

        layer_data.layer_draw_time_ms = time->GetTickCount();
        layer->OnDraw(*window_);
        layer_data.layer_draw_time_ms =
            (time->GetTickCount() - layer_data.layer_draw_time_ms) * 1000.0f;
        layer_data.identity = *layer;
        layer_data.layer_name = layer->layer_name_;
        custom_layer_data.layer_array.push_back(layer_data);
      }
      ele->after_run();
      custom_layer_data.total_update_time =
          time->GetTickCount() - custom_layer_data.total_update_time;
      engine_monitor.frame_data.structure_data.push_back(custom_layer_data);
    }

    window_->FinishLoop();
    engine_monitor.frame_data.frame_time =
        (time->GetTickCount() - engine_monitor.frame_data.frame_time) * 1000.0f;
    engine_monitor_ = engine_monitor;
    if (time_controller > 1.0) {
      engine_monitor_.fps = frames;
      LOG(INFO) << "Frame rate: " << frames;
      time_controller = 0.0;
      frames = 0;
    }
  }
}

void Engine::AttachLayer(std::unique_ptr<Layer> obs) {
  LOG(INFO) << __FUNCTION__;
  layers_.emplace_back(std::move(obs));
  layers_.back()->RegisterEngine(std::move(shared_from_this()));
  layers_.back()->OnAttach();
}

int Engine::AttachStructure(std::unique_ptr<CustomLayerStructure> str) {
  custom_layers_.emplace_back(std::move(str));
  for (auto& layer : custom_layers_.back()->layer_array) {
    layer->RegisterEngine(shared_from_this());
  }
  return custom_layers_.size() - 1;
}

void Engine::AttachLayerToStructure(std::unique_ptr<Layer> layer,
                                    int custom_layer_index) {
  custom_layers_[custom_layer_index]->layer_array.push_back(std::move(layer));
  custom_layers_[custom_layer_index]->layer_array.back()->RegisterEngine(
      shared_from_this());
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

std::unique_ptr<Layer>& Engine::operator[](std::size_t layer_index) {
  return layers_[layer_index];
}

const EngineMonitorData& Engine::GetEngineMonitor() const {
  return engine_monitor_;
}

bool Engine::operator==(const Identity& id) {
  return *this == id;
}
